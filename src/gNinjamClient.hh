// generated 2006/11/13 18:15:07 CET by tobias@THINKPAD-T43.(none)
// using glademm V2.6.0
//
// newer (non customized) versions of this file go to gNinjamClient.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _GNINJAMCLIENT_HH
#  include "gNinjamClient_glade.hh"
#  define _GNINJAMCLIENT_HH

#include "window_preferences.hh"
#include "dialog_connect.hh"

#include <ninjam/njclient.h>

class gNinjamClient : public gNinjamClient_glade
{
public:
  gNinjamClient(NJClient *client);
  ~gNinjamClient();
  void addChatText(Glib::ustring text);
  void setChatTopic(Glib::ustring text);
private:
  bool on_timeout();
  void on_verbinden1_activate();
  void on_trennen1_activate();
  void on_beenden1_activate();
  void on_ausschneiden1_activate();
  void on_kopieren1_activate();
  void on_einf__gen1_activate();
  void on_l__schen1_activate();
  void on_info1_activate();
  void on_hscale_master_volume_value_changed();
  Glib::ustring on_hscale_volume_format_value(double value);
  Glib::ustring on_hscale_pan_format_value(double value);
  void on_hscale_metronome_volume_value_changed();
  void on_hscale_metronome_pan_value_changed();
  void on_checkbutton_master_mute_toggled();
  void on_checkbutton_metronome_mute_toggled();
  void on_hscale_master_pan_value_changed();
  void on_button_send_clicked();

  dialog_connect *d_connect;
  window_preferences *w_preferences;
  NJClient *_client;
};
#endif
