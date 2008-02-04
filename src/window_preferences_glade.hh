// generated 2008/2/4 20:42:24 CET by tobias@THINKPAD-T43.(none)
// using glademm V2.6.0
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/tobias/src/svn/gninjam/trunk/gninjam.glade
// for gtk 2.12.0 and gtkmm 2.12.0
//
// Please modify the corresponding derived classes in ./src/window_preferences.hh and./src/window_preferences.cc

#ifndef _WINDOW_PREFERENCES_GLADE_HH
#  define _WINDOW_PREFERENCES_GLADE_HH


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

#include <gtkmm/window.h>
#include <gtkmm/combobox.h>
#include <gtkmm/entry.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/label.h>

class window_preferences_glade : public Gtk::Window
{  
        
        GlademmData *gmm_data;
public:
        class Gtk::Window * window_preferences;
protected:
        class Gtk::ComboBox * combobox_audiodriver;
        class Gtk::Entry * entry_sessiondir;
        class Gtk::ComboBox * combobox_savelocalaudio;
        class Gtk::SpinButton * spinbutton_oggbitrate;
        class Gtk::CheckButton * checkbutton_savelog;
        class Gtk::SpinButton * spinbutton_jack_noutputchannels;
        class Gtk::SpinButton * spinbutton_jack_ninputchannels;
        class Gtk::Entry * entry_jack_client_name;
        class Gtk::Entry * entry_alsa_configstring;
        class Gtk::Entry * entry_hostname;
        class Gtk::Entry * entry_username;
        class Gtk::Entry * entry_password;
        class Gtk::Label * label23;
        class Gtk::CheckButton * checkbutton_anonymous;
        
        window_preferences_glade();
        
        ~window_preferences_glade();
private:
        virtual void on_checkbutton_anonymous_toggled() = 0;
        virtual void on_button_abort_clicked() = 0;
        virtual void on_button_apply_clicked() = 0;
        virtual void on_button_ok_clicked() = 0;
        virtual void on_window_preferences_show() = 0;
};
#endif
