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

#ifndef _VBOX_LOCAL_CHANNEL_HH
#  include "vbox_local_channel_glade.hh"
#  define _VBOX_LOCAL_CHANNEL_HH
class vbox_local_channel : public vbox_local_channel_glade
{  
public:
  vbox_local_channel(GlademmData *gmm_data);
  void init(int idx);
  void update_VUmeter();
  void update_inputList();
private:
  void on_entry_local_channelname_changed();
  void on_checkbutton_local_transmit_toggled();
  void on_combobox_local_input_changed();
  void on_hscale_local_volume_value_changed();
  void on_hscale_local_pan_value_changed();
  void on_spinbutton_bitrate_value_changed();
  void on_checkbutton_local_mute_toggled();
  void on_checkbutton_local_solo_toggled();
  void on_button_local_remove_clicked();
  int _idx;
  Gtk::TreeModel::ColumnRecord _column_model;
  Gtk::TreeModelColumn<Glib::ustring> _textcolumn;
};
#endif
