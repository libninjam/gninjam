// generated 2006/11/15 18:53:44 CET by tobias@THINKPAD-T43.(none)
// using glademm V2.6.0
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/tobias/src/svn/gninjam/trunk/gninjam.glade
// for gtk 2.10.6 and gtkmm 2.10.3
//
// Please modify the corresponding derived classes in ./src/window_preferences.cc


#if defined __GNUC__ && __GNUC__ < 3
#error This program will crash if compiled with g++ 2.x
// see the dynamic_cast bug in the gtkmm FAQ
#endif //
#include "config.h"
#include <gtkmmconfig.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#include <sigc++/compatibility.h>
#define GMM_GTKMM_22_24(a,b) b
#else //gtkmm 2.2
#define GMM_GTKMM_22_24(a,b) a
#endif //
#include "window_preferences_glade.hh"
#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/label.h>
#include <gtkmm/table.h>

window_preferences_glade::window_preferences_glade(
) : Gtk::Window(Gtk::WINDOW_TOPLEVEL)
{  window_preferences = this;
   gmm_data = new GlademmData(get_accel_group());
   
   Gtk::Button *button_abort = Gtk::manage(new class Gtk::Button("Abort"));
   Gtk::Button *button_apply = Gtk::manage(new class Gtk::Button("Apply"));
   Gtk::Button *button_ok = Gtk::manage(new class Gtk::Button("OK"));
   Gtk::HButtonBox *hbuttonbox1 = Gtk::manage(new class Gtk::HButtonBox(Gtk::BUTTONBOX_DEFAULT_STYLE, 0));
   Gtk::Label *label10 = Gtk::manage(new class Gtk::Label("Audio driver:"));
   combobox1 = Gtk::manage(new class Gtk::ComboBox());
   
   Gtk::Label *label11 = Gtk::manage(new class Gtk::Label("Session directory"));
   filechooserbutton1 = Gtk::manage(new class Gtk::Label("GtkFileChooserButton filechooserbutton1"));
   
   Gtk::Table *table3 = Gtk::manage(new class Gtk::Table(2, 2, false));
   button_abort->set_name("button_abort");
   button_abort->set_flags(Gtk::CAN_FOCUS);
   button_abort->set_flags(Gtk::CAN_DEFAULT);
   button_abort->set_relief(Gtk::RELIEF_NORMAL);
   button_apply->set_name("button_apply");
   button_apply->set_flags(Gtk::CAN_FOCUS);
   button_apply->set_flags(Gtk::CAN_DEFAULT);
   button_apply->set_relief(Gtk::RELIEF_NORMAL);
   button_ok->set_name("button_ok");
   button_ok->set_flags(Gtk::CAN_FOCUS);
   button_ok->set_flags(Gtk::CAN_DEFAULT);
   button_ok->set_relief(Gtk::RELIEF_NORMAL);
   hbuttonbox1->set_name("hbuttonbox1");
   hbuttonbox1->pack_start(*button_abort);
   hbuttonbox1->pack_start(*button_apply);
   hbuttonbox1->pack_start(*button_ok);
   label10->set_name("label10");
   label10->set_alignment(0,0.5);
   label10->set_padding(0,0);
   label10->set_justify(Gtk::JUSTIFY_LEFT);
   label10->set_line_wrap(false);
   label10->set_use_markup(false);
   label10->set_selectable(false);
   combobox1->set_name("combobox1");
   label11->set_name("label11");
   label11->set_alignment(0,0.5);
   label11->set_padding(0,0);
   label11->set_justify(Gtk::JUSTIFY_LEFT);
   label11->set_line_wrap(false);
   label11->set_use_markup(false);
   label11->set_selectable(false);
   filechooserbutton1->set_name("filechooserbutton1");
   table3->set_name("table3");
   table3->set_row_spacings(0);
   table3->set_col_spacings(0);
   table3->attach(*hbuttonbox1, 0, 2, 2, 3, Gtk::FILL, Gtk::FILL, 0, 0);
   table3->attach(*label10, 0, 1, 0, 1, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table3->attach(*combobox1, 1, 2, 0, 1, Gtk::FILL, Gtk::FILL, 0, 0);
   table3->attach(*label11, 0, 1, 1, 2, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table3->attach(*filechooserbutton1, 1, 2, 1, 2, Gtk::FILL, Gtk::FILL, 0, 0);
   window_preferences->set_name("window_preferences");
   window_preferences->set_title("Preferences");
   window_preferences->set_modal(false);
   window_preferences->property_window_position().set_value(Gtk::WIN_POS_NONE);
   window_preferences->set_resizable(true);
   window_preferences->property_destroy_with_parent().set_value(false);
   window_preferences->add(*table3);
   button_abort->show();
   button_apply->show();
   button_ok->show();
   hbuttonbox1->show();
   label10->show();
   combobox1->show();
   label11->show();
   filechooserbutton1->show();
   table3->show();
   button_abort->signal_clicked().connect(SigC::slot(*this, &window_preferences_glade::on_button_abort_clicked), false);
   button_apply->signal_clicked().connect(SigC::slot(*this, &window_preferences_glade::on_button_apply_clicked), false);
   button_ok->signal_clicked().connect(SigC::slot(*this, &window_preferences_glade::on_button_ok_clicked), false);
}

window_preferences_glade::~window_preferences_glade()
{  delete gmm_data;
}