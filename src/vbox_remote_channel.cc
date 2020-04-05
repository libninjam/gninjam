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
/*
 * Standard gettext macros.
 */
#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (GETTEXT_PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif

#include "vbox_remote_channel.hh"
#include "gNinjamClient.hh"

#include "common.hh"

#include <gtkmm/liststore.h>

#include <ninjam/njmisc.h>
#include <ninjam/audiostream.h>
#include <ninjam/njclient.h>

extern audioStreamer *g_audio;
extern NJClient *g_client;
extern gNinjamClient *window;

vbox_remote_channel::vbox_remote_channel(GlademmData *gmm_data)
  : vbox_remote_channel_glade(gmm_data)
{
  hscale_remote_pan->signal_format_value().connect(sigc::ptr_fun(on_hscale_pan_format_value), false);
  hscale_remote_volume->signal_format_value().connect(sigc::ptr_fun(on_hscale_volume_format_value), false);
  _column_model.add(_textcolumn);
  combobox_remote_output->pack_start(_textcolumn);
}

void vbox_remote_channel::update_VUmeter()
{
  if (_useridx != -1) {
    float peak = VAL2DB(g_client->GetUserChannelPeak(_useridx, _channelidx));
    progressbar_remote->set_fraction((peak+120)/140);
    char output[11];
    snprintf(output, sizeof(output), "%.2f dB", peak);
    progressbar_remote->set_text(output);
  }
}

void vbox_remote_channel::update_outputList()
{
  int active = combobox_remote_output->get_active_row_number();
  Glib::RefPtr<Gtk::ListStore> model = Gtk::ListStore::create(_column_model);
  Gtk::TreeModel::Row row;
  if (g_audio) {
    for (int i=0; i < g_audio->getNOutputChannels(); i++) {
      row = *(model->append());
      row[_textcolumn] = g_audio->GetOutputChannelName(i);
    }
  }
  row = *(model->append());
  row[_textcolumn] = _("New channel");
  combobox_remote_output->set_model(model);
  combobox_remote_output->set_active(active);
}

void vbox_remote_channel::set_data(int useridx, int channelidx)
{
  _useridx = useridx;
  _channelidx = channelidx;

  float vol, pan;
  bool sub, mute, solo, stereoout;
  int outch;
  char *name = g_client->GetUserChannelState(useridx, channelidx, &sub, &vol, &pan, &mute, &solo, &outch, &stereoout);
  entry_remote_channelname->set_text(name);
  update_outputList();
  combobox_remote_output->set_active(outch);
  checkbutton_remote_receive->set_active(sub);
  hscale_remote_volume->set_value(VAL2DB(vol));
  hscale_remote_pan->set_value(pan);
  checkbutton_remote_mute->set_active(mute);
  checkbutton_remote_solo->set_active(solo);
  checkbutton_remote_stereo->set_active(stereoout);
}

void vbox_remote_channel::on_checkbutton_remote_receive_toggled()
{
  g_client->SetUserChannelState(_useridx, _channelidx,
				true, checkbutton_remote_receive->get_active(), // sub
				false, 0.0f, // vol
				false, 0.0f, // pan
				false, false, // mute
				false, false, // solo
				false, 0, // outch
				false, false); // stereoout
}

void vbox_remote_channel::on_hscale_remote_volume_value_changed()
{  
  g_client->SetUserChannelState(_useridx, _channelidx,
				false, false, // sub
				true, DB2VAL(hscale_remote_volume->get_value()), // vol
				false, 0.0f, // pan
				false, false, // mute
				false, false, // solo
				false, 0, // outch
				false, false); // stereoout
}

void vbox_remote_channel::on_hscale_remote_pan_value_changed()
{
  g_client->SetUserChannelState(_useridx, _channelidx,
				false, false, // sub
				false, 0.0f, // vol
				true, hscale_remote_pan->get_value(), // pan
				false, false, // mute
				false, false, // solo
				false, 0, // outch
				false, false); // stereoout
}

void vbox_remote_channel::on_checkbutton_remote_mute_toggled()
{
  g_client->SetUserChannelState(_useridx, _channelidx,
				false, false, // sub
				false, 0.0f, // vol
				false, 0.0f, // pan
				true, checkbutton_remote_mute->get_active(), // mute
				false, false, // solo
				false, 0, // outch
				false, false); // stereoout
}

void vbox_remote_channel::on_checkbutton_remote_solo_toggled()
{
  g_client->SetUserChannelState(_useridx, _channelidx,
				false, false, // sub
				false, 0.0f, // vol
				false, 0.0f, // pan
				false, false, // mute
				true, checkbutton_remote_solo->get_active(), // solo
				false, 0, // outch
				false, false); // stereoout
}

void vbox_remote_channel::on_combobox_remote_output_changed()
{
  int channel = combobox_remote_output->get_active_row_number();
  if (g_audio && (channel == g_audio->getNOutputChannels())) {
    if (g_audio->addOutputChannel())
      window->update_outputLists();
  }
  g_client->SetUserChannelState(_useridx, _channelidx,
				false, false, // sub
				false, 0.0f, // vol
				false, 0.0f, // pan
				false, false, // mute
				false, false, // solo
				true, channel, // outch
				false, false); // stereoout
}

void vbox_remote_channel::on_checkbutton_remote_stereo_toggled()
{
  g_client->SetUserChannelState(_useridx, _channelidx,
				false, false, // sub
				false, 0.0f, // vol
				false, 0.0f, // pan
				false, false, // mute
				false, false, // solo
				false, 0, // outch
				true, checkbutton_remote_stereo->get_active()); // stereoout
}
