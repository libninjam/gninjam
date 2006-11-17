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
#include "vbox_remote_channel.hh"

#include "common.hh"

#include <ninjam/njmisc.h>
#include <ninjam/njclient.h>

extern NJClient *g_client;

vbox_remote_channel::vbox_remote_channel(GlademmData *gmm_data)
  : vbox_remote_channel_glade(gmm_data)
{
  hscale_remote_pan->signal_format_value().connect(sigc::ptr_fun(on_hscale_pan_format_value), false);
  hscale_remote_volume->signal_format_value().connect(sigc::ptr_fun(on_hscale_volume_format_value), false);
}

void vbox_remote_channel::set_data(int useridx, int channelidx)
{
  _useridx = useridx;
  _channelidx = channelidx;
  char *user = g_client->GetUserState(useridx);
  entry_remote_user->set_text(user);

  float vol, pan;
  bool sub, mute;
  char *name = g_client->GetUserChannelState(useridx, channelidx, &sub, &vol, &pan, &mute);
  entry_remote_channelname->set_text(name);
  checkbutton_remote_receive->set_active(sub);
  hscale_remote_volume->set_value(VAL2DB(vol));
  hscale_remote_pan->set_value(pan);
  checkbutton_remote_mute->set_active(mute);

  float peak = VAL2DB(g_client->GetUserChannelPeak(useridx, channelidx));
  progressbar_remote->set_fraction((peak+120)/140);
  char output[11];
  snprintf(output, sizeof(output), "%.2f dB", peak);
  progressbar_remote->set_text(output);
}

void vbox_remote_channel::on_checkbutton_remote_receive_toggled()
{
  g_client->SetUserChannelState(_useridx, _channelidx,
				true, checkbutton_remote_receive->get_active(), // sub
				false, 0.0f, // vol
				false, 0.0f, // pan
				false, false, // mute
				false, false); // solo
}

void vbox_remote_channel::on_hscale_remote_volume_value_changed()
{  
  g_client->SetUserChannelState(_useridx, _channelidx,
				false, false, // sub
				true, DB2VAL(hscale_remote_volume->get_value()), // vol
				false, 0.0f, // pan
				false, false, // mute
				false, false); // solo
}

void vbox_remote_channel::on_hscale_remote_pan_value_changed()
{
  g_client->SetUserChannelState(_useridx, _channelidx,
				false, false, // sub
				false, 0.0f, // vol
				true, hscale_remote_pan->get_value(), // pan
				false, false, // mute
				false, false); // solo
}

void vbox_remote_channel::on_checkbutton_remote_mute_toggled()
{
  g_client->SetUserChannelState(_useridx, _channelidx,
				false, false, // sub
				false, 0.0f, // vol
				false, 0.0f, // pan
				true, checkbutton_remote_mute->get_active(), // mute
				false, false); // solo
}

void vbox_remote_channel::on_checkbutton_remote_solo_toggled()
{
  g_client->SetUserChannelState(_useridx, _channelidx,
				false, false, // sub
				false, 0.0f, // vol
				false, 0.0f, // pan
				false, false, // mute
				true, checkbutton_remote_solo->get_active()); // solo
}
