// generated 2008/1/31 4:43:48 CET by tobias@THINKPAD-T43.(none)
// using glademm V2.6.0
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/tobias/src/svn/gninjam/trunk/gninjam.glade
// for gtk 2.12.0 and gtkmm 2.12.0
//
// Please modify the corresponding derived classes in ./src/vbox_local_channel.hh and./src/vbox_local_channel.cc

#ifndef _VBOX_LOCAL_CHANNEL_GLADE_HH
#  define _VBOX_LOCAL_CHANNEL_GLADE_HH


#if !defined(GLADEMM_DATA)
#define GLADEMM_DATA 
#include <gtkmm/accelgroup.h>

class GlademmData
{  
        
        Glib::RefPtr<Gtk::AccelGroup> accgrp;
public:
        
        GlademmData(Glib::RefPtr<Gtk::AccelGroup> ag) : accgrp(ag)
        {  
        }
        
        Glib::RefPtr<Gtk::AccelGroup>  getAccelGroup()
        {  return accgrp;
        }
};
#endif //GLADEMM_DATA

#include <gtkmm/box.h>
#include <gtkmm/tooltips.h>
#include <gtkmm/entry.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/combobox.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/scale.h>
#include <gtkmm/spinbutton.h>

class vbox_local_channel_glade : public Gtk::VBox
{  
        
        Gtk::Tooltips _tooltips;
protected:
        class Gtk::Entry * entry_local_channelname;
        class Gtk::CheckButton * checkbutton_local_transmit;
        class Gtk::ComboBox * combobox_local_input;
        class Gtk::ProgressBar * progressbar_local;
        class Gtk::HScale * hscale_local_volume;
        class Gtk::HScale * hscale_local_pan;
        class Gtk::SpinButton * spinbutton_bitrate;
        class Gtk::CheckButton * checkbutton_local_mute;
        class Gtk::CheckButton * checkbutton_local_solo;
        
        vbox_local_channel_glade(GlademmData *gmm_data);
        
        ~vbox_local_channel_glade();
private:
        virtual void on_entry_local_channelname_changed() = 0;
        virtual void on_checkbutton_local_transmit_toggled() = 0;
        virtual void on_combobox_local_input_changed() = 0;
        virtual void on_hscale_local_volume_value_changed() = 0;
        virtual void on_hscale_local_pan_value_changed() = 0;
        virtual void on_spinbutton_bitrate_value_changed() = 0;
        virtual void on_checkbutton_local_mute_toggled() = 0;
        virtual void on_checkbutton_local_solo_toggled() = 0;
        virtual void on_button_local_remove_clicked() = 0;
};
#endif
