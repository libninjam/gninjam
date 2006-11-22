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
#include "RemoteUserFrame.hh"
#include <list>

#include <ninjam/njclient.h>

extern NJClient *g_client;

vbox_remote::vbox_remote(GlademmData *gmm_data)
  : vbox_remote_glade(gmm_data)
{  
}

void vbox_remote::update()
{
  std::list<Widget*> users = get_children();
  std::list<Widget*>::iterator iter = users.begin();
  for (int iUser=0; iUser < g_client->GetNumUsers(); iUser++) {
    if (iter == users.end()) {
      RemoteUserFrame* user = Gtk::manage(new class RemoteUserFrame(NULL));
      user->update(iUser);
      pack_start(*user);
    } else {
      RemoteUserFrame* user = (RemoteUserFrame*)*iter++;
      user->update(iUser);
    }
  }
  while (iter != users.end()) {
    remove(*(*iter++));
  }
}

void vbox_remote::update_VUmeters()
{
  std::list<Widget*> users = get_children();
  std::list<Widget*>::iterator iter = users.begin();
  while (iter != users.end()) {
    RemoteUserFrame* user = (RemoteUserFrame*)*iter++;
    user->update_VUmeters();
  }
}

void vbox_remote::update_outputLists()
{
  std::list<Widget*> users = get_children();
  std::list<Widget*>::iterator iter = users.begin();
  while (iter != users.end()) {
    RemoteUserFrame* user = (RemoteUserFrame*)*iter++;
    user->update_outputLists();
  }
}
