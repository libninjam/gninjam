// generated 2006/11/15 21:05:02 CET by tobias@THINKPAD-T43.(none)
// using glademm V2.6.0
//
// newer (non customized) versions of this file go to vbox_local_channel.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "vbox_local_channel.hh"

#include "common.hh"

vbox_local_channel::vbox_local_channel(GlademmData *gmm_data)
 : vbox_local_channel_glade(gmm_data)
{
  hscale_local_pan->signal_format_value().connect(sigc::ptr_fun(on_hscale_pan_format_value), false);
  hscale_local_volume->signal_format_value().connect(sigc::ptr_fun(on_hscale_volume_format_value), false);
}

void vbox_local_channel::on_entry_local_channelname_activate()
{  
}

void vbox_local_channel::on_checkbutton_local_transmit_toggled()
{  
}

void vbox_local_channel::on_combobox_local_input_changed()
{  
}

void vbox_local_channel::on_hscale_local_volume_value_changed()
{  
}

void vbox_local_channel::on_hscale_local_pan_value_changed()
{  
}

void vbox_local_channel::on_checkbutton_local_mute_toggled()
{  
}

void vbox_local_channel::on_checkbutton_local_solo_toggled()
{  
}

void vbox_local_channel::on_button_local_remove_clicked()
{  
}
