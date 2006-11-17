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
#include "vbox_remote.hh"
#include "vbox_remote_channel.hh"
#include <list>

#include <ninjam/njclient.h>

extern NJClient *g_client;

vbox_remote::vbox_remote(GlademmData *gmm_data)
  : vbox_remote_glade(gmm_data)
{  
}

void vbox_remote::update()
{
  std::list<Widget*> channels = get_children();
  std::list<Widget*>::iterator iter = channels.begin();
  int iChan = 0;
  for (int iUser=0; iUser < g_client->GetNumUsers(); iUser++) {
    int iEnum;
    for (int iUserChan=0; (iEnum = g_client->EnumUserChannels(iUser, iUserChan)) >= 0; iUserChan++, iChan++) {
      if (iter == channels.end()) {
	vbox_remote_channel* channel = Gtk::manage(new class vbox_remote_channel(NULL));
	channel->set_data(iUser, iEnum);
	pack_start(*channel);
      } else {
	vbox_remote_channel* channel = (vbox_remote_channel*)*iter++;
	channel->set_data(iUser, iEnum);
      }
    }
  }
  while (iter != channels.end()) {
    remove(*(*iter++));
  }
}
