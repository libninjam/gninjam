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

#ifndef _REMOTEUSERFRAME_HH
#  include "RemoteUserFrame_glade.hh"
#  define _REMOTEUSERFRAME_HH
class RemoteUserFrame : public RemoteUserFrame_glade
{  
public:
  RemoteUserFrame(GlademmData *gmm_data);
  void update(int useridx);
  void update_VUmeters();
  void update_outputLists();
private:
  void on_hscale_volume_value_changed();
  void on_hscale_pan_value_changed();
  void on_checkbutton_mute_toggled();
  int _useridx;
};
#endif
