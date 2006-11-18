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

#include <config.h>
#include <gtkmm/main.h>

#include "gNinjamClient.hh"

#include <WDL/string.h>
#include <WDL/ptrlist.h>
#include <ninjam/audiostream.h>
#include <ninjam/njclient.h>

#include "dialog_license.hh"


dialog_license *d_license = NULL;
gNinjamClient *window = NULL;
int g_chat_scroll;
int curs_ypos,curs_xpos;
int color_map[8];
int g_ui_inchat;
int g_done;
int g_ui_state;
int g_ui_locrename_ch;
int g_ui_voltweakstate_channel;
int g_need_disp_update;
char m_lineinput_str[120];
char m_chatinput_str[120];

WDL_PtrList<char> g_chat_buffers;
Glib::ustring g_topic;

audioStreamer *g_audio;
NJClient *g_client;
void audiostream_onunder() { }
void audiostream_onover() { }
int g_audio_enable = 0;

#include <WDL/dirscan.h>
#include <WDL/lineparse.h>

#include <ninjam/njmisc.h>
#include <iostream>

#define VALIDATE_TEXT_CHAR(thischar) ((isspace(thischar) || isgraph(thischar)) && (thischar) < 256)

void chatmsg_cb(int user32, NJClient *inst, const char **parms, int nparms)
{
  if (!parms[0]) return;

  Glib::ustring tmp;
  if (!strcmp(parms[0],"TOPIC")) {
    if (parms[2]) {
      tmp = "*** ";
      if (parms[1] && *parms[1]) {
        tmp += parms[1];
        tmp += " sets topic to: ";
      } else
	tmp += "Topic is: ";
      tmp += parms[2];

      window->setChatTopic(parms[2]);
      window->addChatText(tmp);
    }
  } else if (!strcmp(parms[0],"MSG")) {
    if (parms[1] && parms[2]) {
      tmp = "<";
      tmp += parms[1];
      tmp += "> ";
      tmp += parms[2];
      window->addChatText(tmp);
    }
  } else if (!strcmp(parms[0],"PRIVMSG")) {
    if (parms[1] && parms[2]) {
      tmp = "*";
      tmp += parms[1];
      tmp += "* ";
      tmp += parms[2];
      window->addChatText(tmp);
    }
  } else if (!strcmp(parms[0],"JOIN") || !strcmp(parms[0],"PART")) {
    if (parms[1] && *parms[1]) {
      tmp = "*** ";
      tmp += parms[1];
      tmp += " has ";
      tmp += (parms[0][0]=='P' ? "left" : "joined");
      tmp += " the server";
      window->addChatText(tmp);
    }
  } 
}

int licensecallback(int user32, char *licensetext)
{
  d_license->set_text(licensetext);
  d_license->show();
  int response_id = d_license->run();
  d_license->hide();
  if (response_id == Gtk::RESPONSE_ACCEPT)
    return true;
  else
    return false;
}

void audiostream_onsamples(float **inbuf, int innch, float **outbuf, int outnch, int len, int srate) 
{ 
  if (!g_audio_enable) 
  {
    int x;
    // clear all output buffers
    for (x = 0; x < outnch; x ++) memset(outbuf[x],0,sizeof(float)*len);
    return;
  }
  g_client->AudioProc(inbuf,innch, outbuf, outnch, len,srate);
}

void sigfunc(int sig)
{
  Gtk::Main::quit();
}

#include <gconfmm.h>

int main(int argc, char **argv)
{  
  Gtk::Main m(&argc, &argv);

  int nolog=0,nowav=1,writeogg=0,g_nssf=0;
  d_license = new class dialog_license();
  g_chat_scroll = 0;
  g_ui_inchat = 0;
  g_done = 0;
  g_ui_state = 0;
  g_audio_enable = 0;
  g_client=new NJClient;
  g_client->config_savelocalaudio=0;
  g_client->LicenseAgreementCallback=licensecallback;
  g_client->ChatMessage_Callback=chatmsg_cb;

  Gnome::Conf::init();
  Glib::RefPtr<Gnome::Conf::Client> gconf_client = Gnome::Conf::Client::get_default_client();
  gconf_client->set_error_handling(Gnome::Conf::CLIENT_HANDLE_ALL);
  Glib::ustring gconf_dir = "/apps/gninjam";
  Glib::ustring gconf_prefdir = gconf_dir+"/preferences";
  gconf_client->add_dir(gconf_dir);

  if (gconf_client->get_int(gconf_prefdir+"/audio_driver") == 0) {
    g_audio = create_audioStreamer_JACK(NULL,
					audiostream_onsamples,
					g_client);
  } else {
    Glib::ustring audiocfgstr = gconf_client->get_string(gconf_prefdir+"audio_config_string");
    char* cfgstr = new char[audiocfgstr.size()+1];
    memcpy(cfgstr, audiocfgstr.c_str(), audiocfgstr.size());

    g_audio = create_audioStreamer_ALSA(cfgstr,
					audiostream_onsamples);
    delete cfgstr;
  }
  if (!g_audio) {
    printf("Error opening audio!\n");
    return 0;
  }

  signal(SIGINT,sigfunc);

  JNL::open_socketlib();

  {
    if (gconf_client->dir_exists(gconf_dir+"/master")) {
      g_client->config_mastervolume = gconf_client->get_float(gconf_dir+"/master/master_volume");
      g_client->config_masterpan = gconf_client->get_float(gconf_dir+"/master/master_pan");
      g_client->config_metronome = gconf_client->get_float(gconf_dir+"/master/metronome_volume");
      g_client->config_metronome_pan = gconf_client->get_float(gconf_dir+"/master/metronome_pan");
      g_client->config_mastermute = gconf_client->get_bool(gconf_dir+"/master/master_mute");
      g_client->config_metronome_mute = gconf_client->get_bool(gconf_dir+"/master/metronome_mute");
    }
    unsigned localchannel = 0;
    int a = 0;
    for (localchannel = 0; localchannel<g_client->GetMaxLocalChannels(); localchannel++) {
      char localdir[20];
      snprintf(localdir, sizeof(localdir), "/local%d", localchannel);
      Glib::ustring localpath = gconf_dir+localdir;
      if (gconf_client->dir_exists(localpath) &&
	  gconf_client->get_bool(localpath+"/active")) {
	g_client->SetLocalChannelMonitoring(a,
					    true, gconf_client->get_float(localpath+"/volume"),
					    true, gconf_client->get_float(localpath+"/pan"),
					    true, gconf_client->get_bool(localpath+"/mute"),
					    true, gconf_client->get_bool(localpath+"/solo"));
	g_client->SetLocalChannelInfo(a,
				      gconf_client->get_string(localpath+"/name").c_str(),
				      true, gconf_client->get_int(localpath+"/source"),
				      false, gconf_client->get_int(localpath+"/bitrate"),
				      true, gconf_client->get_bool(localpath+"/broadcast"));
	a++;
      } else {
	break;
      }
    }
    if (localchannel == 0) {
      g_client->SetLocalChannelInfo(0,"new channel",true,0,false,0,true,true);
      g_client->SetLocalChannelMonitoring(0,false,0.0f,false,0.0f,false,false,false,false);
    }
  }

  Glib::ustring sessiondir = gconf_client->get_string(gconf_prefdir+"/session_directory");
  if (sessiondir == "") {
    char buf[512];
    
    int cnt = 0;
    while (cnt < 16) {
      time_t tv;
      time(&tv);
      struct tm *t = localtime(&tv);
      sprintf(buf,
	      "%04d%02d%02d_%02d%02d",
	      t->tm_year+1900,
	      t->tm_mon+1,
	      t->tm_mday,
	      t->tm_hour,
	      t->tm_min);
      if (cnt)
	wsprintf(buf+strlen(buf), "_%d", cnt);
      strcat(buf, ".ninjam");
      
      if (!mkdir(buf, 0700)) break;
      cnt++;
    }
    if (cnt >= 16) {
      printf("Error creating session directory\n");
      buf[0] = 0;
      return 0;
    }
    sessiondir = buf;
  } else {
    mkdir(sessiondir.c_str(), 0700);
  }
  if ((sessiondir != "") &&
      (sessiondir[sessiondir.size()-1] != '\\') &&
      (sessiondir[sessiondir.size()-1] != '/')) {
    sessiondir += "/";
  }
  g_client->SetWorkDir(sessiondir.c_str());

  switch (gconf_client->get_int(gconf_prefdir+"/savelocalaudio")) {
  case 2:
    if (g_client->waveWrite != NULL)
      delete g_client->waveWrite;
    g_client->waveWrite = new WaveWriter((sessiondir+"output.wav").c_str(),
					 24,
					 g_audio->m_outnch>1?2:1,
					 g_audio->m_srate);
  case 1:
    g_client->SetOggOutFile(fopen((sessiondir+"output.ogg").c_str(),"ab"),
			    g_audio->m_srate,
			    g_audio->m_outnch>1?2:1,
			    gconf_client->get_int(gconf_prefdir+"/ogg_bitrate"));
  case 0:
    g_client->config_savelocalaudio = 0;
    break;
  default:
    g_client->config_savelocalaudio = -1;
    break;
  }
  
  if (gconf_client->get_bool(gconf_prefdir+"/save_log")) {
    g_client->SetLogFile((sessiondir+"clipsort.log").c_str());
  }
  
  g_audio_enable=1;

  window = new class gNinjamClient();
  m.run(*window);
  delete window;
  delete d_license;
  delete g_audio;


  delete g_client->waveWrite;
  g_client->waveWrite=0;


  // save local channel state
  {
    gconf_client->set(gconf_dir+"/master/master_volume", g_client->config_mastervolume);
    gconf_client->set(gconf_dir+"/master/master_pan", g_client->config_masterpan);
    gconf_client->set(gconf_dir+"/master/metronome_volume", g_client->config_metronome);
    gconf_client->set(gconf_dir+"/master/metronome_pan", g_client->config_metronome_pan);
    gconf_client->set(gconf_dir+"/master/master_mute", g_client->config_mastermute);
    gconf_client->set(gconf_dir+"/master/metronome_mute", g_client->config_metronome_mute);

    unsigned localchannel;
    for (localchannel = 0;;localchannel++) {
      int a = g_client->EnumLocalChannels(localchannel);
      if (a<0) break;

      int sch = 0, bitrate = 0;
      bool broadcast = 0;
      float v = 0.0f, p = 0.0f;
      bool m = 0, s = 0;
      
      Glib::ustring name = g_client->GetLocalChannelInfo(a,
							 &sch,
							 &bitrate,
							 &broadcast);
      g_client->GetLocalChannelMonitoring(a, &v,&p,&m,&s);

      char localdir[20];
      snprintf(localdir, sizeof(localdir), "/local%d", localchannel);
      Glib::ustring localpath = gconf_dir+localdir;
      gconf_client->set(localpath+"/active", true);
      gconf_client->unset(localpath+"/name");
      gconf_client->set(localpath+"/name", name);
      gconf_client->set(localpath+"/source", sch);
      gconf_client->set(localpath+"/bitrate", bitrate);
      gconf_client->set(localpath+"/broadcast", broadcast);
      gconf_client->set(localpath+"/volume", v);
      gconf_client->set(localpath+"/pan", p);
      gconf_client->set(localpath+"/mute", m);
      gconf_client->set(localpath+"/solo", s);
    }
    for (; localchannel < g_client->GetMaxLocalChannels(); localchannel++) {
      char localdir[20];
      snprintf(localdir, sizeof(localdir), "/local%d", localchannel);
      Glib::ustring localpath = gconf_dir+localdir;

      if (gconf_client->dir_exists(localpath)) {
	gconf_client->set(localpath+"/active", false);
      } else {
	break;
      }
    }
  }

  delete g_client;

  /*
  if (g_nssf) {
    int n;
    for (n = 0; n < 16; n ++) {
      WDL_String s(sessiondir.Get());
      char buf[32];
      sprintf(buf,"%x",n);
      s.Append(buf);

      {
        WDL_DirScan ds;
        if (!ds.First(s.Get())) {
          do {
            if (ds.GetCurrentFN()[0] != '.') {
              WDL_String t;
              ds.GetCurrentFullFN(&t);
              unlink(t.Get());          
            }
          }
          while (!ds.Next());
        }
      }
      rmdir(s.Get());
    }
  }
  if (!sessionspec) {
      rmdir(sessiondir.Get());
  }
  */
  JNL::close_socketlib();

  return 0;
}
