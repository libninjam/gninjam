// generated 2006/11/15 21:05:02 CET by tobias@THINKPAD-T43.(none)
// using glademm V2.6.0
//
// newer (non customized) versions of this file go to vbox_remote.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "vbox_remote.hh"
#include "vbox_remote_channel.hh"
#include <vector>

#include <ninjam/njclient.h>

extern NJClient *g_client;

vbox_remote::vbox_remote(GlademmData *gmm_data)
  : vbox_remote_glade(gmm_data)
{  
}

void vbox_remote::update()
{
  std::vector<Widget*> channels = get_children();
  int iChan = 0;
  for (int iUser=0; iUser < g_client->GetNumUsers(); iUser++) {
    int iEnum;
    for (int iUserChan=0; (iEnum = g_client->EnumUserChannels(iUser, iUserChan)) >= 0; iUserChan++, iChan++) {
      if (channels.size() <= iChan) {
	vbox_remote_channel* channel = Gtk::manage(new class vbox_remote_channel(NULL));
	channel->set_data(iUser, iEnum);
	channel->show();
	pack_start(*channel);
      } else {
	vbox_remote_channel* channel = (vbox_remote_channel*)channels[iChan];
	channel->set_data(iUser, iEnum);
      }
    }
  }
  for (int chSize = channels.size(); iChan < chSize; iChan++) {
    remove(*channels[iChan]);
  }
}
