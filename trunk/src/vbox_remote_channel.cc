// generated 2006/11/15 21:05:02 CET by tobias@THINKPAD-T43.(none)
// using glademm V2.6.0
//
// newer (non customized) versions of this file go to vbox_remote_channel.cc_new

// This file is for your program, I won't touch it again!

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
