/*
    Copyright (C) 2006 Tobias Gehrig <tobias@gehrignet.de>
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "config.h"
#include "gNinjamClient.hh"

#include <gtkmm/main.h>

#ifdef _WIN32
#include <windows.h>
#define strncasecmp strnicmp
#else
#include <stdlib.h>
#include <memory.h>
#endif

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <signal.h>
#include <float.h>

#include <iostream>

#include <ninjam/njmisc.h>
#include "common.hh"

#define TIMEOUT_VALUE 50

#include <ninjam/audiostream.h>
#include <ninjam/njclient.h>

extern audioStreamer *g_audio;
extern NJClient *g_client;

gNinjamClient::gNinjamClient()
  : d_connect(new class dialog_connect()),
    w_preferences(new class window_preferences())
{
  hscale_master_pan->signal_format_value().connect(sigc::ptr_fun(on_hscale_pan_format_value), false);
  hscale_metronome_pan->signal_format_value().connect(sigc::ptr_fun(on_hscale_pan_format_value), false);
  hscale_master_volume->signal_format_value().connect(sigc::ptr_fun(on_hscale_volume_format_value), false);
  hscale_metronome_volume->signal_format_value().connect(sigc::ptr_fun(on_hscale_volume_format_value), false);

  hscale_master_volume->set_value(VAL2DB(g_client->config_mastervolume));
  hscale_master_pan->set_value(g_client->config_masterpan);
  checkbutton_master_mute->set_active(g_client->config_mastermute);
  hscale_metronome_volume->set_value(VAL2DB(g_client->config_metronome));
  hscale_metronome_pan->set_value(g_client->config_metronome_pan);
  checkbutton_metronome_mute->set_active(g_client->config_metronome_mute);

  vbox_local->update();

  sigc::slot<bool> my_slot = sigc::mem_fun(*this, &gNinjamClient::on_timeout);
  sigc::connection conn = Glib::signal_timeout().connect(my_slot, TIMEOUT_VALUE);
  sigc::slot<bool> my_slot2 = sigc::mem_fun(*this, &gNinjamClient::on_timeout_gui);
  sigc::connection conn2 = Glib::signal_timeout().connect(my_slot2, 100);

  Glib::RefPtr<Gnome::Conf::Client> gconf_client = Gnome::Conf::Client::get_default_client();
  Glib::ustring gconf_dir = "/apps/gninjam/preferences";
  d_connect->entry_hostname->set_text(gconf_client->get_string(gconf_dir+"/hostname"));
  d_connect->entry_username->set_text(gconf_client->get_string(gconf_dir+"/username"));
  d_connect->entry_password->set_text(gconf_client->get_string(gconf_dir+"/password"));
}

gNinjamClient::~gNinjamClient()
{
  delete d_connect;
  delete w_preferences;
}

bool gNinjamClient::on_timeout()
{
  int status = g_client->GetStatus();
  if (status >= 0) {
    if (g_client->Run()) {}
  }
  return true;
}

bool gNinjamClient::on_timeout_gui()
{
  int status = g_client->GetStatus();
  Glib::ustring statusmsg = "Status: ";
  switch (status) {
  case NJClient::NJC_STATUS_OK:
    statusmsg += "Connected to ";
    statusmsg += g_client->GetHostName();
    statusmsg += " as ";
    statusmsg += g_client->GetUserName();
    break;
  case NJClient::NJC_STATUS_INVALIDAUTH:
    statusmsg += "ERROR: invalid login/password";
    break;
  case NJClient::NJC_STATUS_CANTCONNECT:
    statusmsg += "ERROR: failed connecting to host";
    break;
  case NJClient::NJC_STATUS_PRECONNECT:
    statusmsg += "Not connected";
    break;
  case NJClient::NJC_STATUS_DISCONNECTED:
    statusmsg += "ERROR: disconnected from host";
    break;
    
  default:
    char statusnum[8];
    snprintf(statusnum, sizeof(statusnum), "%d", status);
    statusmsg += "CODE = ";
    statusmsg += statusnum;
    break;
  }
  if (g_client->GetErrorStr()[0]) {
    statusmsg += ". Server gave explanation: ";
    statusmsg += g_client->GetErrorStr();
  }
  if (status >= 0) {
    int pos, len;
    g_client->GetPosition(&pos,&len);
    progressbar1->set_fraction(1.0*pos/len);
    int bpi = g_client->GetBPI();
    char output[11];
    snprintf(output, sizeof(output), "%d", (pos*bpi)/len);
    Glib::ustring beatmsg = output;
    beatmsg += "/";
    snprintf(output, sizeof(output), "%d", bpi);
    beatmsg += output;
    beatmsg += " @ ";
    snprintf(output, sizeof(output), "%.1f", g_client->GetActualBPM());
    beatmsg += output;
    beatmsg += " BPM ";
    progressbar1->set_text(beatmsg);
    float value = VAL2DB(g_client->GetOutputPeak());
    progressbar_master->set_fraction((value+120)/140);
    snprintf(output, sizeof(output), "%.2f dB", value);
    progressbar_master->set_text(output);
    vbox_local->update_VUmeters();
    vbox_remote->update();
  }
  if (g_audio) {
    statusmsg += " : ";
    char output[16];
    snprintf(output, sizeof(output), "%d", g_audio->m_srate);
    statusmsg += output;
    statusmsg += " Hz ";
    snprintf(output, sizeof(output), "%d", g_audio->m_innch);
    statusmsg += output;
    statusmsg += "ch->";
    snprintf(output, sizeof(output), "%d", g_audio->m_outnch);
    statusmsg += output;
    statusmsg += "ch ";
    snprintf(output, sizeof(output), "%d", g_audio->m_bps);
    statusmsg += output;
    statusmsg += "bps";
  }
  statusbar1->push(statusmsg);
  return true;
}

void gNinjamClient::on_neu1_activate()
{
  int idx, maxchannels = g_client->GetMaxLocalChannels();
  for (idx = 0; (idx < maxchannels) && g_client->GetLocalChannelInfo(idx,NULL,NULL,NULL); idx++);
  if (idx < maxchannels) {
    g_client->SetLocalChannelInfo(idx,"new channel",true,0,false,0,true,true);
    // g_client->SetLocalChannelMonitoring(idx,false,0.0f,false,0.0f,false,false,false,false);
    g_client->NotifyServerOfChannelChange();
    vbox_local->add_channel(idx);
  }
}

void gNinjamClient::on_verbinden1_activate()
{
  d_connect->show();
  int response_id = d_connect->run();
  d_connect->hide();
  if (response_id == Gtk::RESPONSE_OK) {
    Glib::ustring hostname = d_connect->entry_hostname->get_text();
    Glib::ustring username = d_connect->entry_username->get_text();
    Glib::ustring password = d_connect->entry_password->get_text();
    g_client->Connect(hostname.c_str(), username.c_str(), password.c_str());
  }
}

void gNinjamClient::on_trennen1_activate()
{
  g_client->Disconnect();
}

void gNinjamClient::on_beenden1_activate()
{
  Gtk::Main::quit();
}

void gNinjamClient::on_ausschneiden1_activate()
{  
}

void gNinjamClient::on_kopieren1_activate()
{  
}

void gNinjamClient::on_einf__gen1_activate()
{  
}

void gNinjamClient::on_l__schen1_activate()
{  
}

void gNinjamClient::on_einstellungen1_activate()
{
  w_preferences->show();
}

void gNinjamClient::on_info1_activate()
{  
}

void gNinjamClient::on_hscale_master_volume_value_changed()
{
  g_client->config_mastervolume = DB2VAL(hscale_master_volume->get_value());
}

void gNinjamClient::on_hscale_master_pan_value_changed()
{
  g_client->config_masterpan = hscale_master_pan->get_value();
}

void gNinjamClient::on_hscale_metronome_volume_value_changed()
{
  g_client->config_metronome = DB2VAL(hscale_metronome_volume->get_value());
}

void gNinjamClient::on_hscale_metronome_pan_value_changed()
{
  g_client->config_metronome_pan = hscale_metronome_pan->get_value();
}

void gNinjamClient::on_checkbutton_master_mute_toggled()
{
  g_client->config_mastermute = checkbutton_master_mute->get_active();  
}

void gNinjamClient::on_checkbutton_metronome_mute_toggled()
{
  g_client->config_metronome_mute = checkbutton_metronome_mute->get_active();
}

void gNinjamClient::on_chat_entry_activate()
{
  Glib::ustring text = chat_entry->get_text();
  chat_entry->set_text("");
  
  if (text.size() > 0) {
    if (text[0] == '/') {
      if (!strncasecmp(text.c_str(), "/me ", 4)) {
	g_client->ChatMessage_Send("MSG", text.c_str());
      } else if (!strncasecmp(text.c_str(), "/topic ", 7)||
		 !strncasecmp(text.c_str(), "/kick ", 6) ||                        
		 !strncasecmp(text.c_str(), "/bpm ", 5) ||
		 !strncasecmp(text.c_str(), "/bpi ", 5)
		 ) { // alias to /admin *
	g_client->ChatMessage_Send("ADMIN", text.c_str()+1);
      } else if (!strncasecmp(text.c_str(), "/admin ", 7)) {
	const char *p=text.c_str()+7;
	while (*p == ' ') p++;
	g_client->ChatMessage_Send("ADMIN", p);
      } else if (!strncasecmp(text.c_str(), "/msg ", 5)) {
	unsigned i = 5;
	while (text[i] == ' ') i++;
	Glib::ustring name;
	unsigned n = i;
	while ((n < text.size()) && (text[n] != ' ')) n++;
	name = text.substr(i, n-i);
	Glib::ustring msg = text.substr(n);
	if (msg.size() > 0) {
	  g_client->ChatMessage_Send("PRIVMSG", name.c_str(), msg.c_str());
	  Glib::ustring tmp;
	  tmp = "-> *";
	  tmp += name;
	  tmp += "* ";
	  tmp += msg;
	  addChatText(tmp);
	} else {
	  addChatText("*** error: /msg requires a username and a message.");
	}
      } else {
	addChatText("*** error: unknown command.");
      }
    } else {
      g_client->ChatMessage_Send("MSG", text.c_str());
    }
  }
}

void gNinjamClient::addChatText(Glib::ustring text)
{
  Glib::RefPtr<Gtk::TextBuffer> buffer = textview1->get_buffer();
  Glib::ustring buffertext = buffer->get_text();
  buffer->set_text(buffertext+text+"\n");
}

void gNinjamClient::setChatTopic(Glib::ustring text)
{
  label_chat->set_text(text);
}

void gNinjamClient::update_inputLists()
{
  vbox_local->update_inputLists();
}
