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

#include "common.hh"

#include <ninjam/njclient.h>
#include <ninjam/njmisc.h>

extern NJClient *g_client;

RemoteUserFrame::RemoteUserFrame(GlademmData *gmm_data)
  : RemoteUserFrame_glade(gmm_data)
{
  hscale_pan->signal_format_value().connect(sigc::ptr_fun(on_hscale_pan_format_value), false);
  hscale_volume->signal_format_value().connect(sigc::ptr_fun(on_hscale_volume_format_value), false);
}

void RemoteUserFrame::update(int useridx)
{
  _useridx = useridx;
  float vol, pan;
  bool mute;
  label->set_text(g_client->GetUserState(useridx, &vol, &pan, &mute));
  hscale_volume->set_value(VAL2DB(vol));
  hscale_pan->set_value(pan);
  checkbutton_mute->set_active(mute);

  std::list<Widget*> channels = vbox->get_children();
  std::list<Widget*>::iterator iter = channels.begin();
  int iEnum;
  for (int iUserChan=0; (iEnum = g_client->EnumUserChannels(useridx, iUserChan)) >= 0; iUserChan++) {
    if (iter == channels.end()) {
      vbox_remote_channel* channel = Gtk::manage(new class vbox_remote_channel(NULL));
      channel->set_data(useridx, iEnum);
      vbox->pack_start(*channel, Gtk::PACK_SHRINK, 0);
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

void RemoteUserFrame::update_sessionState()
{
  time_t lastupdatetime;
  double lastsessionpos, maxlen;
  lastsessionpos = g_client->GetUserSessionPos(_useridx, &lastupdatetime, &maxlen);
  if (maxlen != -1.0) {
    char str_last[9];
    if (lastsessionpos != -1.0) {
      unsigned ulastsessionpos = (unsigned)round(lastsessionpos);
      unsigned sec_last, min_last, hour_last;
      sec_last = ulastsessionpos % 60;
      min_last = (ulastsessionpos / 60) % 60;
      hour_last = (ulastsessionpos / 3600) % 3600;
      snprintf(str_last, sizeof(str_last),
	       "%0.2u:%0.2u:%0.2u",
	       hour_last, min_last, sec_last);
    } else {
      snprintf(str_last, sizeof(str_last), "--:--:--");
    }
    char sessionstate[18];
    unsigned umaxlen = (unsigned)round(maxlen);
    unsigned sec_max, min_max, hour_max;
    sec_max = umaxlen % 60;
    min_max = (umaxlen / 60) % 60;
    hour_max = (umaxlen / 3600) % 3600;
    snprintf(sessionstate, sizeof(sessionstate),
	     "%s/%0.2u:%0.2u:%0.2u", str_last,
	     hour_max, min_max, sec_max);
    entry_sessionstate->set_text(sessionstate);
  } else {
    entry_sessionstate->set_text("");
  }
}

void RemoteUserFrame::on_hscale_volume_value_changed()
{
  g_client->SetUserState(_useridx,
			 true, DB2VAL(hscale_volume->get_value()), // vol
			 false, 0.0f, // pan
			 false, false); // mute
}

void RemoteUserFrame::on_hscale_pan_value_changed()
{
  g_client->SetUserState(_useridx,
			 false, 0.0f, // vol
			 true, hscale_pan->get_value(), // pan
			 false, false); // mute
}

void RemoteUserFrame::on_checkbutton_mute_toggled()
{
  g_client->SetUserState(_useridx,
			 false, 0.0f, // vol
			 false, 0.0f, // pan
			 true, checkbutton_mute->get_active()); // mute
}
