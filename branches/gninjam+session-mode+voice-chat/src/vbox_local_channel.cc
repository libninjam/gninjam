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

#include "vbox_local_channel.hh"
#include "gNinjamClient.hh"

#include "common.hh"

#include <gtkmm/liststore.h>

#include <ninjam/njmisc.h>
#include <ninjam/audiostream.h>
#include <ninjam/njclient.h>

extern audioStreamer *g_audio;
extern NJClient *g_client;
extern gNinjamClient *window;

vbox_local_channel::vbox_local_channel(GlademmData *gmm_data)
  : vbox_local_channel_glade(gmm_data),
    _idx(-1)
{
  hscale_local_pan->signal_format_value().connect(sigc::ptr_fun(on_hscale_pan_format_value), false);
  hscale_local_volume->signal_format_value().connect(sigc::ptr_fun(on_hscale_volume_format_value), false);
  _column_model.add(_textcolumn);
  combobox_local_input->pack_start(_textcolumn);
  combobox_local_output->pack_start(_textcolumn);

  Gtk::TreeModel::ColumnRecord column_model;
  Gtk::TreeModelColumn<Glib::ustring> textcolumn;
  column_model.add(textcolumn);
  Glib::RefPtr<Gtk::ListStore> model = Gtk::ListStore::create(column_model);
  combobox_local_mode->set_model(model);
  Gtk::TreeModel::Row row = *(model->append());
  row[textcolumn] = _("Normal NINJAM");
  row = *(model->append());
  row[textcolumn] = _("Voice Chat");
  row = *(model->append());
  row[textcolumn] = _("Session Mode");
  combobox_local_mode->pack_start(textcolumn);
}

void vbox_local_channel::update_VUmeter()
{
  if (_idx != -1) {
    float value = VAL2DB(g_client->GetLocalChannelPeak(_idx));
    progressbar_local->set_fraction((value+120)/140);
    char output[11];
    snprintf(output, sizeof(output), "%.2f dB", value);
    progressbar_local->set_text(output);
  }
}

void vbox_local_channel::update_inputList()
{
  int active = combobox_local_input->get_active_row_number();
  Glib::RefPtr<Gtk::ListStore> model = Gtk::ListStore::create(_column_model);
  const char* sourcename;
  Gtk::TreeModel::Row row;
  if (g_audio) {
    for (int i=0; (sourcename = g_audio->GetChannelName(i)) != NULL; i++) {
      row = *(model->append());
      row[_textcolumn] = sourcename;
    }
  }
  row = *(model->append());
  row[_textcolumn] = _("New channel");
  combobox_local_input->set_model(model);
  combobox_local_input->set_active(active);
}

void vbox_local_channel::update_outputList()
{
  int active = combobox_local_output->get_active_row_number();
  Glib::RefPtr<Gtk::ListStore> model = Gtk::ListStore::create(_column_model);
  Gtk::TreeModel::Row row;
  if (g_audio) {
    for (int i=0; i < g_audio->m_outnch; i++) {
      row = *(model->append());
      row[_textcolumn] = g_audio->GetOutputChannelName(i);
    }
  }
  row = *(model->append());
  row[_textcolumn] = _("New channel");
  combobox_local_output->set_model(model);
  combobox_local_output->set_active(active);
}

void vbox_local_channel::init(int idx)
{
  _idx = idx;
  int sourcechannel, bitrate, outch, mode;
  bool broadcast;
  char *channelname = g_client->GetLocalChannelInfo(_idx,
						    &sourcechannel,
						    &bitrate,
						    &broadcast,
						    &outch,
						    &mode);
  entry_local_channelname->set_text(channelname);
  update_inputList();
  combobox_local_input->set_active(sourcechannel&1023);
  checkbutton_local_stereo_in->set_active(!!(sourcechannel&1024));
  checkbutton_local_transmit->set_active(broadcast);
  float volume, pan;
  bool mute, solo;
  g_client->GetLocalChannelMonitoring(_idx, &volume, &pan, &mute, &solo);
  hscale_local_volume->set_value(VAL2DB(volume));
  hscale_local_pan->set_value(pan);
  update_outputList();
  combobox_local_output->set_active(outch&1023);
  checkbutton_local_stereo_out->set_active(!(outch&1024));
  spinbutton_bitrate->set_value(bitrate);
  checkbutton_local_mute->set_active(mute);
  checkbutton_local_solo->set_active(solo);
  combobox_local_mode->set_active(mode>>1);
}

void vbox_local_channel::on_entry_local_channelname_changed()
{  
  g_client->SetLocalChannelInfo(_idx,
				entry_local_channelname->get_text().c_str(), // name
				false, 0, // src
				false, 0, // bitrate
				false, false, // broadcast
				false, 0, // outch
				false, 0); // mode
  g_client->NotifyServerOfChannelChange();
}

void vbox_local_channel::on_checkbutton_local_transmit_toggled()
{
  g_client->SetLocalChannelInfo(_idx,
				NULL, // name
				false, 0, // src
				false, 0, // bitrate
				true, checkbutton_local_transmit->get_active(), // broadcast
				false, 0, // outch
				false, 0); // mode
  g_client->NotifyServerOfChannelChange();
}

void vbox_local_channel::on_combobox_local_input_changed()
{
  int channel = combobox_local_input->get_active_row_number();
  if (g_audio && (channel == g_audio->m_innch)) {
    if (g_audio->addInputChannel())
      window->update_inputLists();
  }
  channel &= 1023;
  if (checkbutton_local_stereo_in->get_active())
    channel |= 1024;
  g_client->SetLocalChannelInfo(_idx,
				NULL, // name
				true, channel, // src
				false, 0, // bitrate
				false, false, // broadcast
				false, 0, // outch
				false, 0); // mode
  g_client->NotifyServerOfChannelChange();
}

void vbox_local_channel::on_checkbutton_local_stereo_in_toggled()
{
  on_combobox_local_input_changed();
}

void vbox_local_channel::on_hscale_local_volume_value_changed()
{
  g_client->SetLocalChannelMonitoring(_idx,
				      true, DB2VAL(hscale_local_volume->get_value()), // vol
				      false, 0.0f, // pan
				      false, false, // mute
				      false, false); // solo
  g_client->NotifyServerOfChannelChange();
}

void vbox_local_channel::on_hscale_local_pan_value_changed()
{
  g_client->SetLocalChannelMonitoring(_idx,
				      false, 0.0f, // vol
				      true, hscale_local_pan->get_value(), // pan
				      false, false, // mute
				      false, false); // solo
  g_client->NotifyServerOfChannelChange();
}

void vbox_local_channel::on_combobox_local_output_changed()
{
  int channel = combobox_local_output->get_active_row_number();
  if (g_audio && (channel == g_audio->m_outnch)) {
    if (g_audio->addOutputChannel())
      window->update_outputLists();
  }
  channel &= 1023;
  if (!checkbutton_local_stereo_out->get_active())
    channel |= 1024;
  g_client->SetLocalChannelInfo(_idx,
				NULL, // name
				true, channel, // src
				false, 0, // bitrate
				false, false, // broadcast
				false, 0, // outch
				false, 0); // mode
  g_client->NotifyServerOfChannelChange();
}

void vbox_local_channel::on_checkbutton_local_stereo_out_toggled()
{
  on_combobox_local_output_changed();
}

void vbox_local_channel::on_spinbutton_bitrate_value_changed()
{
  g_client->SetLocalChannelInfo(_idx,
				NULL, // name
				false, 0, // src
				true, (int)spinbutton_bitrate->get_value(), // bitrate
				false, false, // broadcast
				false, 0, // outch
				false, 0); // mode
  g_client->NotifyServerOfChannelChange();
}

void vbox_local_channel::on_checkbutton_local_mute_toggled()
{
  g_client->SetLocalChannelMonitoring(_idx,
				      false, 0.0f, // vol
				      false, 0.0f, // pan
				      true, checkbutton_local_mute->get_active(), // mute
				      false, false); // solo
  g_client->NotifyServerOfChannelChange();
}

void vbox_local_channel::on_checkbutton_local_solo_toggled()
{
  g_client->SetLocalChannelMonitoring(_idx,
				      false, 0.0f, // vol
				      false, 0.0f, // pan
				      false, false, // mute
				      true, checkbutton_local_solo->get_active()); // solo
  g_client->NotifyServerOfChannelChange();
}

void vbox_local_channel::on_button_local_remove_clicked()
{
  g_client->DeleteLocalChannel(_idx);
  g_client->NotifyServerOfChannelChange();
  Gtk::Container* parent = get_parent();
  parent->remove(*this);
}

void vbox_local_channel::on_combobox_local_mode_changed()
{
  g_client->SetLocalChannelInfo(_idx,
				NULL, // name
				false, 0, // src
				false, 0, // bitrate
				false, false, // broadcast
				false, 0, // outch
				true, combobox_local_mode->get_active_row_number()<<1); // mode
  g_client->NotifyServerOfChannelChange();
}
