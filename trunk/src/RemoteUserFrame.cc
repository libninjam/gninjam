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
#include "RemoteUserFrame.hh"
#include "vbox_remote_channel.hh"

#include <ninjam/njclient.h>

extern NJClient *g_client;

RemoteUserFrame::RemoteUserFrame(GlademmData *gmm_data)
  : RemoteUserFrame_glade(gmm_data)
{
}

void RemoteUserFrame::update(int useridx)
{
  label->set_text(g_client->GetUserState(useridx));

  std::list<Widget*> channels = vbox->get_children();
  std::list<Widget*>::iterator iter = channels.begin();
  int iEnum;
  for (int iUserChan=0; (iEnum = g_client->EnumUserChannels(useridx, iUserChan)) >= 0; iUserChan++) {
    if (iter == channels.end()) {
      vbox_remote_channel* channel = Gtk::manage(new class vbox_remote_channel(NULL));
      channel->set_data(useridx, iEnum);
      vbox->pack_start(*channel);
    } else {
      vbox_remote_channel* channel = (vbox_remote_channel*)*iter++;
      channel->set_data(useridx, iEnum);
    }
  }
  while (iter != channels.end()) {
    vbox->remove(*(*iter++));
  }
}

void RemoteUserFrame::update_VUmeters()
{
  std::list<Widget*> channels = vbox->get_children();
  std::list<Widget*>::iterator iter = channels.begin();
  while (iter != channels.end()) {
    vbox_remote_channel* channel = (vbox_remote_channel*)*iter++;
    channel->update_VUmeter();
  }
}

void RemoteUserFrame::update_outputLists()
{
  std::list<Widget*> channels = vbox->get_children();
  std::list<Widget*>::iterator iter = channels.begin();
  while (iter != channels.end()) {
    vbox_remote_channel* channel = (vbox_remote_channel*)*iter++;
    channel->update_outputList();
  }
}
