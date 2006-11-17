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

int main(int argc, char **argv)
{  
  Gtk::Main m(&argc, &argv);

  WDL_String sessiondir;
  int sessionspec=0;
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

  {
    char *dev_name_in = NULL;
    g_audio=create_audioStreamer_JACK(dev_name_in,audiostream_onsamples,g_client);
  }
  if (!g_audio)
  {
    printf("Error opening audio!\n");
    return 0;
  }

  signal(SIGINT,sigfunc);

  JNL::open_socketlib();

  {
    FILE *fp=fopen("ninjam.config","rt");
    int x=0;
    if (fp) 
    {
      bool comment_state=false;
      while (!feof(fp))
      {
        char buf[4096];
        buf[0]=0;
        fgets(buf,sizeof(buf),fp);
        if (!buf[0]) continue;
        if (buf[strlen(buf)-1] == '\n')
          buf[strlen(buf)-1]=0;
        if (!buf[0]) continue;

        LineParser lp(comment_state);

        lp.parse(buf);

        switch (lp.gettoken_enum(0,"local\0master\0"))
        {
          case 0:
            // process local line
            if (lp.getnumtokens()>2)
            {
              int ch=lp.gettoken_int(1);
              int n;
              for (n = 2; n < lp.getnumtokens()-1; n += 2)
              {
                switch (lp.gettoken_enum(n,"source\0bc\0mute\0solo\0volume\0pan\0jesus\0name\0"))
                {
                  case 0: // source 
                    g_client->SetLocalChannelInfo(ch,NULL,true,lp.gettoken_int(n+1),false,0,false,false);
                  break;
                  case 1: //broadcast
                    g_client->SetLocalChannelInfo(ch,NULL,false,false,false,0,true,!!lp.gettoken_int(n+1));
                  break;
                  case 2: //mute
                    g_client->SetLocalChannelMonitoring(ch,false,false,false,false,true,!!lp.gettoken_int(n+1),false,false);
                  break;
                  case 3: //solo
                    g_client->SetLocalChannelMonitoring(ch,false,false,false,false,false,false,true,!!lp.gettoken_int(n+1));
                  break;
                  case 4: //volume
                    g_client->SetLocalChannelMonitoring(ch,true,(float)lp.gettoken_float(n+1),false,false,false,false,false,false);
                  break;
                  case 5: //pan
                    g_client->SetLocalChannelMonitoring(ch,false,false,true,(float)lp.gettoken_float(n+1),false,false,false,false);
                  break;
                  case 6: //jesus
                    if (lp.gettoken_int(n+1))
                    {
                    }
                  break;
                  case 7: //name
                    g_client->SetLocalChannelInfo(ch,lp.gettoken_str(n+1),false,false,false,0,false,false);
                  break;
                  default:
                  break;
                }
              }
            }

          break;
          case 1:
            if (lp.getnumtokens()>2)
            {
              int n;
              for (n = 1; n < lp.getnumtokens()-1; n += 2)
              {
                switch (lp.gettoken_enum(n,"mastervol\0masterpan\0metrovol\0metropan\0mastermute\0metromute\0"))
                {
                  case 0: // mastervol
                    g_client->config_mastervolume = (float)lp.gettoken_float(n+1);
                  break;
                  case 1: // masterpan
                    g_client->config_masterpan = (float)lp.gettoken_float(n+1);
                  break;
                  case 2:
                    g_client->config_metronome = (float)lp.gettoken_float(n+1);
                  break;
                  case 3:
                    g_client->config_metronome_pan = (float)lp.gettoken_float(n+1);
                  break;
                  case 4:
                    g_client->config_mastermute = !!lp.gettoken_int(n+1);
                  break;
                  case 5:
                    g_client->config_metronome_mute = !!lp.gettoken_int(n+1);
                  break;
                  default:
                  break;
                }
              }
            }
          break;
          default:
          break;
        }       


      }
      fclose(fp);
    }    
    else // set up defaults
    {
      g_client->SetLocalChannelInfo(0,"channel0",true,0,false,0,true,true);
      g_client->SetLocalChannelMonitoring(0,false,0.0f,false,0.0f,false,false,false,false);
    }
  }

  if (!sessiondir.Get()[0])
  {
    char buf[512];
    
    int cnt=0;
    while (cnt < 16)
    {
      time_t tv;
      time(&tv);
      struct tm *t=localtime(&tv);
      sprintf(buf,"%04d%02d%02d_%02d%02d",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min);
      if (cnt)
        wsprintf(buf+strlen(buf),"_%d",cnt);
      strcat(buf,".ninjam");

      if (!mkdir(buf,0700)) break;

      cnt++;
    }
    
    if (cnt >= 16)
    {
      printf("Error creating session directory\n");
      buf[0]=0;
      return 0;
    }
      
    sessiondir.Set(buf);
  }
  else
    mkdir(sessiondir.Get(),0700);
  if (sessiondir.Get()[0] && sessiondir.Get()[strlen(sessiondir.Get())-1]!='\\' && sessiondir.Get()[strlen(sessiondir.Get())-1]!='/')
    sessiondir.Append("/");

  g_client->SetWorkDir(sessiondir.Get());


  if (!nowav)
  {
    WDL_String wf;
    wf.Set(sessiondir.Get());
    wf.Append("output.wav");
    g_client->waveWrite = new WaveWriter(wf.Get(),24,g_audio->m_outnch>1?2:1,g_audio->m_srate);
  }
  if (writeogg)
  {
    WDL_String wf;
    wf.Set(sessiondir.Get());
    wf.Append("output.ogg");
    g_client->SetOggOutFile(fopen(wf.Get(),"ab"),g_audio->m_srate,g_audio->m_outnch>1?2:1,writeogg);
  }
  if (!nolog)
  {
    WDL_String lf;
    lf.Set(sessiondir.Get());
    lf.Append("clipsort.log");
    g_client->SetLogFile(lf.Get());
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
    FILE *fp=fopen("ninjam.config","wt");
    int x=0;
    if (fp) {
      fprintf(fp,"master mastervol %f masterpan %f metrovol %f metropan %f mastermute %d metromute %d\n",
        g_client->config_mastervolume,g_client->config_masterpan,g_client->config_metronome,g_client->config_metronome_pan,
        g_client->config_mastermute,g_client->config_metronome_mute);



      for (x = 0;;x++) {
        int a=g_client->EnumLocalChannels(x);
        if (a<0) break;


        int sch=0;
        bool bc=0;
        void *has_jesus=0;
        char *lcn;
        float v=0.0f,p=0.0f;
        bool m=0,s=0;
      
        lcn=g_client->GetLocalChannelInfo(a,&sch,NULL,&bc);
        g_client->GetLocalChannelMonitoring(a,&v,&p,&m,&s);
        g_client->GetLocalChannelProcessor(a,NULL,&has_jesus);

        char *ptr=lcn;
        while (*ptr) {
          if (*ptr == '`') *ptr='\'';
          ptr++;
        }
        fprintf(fp,"local %d source %d bc %d mute %d solo %d volume %f pan %f jesus %d name `%s`\n",a,sch,bc,m,s,v,p,!!has_jesus,lcn);
      }
      fclose(fp);
    }    
  }


  // delete all effects processors in g_client
  {
    int x=0;
    for (x = 0;;x++) {
      int a=g_client->EnumLocalChannels(x);
      if (a<0) break;
    }
  }


  delete g_client;


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

  JNL::close_socketlib();

  return 0;
}
