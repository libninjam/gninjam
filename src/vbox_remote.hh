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

#ifndef _VBOX_REMOTE_HH
#  include "vbox_remote_glade.hh"
#  define _VBOX_REMOTE_HH

class vbox_remote : public vbox_remote_glade
{  
public:
  vbox_remote(GlademmData *gmm_data);
  void update();
  void update_VUmeters();
  void update_outputLists();
};
#endif
