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
#include "vbox_local_channel.hh"

#include "common.hh"

#include <gtkmm/liststore.h>

#include <ninjam/njmisc.h>
#include <ninjam/audiostream.h>
#include <ninjam/njclient.h>

extern audioStreamer *g_audio;
extern NJClient *g_client;

vbox_local_channel::vbox_local_channel(GlademmData *gmm_data)
  : vbox_local_channel_glade(gmm_data),
    _idx(-1)
{
  hscale_local_pan->signal_format_value().connect(sigc::ptr_fun(on_hscale_pan_format_value), false);
  hscale_local_volume->signal_format_value().connect(sigc::ptr_fun(on_hscale_volume_format_value), false);
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

void vbox_local_channel::init(int idx)
{
  _idx = idx;
  int sourcechannel, bitrate;
  bool broadcast;
  char *channelname = g_client->GetLocalChannelInfo(_idx,
						    &sourcechannel,
						    &bitrate,
						    &broadcast);
  entry_local_channelname->set_text(channelname);
  Gtk::TreeModel::ColumnRecord column_model;
  Gtk::TreeModelColumn<Glib::ustring> textcolumn;
  column_model.add(textcolumn);
  Glib::RefPtr<Gtk::ListStore> model = Gtk::ListStore::create(column_model);
  combobox_local_input->set_model(model);
  const char* sourcename;
  for (int i=0; (sourcename = g_audio->GetChannelName(i)) != NULL; i++) {
    Gtk::TreeModel::Row row = *(model->append());
    row[textcolumn] = sourcename;
  }
  Gtk::TreeModel::Row row = *(model->append());
  row[textcolumn] = "Silence";
  combobox_local_input->pack_start(textcolumn);
  combobox_local_input->set_active(sourcechannel);
  checkbutton_local_transmit->set_active(broadcast);
  float volume, pan;
  bool mute, solo;
  g_client->GetLocalChannelMonitoring(_idx, &volume, &pan, &mute, &solo);
  hscale_local_volume->set_value(VAL2DB(volume));
  hscale_local_pan->set_value(pan);
  checkbutton_local_mute->set_active(mute);
  checkbutton_local_solo->set_active(solo);
}

void vbox_local_channel::on_entry_local_channelname_changed()
{  
  g_client->SetLocalChannelInfo(_idx,
				entry_local_channelname->get_text().c_str(), // name
				false, 0, // src
				false, 0, // bitrate
				false, false); // broadcast
  g_client->NotifyServerOfChannelChange();
}

void vbox_local_channel::on_checkbutton_local_transmit_toggled()
{
  g_client->SetLocalChannelInfo(_idx,
				NULL, // name
				false, 0, // src
				false, 0, // bitrate
				true, checkbutton_local_transmit->get_active()); // broadcast
  g_client->NotifyServerOfChannelChange();
}

void vbox_local_channel::on_combobox_local_input_changed()
{
  g_client->SetLocalChannelInfo(_idx,
				NULL, // name
				true, combobox_local_input->get_active_row_number(), // src
				false, 0, // bitrate
				false, false); // broadcast
  g_client->NotifyServerOfChannelChange();
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
