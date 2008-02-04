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
/*
 * Standard gettext macros.
 */
#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (GETTEXT_PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif

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

#include <gtkmm/liststore.h>

#include <ninjam/audiostream.h>
#include <ninjam/njclient.h>

extern audioStreamer *g_audio;
extern NJClient *g_client;

gNinjamClient::gNinjamClient()
  : d_connect(new class dialog_connect()),
    w_preferences(new class window_preferences()),
    _old_status(0),
    _audio_status_changed(true),
    _dummy_adjustment(0.0, 0.0, 0.0, 0.0, 0.0, 0.0)
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
  checkbutton_metronome_stereo->set_active(g_client->config_metronome_stereoout);
  chat_entry->set_text("");

  _column_model.add(_textcolumn);
  combobox_metronome_output->pack_start(_textcolumn);

  // The dummy adjustment is necessary because of a Gtkmm bug: there's no way to pass
  // 0 as the first argument of this method, since it now takes references rather than
  // pointers.
  chat_log->set_scroll_adjustments(_dummy_adjustment, *chat_log_scroll->get_adjustment());

  int metrochan = g_client->config_metronome_channel;
  vbox_local->update();
  update_inputLists();
  update_outputLists();
  combobox_metronome_output->set_active(metrochan);

  sigc::slot<bool> my_slot = sigc::mem_fun(*this, &gNinjamClient::on_timeout);
  sigc::connection conn = Glib::signal_timeout().connect(my_slot, TIMEOUT_VALUE);
  connect_gui_status_update(100);
  Glib::RefPtr<Gnome::Conf::Client> gconf_client = Gnome::Conf::Client::get_default_client();
  Glib::ustring gconf_dir = "/apps/gninjam/preferences";
  d_connect->entry_hostname->set_text(gconf_client->get_string(gconf_dir+"/hostname"));
  d_connect->entry_username->set_text(gconf_client->get_string(gconf_dir+"/username"));
  d_connect->checkbutton_anonymous->set_active(gconf_client->get_bool(gconf_dir+"/anonymous_login"));
  d_connect->entry_password->set_text(gconf_client->get_string(gconf_dir+"/password"));
}

gNinjamClient::~gNinjamClient()
{
  delete d_connect;
  delete w_preferences;
}

void gNinjamClient::connect_gui_status_update(unsigned timeout)
{
  if (_gui_connection.connected())
    _gui_connection.disconnect();
  _gui_connection = Glib::signal_timeout().connect(sigc::mem_fun(*this, &gNinjamClient::on_timeout_gui),
						   timeout);
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
  if (status != _old_status) {
    _old_status = status;
    Glib::ustring statusmsg = _("Status: ");
    switch (status) {
    case NJClient::NJC_STATUS_OK:
      statusmsg += _("Connected to ");
      statusmsg += g_client->GetHostName();
      statusmsg += _(" as ");
      statusmsg += g_client->GetUserName();
      break;
    case NJClient::NJC_STATUS_INVALIDAUTH:
      statusmsg += _("ERROR: invalid login/password");
      break;
    case NJClient::NJC_STATUS_CANTCONNECT:
      statusmsg += _("ERROR: failed connecting to host");
      break;
    case NJClient::NJC_STATUS_PRECONNECT:
      statusmsg += _("Not connected");
      break;
    case NJClient::NJC_STATUS_DISCONNECTED:
      statusmsg += _("ERROR: disconnected from host");
      break;
      
    default:
      char statusnum[8];
      snprintf(statusnum, sizeof(statusnum), "%d", status);
      statusmsg += _("CODE = ");
      statusmsg += statusnum;
      break;
    }
    if (g_client->GetErrorStr()[0]) {
      statusmsg += _(". Server gave explanation: ");
      statusmsg += g_client->GetErrorStr();
    }
    label_connection_status->set_text(statusmsg);
  }
  if (_audio_status_changed && g_audio) {
    _audio_status_changed = false;
    char output[16];
    snprintf(output, sizeof(output), "%d", g_audio->m_srate);
    Glib::ustring statusmsg = output;
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
    label_audio_info->set_text(statusmsg);
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
    beatmsg += _(" BPM ");
    progressbar1->set_text(beatmsg);
    float value = VAL2DB(g_client->GetOutputPeak());
    progressbar_master->set_fraction((value+120)/140);
    snprintf(output, sizeof(output), "%.2f dB", value);
    progressbar_master->set_text(output);
    vbox_local->update_VUmeters();
    vbox_remote->update_VUmeters();
    if (g_client->HasUserInfoChanged())
      vbox_remote->update();
  }
  return true;
}

void gNinjamClient::on_neu1_activate()
{
  int idx, maxchannels = g_client->GetMaxLocalChannels();
  for (idx = 0; (idx < maxchannels) && g_client->GetLocalChannelInfo(idx,NULL,NULL,NULL); idx++);
  if (idx < maxchannels) {
    g_client->SetLocalChannelInfo(idx,_("new channel"),true,0,false,0,true,true);
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
    bool anonymous = d_connect->checkbutton_anonymous->get_active();
    Glib::ustring password = "";
    if (anonymous)
      username = "anonymous:" + username;
    else
      password = d_connect->entry_password->get_text();

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
	  addChatText(_("*** error: /msg requires a username and a message."));
	}
      } else {
	addChatText(_("*** error: unknown command."));
      }
    } else {
      g_client->ChatMessage_Send("MSG", text.c_str());
    }
  }
}

void gNinjamClient::addChatText(Glib::ustring text)
{
  Glib::RefPtr<Gtk::TextBuffer> buffer = chat_log->get_buffer();
  if (buffer->begin() != buffer->end()) {
    buffer->insert(buffer->end(), "\n");
  }
  buffer->insert(buffer->end(), text);

  Gtk::Adjustment *adj = chat_log_scroll->get_adjustment();
  adj->set_value(adj->get_upper());
}

void gNinjamClient::setChatTopic(Glib::ustring text)
{
  label_chat->set_markup(_("<b>Chat</b>: ")+Glib::Markup::escape_text(text));
}

void gNinjamClient::update_inputLists()
{
  vbox_local->update_inputLists();
  _audio_status_changed = true;
}

void gNinjamClient::update_outputLists()
{
  vbox_remote->update_outputLists();

  int active = combobox_metronome_output->get_active_row_number();
  Glib::RefPtr<Gtk::ListStore> model = Gtk::ListStore::create(_column_model);
  Gtk::TreeModel::Row row;
  for (int i=0; i < g_audio->m_outnch; i++) {
    row = *(model->append());
    row[_textcolumn] = g_audio->GetOutputChannelName(i);
  }
  row = *(model->append());
  row[_textcolumn] = _("New channel");
  combobox_metronome_output->set_model(model);
  combobox_metronome_output->set_active(active);

  _audio_status_changed = true;
}

void gNinjamClient::on_local_channels1_activate()
{
  if (frame_local->is_visible())
    frame_local->hide();
  else
    frame_local->show();
}

void gNinjamClient::on_remote_channels1_activate()
{  
  if (frame_remote->is_visible())
    frame_remote->hide();
  else
    frame_remote->show();
}

void gNinjamClient::on_chat1_activate()
{  
  if (frame_chat->is_visible())
    frame_chat->hide();
  else
    frame_chat->show();
}

void gNinjamClient::on_combobox_metronome_output_changed()
{
  int channel = combobox_metronome_output->get_active_row_number();
  if (g_audio && (channel == g_audio->m_outnch)) {
    if (g_audio->addOutputChannel()) {
      update_outputLists();
    }
  }
  g_client->config_metronome_channel = channel;
}

void gNinjamClient::on_checkbutton_metronome_stereo_toggled()
{
  g_client->config_metronome_stereoout = checkbutton_metronome_stereo->get_active();
}
