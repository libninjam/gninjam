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

#ifndef _VBOX_REMOTE_CHANNEL_HH
#  include "vbox_remote_channel_glade.hh"
#  define _VBOX_REMOTE_CHANNEL_HH
class vbox_remote_channel : public vbox_remote_channel_glade
{  
public:
  vbox_remote_channel(GlademmData *gmm_data);
  void set_data(int useridx, int channelidx);
  void update_VUmeter();
  void update_outputList();
private:   
  void on_checkbutton_remote_receive_toggled();
  void on_hscale_remote_volume_value_changed();
  void on_hscale_remote_pan_value_changed();
  void on_checkbutton_remote_mute_toggled();
  void on_checkbutton_remote_solo_toggled();
  void on_combobox_remote_output_changed();
  void on_checkbutton_remote_stereo_toggled();

  int _useridx;
  int _channelidx;
  Gtk::TreeModel::ColumnRecord _column_model;
  Gtk::TreeModelColumn<Glib::ustring> _textcolumn;
};
#endif
