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
#include "vbox_local.hh"
#include "vbox_local_channel.hh"

#include <list>

#include <ninjam/njclient.h>

extern NJClient *g_client;

vbox_local::vbox_local(GlademmData *gmm_data)
  : vbox_local_glade(gmm_data)
{  
}

void vbox_local::add_channel(int idx)
{
  vbox_local_channel* channel = Gtk::manage(new class vbox_local_channel(NULL));
  channel->init(idx);
  pack_start(*channel, Gtk::PACK_SHRINK, 0);
}

void vbox_local::update()
{
  std::list<Widget*> channels = get_children();
  std::list<Widget*>::iterator iter = channels.begin();
  int iChan;
  int iEnum;
  for (iChan = 0; (iEnum = g_client->EnumLocalChannels(iChan)) >= 0; iChan++) {
    if (iter == channels.end()) {
      add_channel(iEnum);
    } else {
      vbox_local_channel* channel = (vbox_local_channel*)*iter++;
      channel->init(iEnum);
    }
  }
  while (iter != channels.end()) {
    remove(*(*iter++));
  }
}

void vbox_local::update_VUmeters()
{
  std::list<Widget*> channels = get_children();
  std::list<Widget*>::iterator iter = channels.begin();
  while (iter != channels.end()) {
    vbox_local_channel* channel = (vbox_local_channel*)*iter++;
    channel->update_VUmeter();
  }
}

void vbox_local::update_inputLists()
{
  std::list<Widget*> channels = get_children();
  std::list<Widget*>::iterator iter = channels.begin();
  while (iter != channels.end()) {
    vbox_local_channel* channel = (vbox_local_channel*)*iter++;
    channel->update_inputList();
  }
}
