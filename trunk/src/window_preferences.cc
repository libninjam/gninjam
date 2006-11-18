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
#include "window_preferences.hh"

#include <gtkmm/liststore.h>

#include <ninjam/audiostream.h>
#include <ninjam/njclient.h>

extern audioStreamer *g_audio;
extern NJClient *g_client;
extern int g_audio_enable;
extern void audiostream_onsamples(float **inbuf, int innch, float **outbuf, int outnch, int len, int srate);

window_preferences::window_preferences()
  : _gconf_client(Gnome::Conf::Client::get_default_client()),
    _gconf_dir("/apps/gninjam/preferences")
{
  Gtk::TreeModel::ColumnRecord column_model;
  Gtk::TreeModelColumn<Glib::ustring> textcolumn;
  column_model.add(textcolumn);
  Glib::RefPtr<Gtk::ListStore> model = Gtk::ListStore::create(column_model);
  combobox_audiodriver->set_model(model);
  Gtk::TreeModel::Row row = *(model->append());
  row[textcolumn] = "Jack";
  row = *(model->append());
  row[textcolumn] = "Alsa";
  combobox_audiodriver->pack_start(textcolumn);

  model = Gtk::ListStore::create(column_model);
  combobox_savelocalaudio->set_model(model);
  row = *(model->append());
  row[textcolumn] = "None";
  row = *(model->append());
  row[textcolumn] = "Ogg Files";
  row = *(model->append());
  row[textcolumn] = "Ogg+Wave Files";
  row = *(model->append());
  row[textcolumn] = "Delete Ogg Files as soon as possible";
  combobox_savelocalaudio->pack_start(textcolumn);
}

void window_preferences::on_button_abort_clicked()
{
  this->hide();
}

void window_preferences::on_button_apply_clicked()
{
  int audiodriver = combobox_audiodriver->get_active_row_number();
  Glib::ustring audiocfgstr = entry_audioconfigstring->get_text();
  if (audiodriver == 0) {
    if (audiodriver != _gconf_client->get_int(_gconf_dir+"/audio_driver")) {
      g_audio_enable = 0;
      if (g_audio != NULL)
	delete g_audio;
      g_audio = create_audioStreamer_JACK(NULL,
					  audiostream_onsamples,
					  g_client);
      g_audio_enable = 1;
    }
  } else {
    if ((audiodriver != _gconf_client->get_int(_gconf_dir+"/audio_driver")) ||
	(audiocfgstr != _gconf_client->get_string(_gconf_dir+"/audio_config_string"))) {
      g_audio_enable = 0;
      if (g_audio != NULL)
	delete g_audio;
      char* cfgstr = new char[audiocfgstr.size()+1];
      memcpy(cfgstr, audiocfgstr.c_str(), audiocfgstr.size());
      g_audio = create_audioStreamer_ALSA(cfgstr,
					  audiostream_onsamples);
      delete cfgstr;
      g_audio_enable = 1;
    }
  }
  Glib::ustring session_dir = entry_sessiondir->get_text();
  Glib::ustring sessiondir;
  if (session_dir != _gconf_client->get_string(_gconf_dir+"/session_directory")) {
    if (session_dir == "") {
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
	return;
      }
      sessiondir = buf;
    } else {
      sessiondir = session_dir;
      mkdir(sessiondir.c_str(), 0700);
    }
    if ((sessiondir != "") &&
	(sessiondir[sessiondir.size()-1] != '\\') &&
	(sessiondir[sessiondir.size()-1] != '/')) {
      sessiondir += "/";
    }
    g_client->SetWorkDir(sessiondir.c_str());
  }
  sessiondir = g_client->GetWorkDir();
  int savelocalaudio = combobox_savelocalaudio->get_active_row_number();
  int oggbitrate = (int)spinbutton_oggbitrate->get_value();
  if ((savelocalaudio != _gconf_client->get_int(_gconf_dir+"/savelocalaudio")) ||
      ((oggbitrate != _gconf_client->get_int(_gconf_dir+"/ogg_bitrate")) &&
       (savelocalaudio > 0))) {
    switch (savelocalaudio) {
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
			      oggbitrate);
    case 0:
      g_client->config_savelocalaudio = savelocalaudio;
      break;
    default:
      g_client->config_savelocalaudio = -1;
      break;
    }
  }
  bool savelog = checkbutton_savelog->get_active();
  if (savelog != _gconf_client->get_bool(_gconf_dir+"/save_log")) {
    if (savelog) {
      g_client->SetLogFile((sessiondir+"clipsort.log").c_str());
    } else {
      g_client->SetLogFile(NULL);
    }
  }
  _gconf_client->set(_gconf_dir+"/audio_driver",
		     audiodriver);
  _gconf_client->set(_gconf_dir+"/audio_config_string",
		     audiocfgstr);
  _gconf_client->set(_gconf_dir+"/session_directory",
		     session_dir);
  _gconf_client->set(_gconf_dir+"/savelocalaudio",
		     g_client->config_savelocalaudio);
  _gconf_client->set(_gconf_dir+"/ogg_bitrate",
		     oggbitrate);
  _gconf_client->set(_gconf_dir+"/save_log",
		     savelog);
}

void window_preferences::on_button_ok_clicked()
{
  on_button_apply_clicked();
  this->hide();
}

void window_preferences::on_window_preferences_show()
{
  combobox_audiodriver->set_active(_gconf_client->get_int(_gconf_dir+"/audio_driver"));
  entry_audioconfigstring->set_text(_gconf_client->get_string(_gconf_dir+"/audio_config_string"));
  entry_sessiondir->set_text(_gconf_client->get_string(_gconf_dir+"/session_directory"));
  if (g_client->config_savelocalaudio >= 0) {
    combobox_savelocalaudio->set_active(g_client->config_savelocalaudio);
  } else {
    combobox_savelocalaudio->set_active(3);
  }
  spinbutton_oggbitrate->set_value(_gconf_client->get_int(_gconf_dir+"/ogg_bitrate"));
  checkbutton_savelog->set_active(_gconf_client->get_bool(_gconf_dir+"/save_log"));
}
