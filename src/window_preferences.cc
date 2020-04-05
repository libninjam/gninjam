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

#include "window_preferences.hh"
#include "gNinjamClient.hh"

#include <gtkmm.h>
#include <gtkmm/liststore.h>
#include <list>

#include <ninjam/audiostream.h>
#include <ninjam/njclient.h>

extern audioStreamer *g_audio;
extern NJClient *g_client;
extern int g_audio_enable;
extern void audiostream_onsamples(float **inbuf, int innch, float **outbuf, int outnch, int len, int srate);
extern gNinjamClient *window;

window_preferences::window_preferences()
  : _gconf_client(Gnome::Conf::Client::get_default_client()),
    _gconf_dir("/apps/gninjam/preferences")
{
  Gtk::TreeModel::ColumnRecord column_model;
  column_model.add(_textcolumn);
  Glib::RefPtr<Gtk::ListStore> model = Gtk::ListStore::create(column_model);
  combobox_audiodriver->set_model(model);
  Gtk::TreeModel::Row row = *(model->append());
  row[_textcolumn] = _("Jack");
  row = *(model->append());
  row[_textcolumn] = _("Alsa");
  combobox_audiodriver->pack_start(_textcolumn);

  model = Gtk::ListStore::create(column_model);
  combobox_savelocalaudio->set_model(model);
  row = *(model->append());
  row[_textcolumn] = _("None");
  row = *(model->append());
  row[_textcolumn] = _("Ogg Files");
  row = *(model->append());
  row[_textcolumn] = _("Ogg+Wave Files");
  row = *(model->append());
  row[_textcolumn] = _("Delete Ogg Files as soon as possible");
  combobox_savelocalaudio->pack_start(_textcolumn);

  model = Gtk::ListStore::create(column_model);
  combobox_autosubscription->set_model(model);
  row = *(model->append());
  row[_textcolumn] = _("Disable autosubscription");
  row = *(model->append());
  row[_textcolumn] = _("Enable autosubscription for all users");
  row = *(model->append());
  row[_textcolumn] = _("Enable autosubscription for all specified users");
  row = *(model->append());
  row[_textcolumn] = _("Disable autosubscription for all specified users");
  combobox_autosubscription->pack_start(_textcolumn);

  _refListStoreAutosubscription = Gtk::ListStore::create(column_model);
  treeview_autosubscription->set_model(_refListStoreAutosubscription);
  treeview_autosubscription->append_column("Username", _textcolumn);
}

void window_preferences::on_checkbutton_anonymous_toggled()
{
  if (checkbutton_anonymous->get_active()) {
    label23->hide();
    entry_password->hide();
  }
  else {
    label23->show();
    entry_password->show();
  }
}

void window_preferences::on_button_autosubscription_add_clicked()
{
  Gtk::Dialog dialog(_("Add user"));

  // Add response buttons the the dialog:
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
  Gtk::VBox *vbox = dialog.get_vbox();
  Gtk::Label label(_("Specify user to be added to the list."));
  vbox->pack_start(label);
  label.show();
  Gtk::Entry entry;
  vbox->pack_start(entry);
  entry.show();

  // Show the dialog and wait for a user response:
  int result = dialog.run();

  // Handle the response:
  switch(result)
  {
    case(Gtk::RESPONSE_OK):
    {
      Glib::ustring username = entry.get_text();
      bool exists = false;
      Gtk::TreeNodeChildren children = _refListStoreAutosubscription->children();
      for (Gtk::TreeIter iter = children.begin(), end(children.end()); iter != end; ++iter) {
	Gtk::TreeModel::Row row = *iter;
	Glib::ustring s = row[_textcolumn];
	if (s == username) {
	  exists = true;
	  break;
	}
      }
      if (!exists) {
	Gtk::TreeModel::Row row = *(_refListStoreAutosubscription->append());
	row[_textcolumn] = username;
      } else {
	Gtk::MessageDialog warning(_("User already in list."),
				   false /* use_markup */,
				   Gtk::MESSAGE_WARNING,
				   Gtk::BUTTONS_CLOSE);
	warning.set_secondary_text(_("The user name you specified is already existing in the list."));
	warning.run();

      }
      break;
    }
    default:
    {
      break;
    }
  }
}

void window_preferences::on_button_autosubscription_delete_clicked()
{
  Glib::RefPtr<Gtk::TreeSelection> selection = treeview_autosubscription->get_selection();
  if (selection->count_selected_rows() == 1) {
    Gtk::TreeIter iter = selection->get_selected();
    //Gtk::TreeModel::Row row = *iter;
    _refListStoreAutosubscription->erase(iter);
  }
}

void window_preferences::on_button_autosubscription_clear_clicked()
{
  _refListStoreAutosubscription->clear();
}

void window_preferences::on_button_abort_clicked()
{
  this->hide();
}

void window_preferences::on_button_apply_clicked()
{
  int audiodriver = combobox_audiodriver->get_active_row_number();
  Glib::ustring alsacfgstr = entry_alsa_configstring->get_text();
  Glib::ustring client_name = entry_jack_client_name->get_text();
  int nInputChannels = (int)spinbutton_jack_ninputchannels->get_value();
  int nOutputChannels = (int)spinbutton_jack_noutputchannels->get_value();
  bool audiooff = (g_audio == NULL);

  if (audiodriver == 0) {
    if ((audiodriver != _gconf_client->get_int(_gconf_dir+"/audio_driver"))||
	(client_name != _gconf_client->get_string(_gconf_dir+"/jack_client_name"))||
	(nInputChannels != _gconf_client->get_int(_gconf_dir+"/jack_ninput_channels"))||
	(nOutputChannels != _gconf_client->get_int(_gconf_dir+"/jack_noutput_channels"))||
	audiooff) {
      g_audio_enable = 0;
      if (g_audio != NULL)
	delete g_audio;
      if (client_name == "")
	client_name = "ninjam";
      if (nInputChannels == 0)
	nInputChannels = 4;
      if (nOutputChannels == 0)
	nOutputChannels = 2;
      g_audio = create_audioStreamer_JACK(client_name.c_str(),
					  nInputChannels,
					  nOutputChannels,
					  audiostream_onsamples,
					  g_client);
      window->update_inputLists();
      window->update_outputLists();
      if (g_audio) {
	g_audio_enable = 1;
      }
    }
  } else {
    if ((audiodriver != _gconf_client->get_int(_gconf_dir+"/audio_driver")) ||
	(alsacfgstr != _gconf_client->get_string(_gconf_dir+"/alsa_config_string"))||
	audiooff) {
      g_audio_enable = 0;
      if (g_audio)
	delete g_audio;
      char* cfgstr = new char[alsacfgstr.size()+1];
      memcpy(cfgstr, alsacfgstr.c_str(), alsacfgstr.size());
      g_audio = create_audioStreamer_ALSA(cfgstr,
					  audiostream_onsamples);
      delete cfgstr;
      window->update_inputLists();
      window->update_outputLists();
      if (g_audio) {
	g_audio_enable = 1;
      }
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
	printf(_("Error creating session directory\n"));
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
       (savelocalaudio > 0)) ||
      audiooff) {
    switch (savelocalaudio) {
    case 2:
      if (g_client->waveWrite != NULL) {
	delete g_client->waveWrite;
	g_client->waveWrite = NULL;
      }
      if (g_audio)
	g_client->waveWrite = new WaveWriter((sessiondir+"output.wav").c_str(),
					     24,
					     g_audio->getNOutputChannels()>1?2:1,
					     g_audio->getSampleRate());
    case 1:
      if (g_audio)
	g_client->SetOggOutFile(fopen((sessiondir+"output.ogg").c_str(),"ab"),
				g_audio->getSampleRate(),
				g_audio->getNOutputChannels()>1?2:1,
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

  g_client->config_autosubscribe = combobox_autosubscription->get_active_row_number();
  printf("%d\n", g_client->config_autosubscribe);
  g_client->config_autosubscribe_userlist.clear();
  std::list<Glib::ustring> autosubscribe_userlist;
  Gtk::TreeNodeChildren children = _refListStoreAutosubscription->children();
  for (Gtk::TreeIter iter = children.begin(), end(children.end()); iter != end; ++iter) {
    Gtk::TreeModel::Row row = *iter;
    Glib::ustring s = row[_textcolumn];
    autosubscribe_userlist.push_back(s);
    g_client->config_autosubscribe_userlist.insert(s);
  }
  _gconf_client->set_string_list(_gconf_dir+"/autosubscribe_userlist",
				 autosubscribe_userlist);
  _gconf_client->set(_gconf_dir+"/audio_driver",
		     audiodriver);
  _gconf_client->set(_gconf_dir+"/alsa_config_string",
		     alsacfgstr);
  _gconf_client->set(_gconf_dir+"/session_directory",
		     session_dir);
  _gconf_client->set(_gconf_dir+"/savelocalaudio",
		     g_client->config_savelocalaudio);
  _gconf_client->set(_gconf_dir+"/ogg_bitrate",
		     oggbitrate);
  _gconf_client->set(_gconf_dir+"/save_log",
		     savelog);
  _gconf_client->set(_gconf_dir+"/jack_client_name",
		     client_name);
  _gconf_client->set(_gconf_dir+"/jack_ninput_channels",
		     nInputChannels);
  _gconf_client->set(_gconf_dir+"/jack_noutput_channels",
		     nOutputChannels);
  _gconf_client->set(_gconf_dir+"/hostname",
		     entry_hostname->get_text());
  _gconf_client->set(_gconf_dir+"/username",
		     entry_username->get_text());
  _gconf_client->set(_gconf_dir+"/anonymous_login",
		     checkbutton_anonymous->get_active());
  _gconf_client->set(_gconf_dir+"/password",
		     entry_password->get_text());
  _gconf_client->set(_gconf_dir+"/autosubscribe",
		     g_client->config_autosubscribe);
}

void window_preferences::on_button_ok_clicked()
{
  on_button_apply_clicked();
  this->hide();
}

void window_preferences::on_window_preferences_show()
{
  combobox_audiodriver->set_active(_gconf_client->get_int(_gconf_dir+"/audio_driver"));
  entry_alsa_configstring->set_text(_gconf_client->get_string(_gconf_dir+"/alsa_config_string"));
  entry_sessiondir->set_text(_gconf_client->get_string(_gconf_dir+"/session_directory"));
  if (g_client->config_savelocalaudio >= 0) {
    combobox_savelocalaudio->set_active(g_client->config_savelocalaudio);
  } else {
    combobox_savelocalaudio->set_active(3);
  }
  spinbutton_oggbitrate->set_value(_gconf_client->get_int(_gconf_dir+"/ogg_bitrate"));
  checkbutton_savelog->set_active(_gconf_client->get_bool(_gconf_dir+"/save_log"));
  entry_jack_client_name->set_text(_gconf_client->get_string(_gconf_dir+"/jack_client_name"));
  if (g_audio) {
    spinbutton_jack_ninputchannels->set_value(g_audio->getNInputChannels());
    spinbutton_jack_noutputchannels->set_value(g_audio->getNOutputChannels());
  } else {
    spinbutton_jack_ninputchannels->set_value(_gconf_client->get_int(_gconf_dir+"/jack_ninput_channels"));
    spinbutton_jack_noutputchannels->set_value(_gconf_client->get_int(_gconf_dir+"/jack_noutput_channels"));
  }
  entry_hostname->set_text(_gconf_client->get_string(_gconf_dir+"/hostname"));
  entry_username->set_text(_gconf_client->get_string(_gconf_dir+"/username"));
  checkbutton_anonymous->set_active(_gconf_client->get_bool(_gconf_dir+"/anonymous_login"));
  entry_password->set_text(_gconf_client->get_string(_gconf_dir+"/password"));
  combobox_autosubscription->set_active(g_client->config_autosubscribe);
  _refListStoreAutosubscription->clear();
  for (std::set<std::string>::iterator iter = g_client->config_autosubscribe_userlist.begin();
       iter != g_client->config_autosubscribe_userlist.end(); ++iter) {
    Gtk::TreeModel::Row row = *(_refListStoreAutosubscription->append());
    row[_textcolumn] = *iter;
  }
}
