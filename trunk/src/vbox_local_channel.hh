// generated 2006/11/15 21:05:02 CET by tobias@THINKPAD-T43.(none)
// using glademm V2.6.0
//
// newer (non customized) versions of this file go to vbox_local_channel.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _VBOX_LOCAL_CHANNEL_HH
#  include "vbox_local_channel_glade.hh"
#  define _VBOX_LOCAL_CHANNEL_HH
class vbox_local_channel : public vbox_local_channel_glade
{  
public:
  vbox_local_channel(GlademmData *gmm_data);
private:
  void on_entry_local_channelname_activate();
  void on_checkbutton_local_transmit_toggled();
  void on_combobox_local_input_changed();
  void on_hscale_local_volume_value_changed();
  void on_hscale_local_pan_value_changed();
  void on_checkbutton_local_mute_toggled();
  void on_checkbutton_local_solo_toggled();
  void on_button_local_remove_clicked();
};
#endif
