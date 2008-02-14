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

#ifndef _GNINJAMCLIENT_HH
#  include "gNinjamClient_glade.hh"
#  define _GNINJAMCLIENT_HH

#include "window_preferences.hh"
#include "dialog_connect.hh"
#include <gtkmm/aboutdialog.h>

#include <vector>

class gNinjamClient : public gNinjamClient_glade
{
public:
  gNinjamClient();
  ~gNinjamClient();
  void addChatText(Glib::ustring text);
  void setChatTopic(Glib::ustring text);
  void update_inputLists();
  void update_outputLists();
private:
  bool on_timeout();
  bool on_timeout_gui();
  void on_neu1_activate();
  void on_verbinden1_activate();
  void on_trennen1_activate();
  void on_beenden1_activate();
  void on_ausschneiden1_activate();
  void on_kopieren1_activate();
  void on_einf__gen1_activate();
  void on_l__schen1_activate();
  void on_einstellungen1_activate();
  void on_info1_activate();
  void on_hscale_master_volume_value_changed();
  void on_hscale_metronome_volume_value_changed();
  void on_hscale_metronome_pan_value_changed();
  void on_checkbutton_master_mute_toggled();
  void on_checkbutton_metronome_mute_toggled();
  void on_hscale_master_pan_value_changed();
  void on_chat_entry_activate();
  void on_local_channels1_activate();
  void on_remote_channels1_activate();
  void on_chat1_activate();
  void on_combobox_metronome_output_changed();
  void on_checkbutton_metronome_stereo_toggled();
  void connect_gui_status_update(unsigned timeout);

  dialog_connect *d_connect;
  window_preferences *w_preferences;
  int _old_status;
  bool _audio_status_changed;
  sigc::connection _gui_connection;
  Gtk::TreeModel::ColumnRecord _column_model;
  Gtk::TreeModelColumn<Glib::ustring> _textcolumn;
  Gtk::Adjustment _dummy_adjustment;

  Gtk::AboutDialog _about_dialog;
};
#endif
