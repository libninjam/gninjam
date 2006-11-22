// generated 2006/11/19 2:10:18 CET by tobias@THINKPAD-T43.(none)
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
#include <gtkmm/label.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/table.h>
#include <gtkmm/notebook.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/box.h>

window_preferences_glade::window_preferences_glade(
) : Gtk::Window(Gtk::WINDOW_TOPLEVEL)
{  window_preferences = this;
   gmm_data = new GlademmData(get_accel_group());
   
   Gtk::Label *label10 = Gtk::manage(new class Gtk::Label("Audio driver:"));
   combobox_audiodriver = Gtk::manage(new class Gtk::ComboBox());
   entry_sessiondir = Gtk::manage(new class Gtk::Entry());
   
   Gtk::Label *label11 = Gtk::manage(new class Gtk::Label("Session directory:"));
   Gtk::Label *label12 = Gtk::manage(new class Gtk::Label("Save local audio:"));
   combobox_savelocalaudio = Gtk::manage(new class Gtk::ComboBox());
   
   Gtk::Adjustment *spinbutton_oggbitrate_adj = Gtk::manage(new class Gtk::Adjustment(1, 64, 256, 1, 10, 10));
   spinbutton_oggbitrate = Gtk::manage(new class Gtk::SpinButton(*spinbutton_oggbitrate_adj, 1, 0));
   
   Gtk::Label *label14 = Gtk::manage(new class Gtk::Label("Ogg bitrate:"));
   checkbutton_savelog = Gtk::manage(new class Gtk::CheckButton("Save log files"));
   
   Gtk::Table *table3 = Gtk::manage(new class Gtk::Table(2, 2, false));
   Gtk::Label *label18 = Gtk::manage(new class Gtk::Label("General"));
   Gtk::Label *label15 = Gtk::manage(new class Gtk::Label("Client name:"));
   Gtk::Label *label16 = Gtk::manage(new class Gtk::Label("Number of input channels:"));
   Gtk::Adjustment *spinbutton_jack_noutputchannels_adj = Gtk::manage(new class Gtk::Adjustment(1, 0, 100, 1, 10, 10));
   spinbutton_jack_noutputchannels = Gtk::manage(new class Gtk::SpinButton(*spinbutton_jack_noutputchannels_adj, 1, 0));
   
   Gtk::Adjustment *spinbutton_jack_ninputchannels_adj = Gtk::manage(new class Gtk::Adjustment(1, 0, 100, 1, 10, 10));
   spinbutton_jack_ninputchannels = Gtk::manage(new class Gtk::SpinButton(*spinbutton_jack_ninputchannels_adj, 1, 0));
   entry_jack_client_name = Gtk::manage(new class Gtk::Entry());
   
   Gtk::Label *label17 = Gtk::manage(new class Gtk::Label("Number of output channels"));
   Gtk::Table *table5 = Gtk::manage(new class Gtk::Table(2, 2, false));
   Gtk::Label *label19 = Gtk::manage(new class Gtk::Label("Jack settings"));
   Gtk::Label *label13 = Gtk::manage(new class Gtk::Label("Config string:"));
   entry_alsa_configstring = Gtk::manage(new class Gtk::Entry());
   
   Gtk::Table *table4 = Gtk::manage(new class Gtk::Table(2, 2, false));
   Gtk::Label *label20 = Gtk::manage(new class Gtk::Label("Alsa settings"));
   Gtk::Label *label21 = Gtk::manage(new class Gtk::Label("Hostname:"));
   Gtk::Label *label22 = Gtk::manage(new class Gtk::Label("Username:"));
   Gtk::Label *label23 = Gtk::manage(new class Gtk::Label("Password:"));
   entry_hostname = Gtk::manage(new class Gtk::Entry());
   entry_username = Gtk::manage(new class Gtk::Entry());
   entry_password = Gtk::manage(new class Gtk::Entry());
   
   Gtk::Table *table6 = Gtk::manage(new class Gtk::Table(2, 2, false));
   Gtk::Label *label24 = Gtk::manage(new class Gtk::Label("Default Connection"));
   Gtk::Notebook *notebook1 = Gtk::manage(new class Gtk::Notebook());
   Gtk::Button *button_abort = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-cancel")));
   Gtk::Button *button_apply = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-apply")));
   Gtk::Button *button_ok = Gtk::manage(new class Gtk::Button(Gtk::StockID("gtk-ok")));
   Gtk::HButtonBox *hbuttonbox1 = Gtk::manage(new class Gtk::HButtonBox(Gtk::BUTTONBOX_END, 4));
   Gtk::VBox *vbox5 = Gtk::manage(new class Gtk::VBox(false, 4));
   label10->set_name("label10");
   label10->set_alignment(0,0.5);
   label10->set_padding(0,0);
   label10->set_justify(Gtk::JUSTIFY_LEFT);
   label10->set_line_wrap(false);
   label10->set_use_markup(false);
   label10->set_selectable(false);
   combobox_audiodriver->set_name("combobox_audiodriver");
   entry_sessiondir->set_name("entry_sessiondir");
   entry_sessiondir->set_flags(Gtk::CAN_FOCUS);
   entry_sessiondir->set_visibility(true);
   entry_sessiondir->set_editable(true);
   entry_sessiondir->set_max_length(0);
   entry_sessiondir->set_text("");
   entry_sessiondir->set_has_frame(true);
   entry_sessiondir->set_activates_default(false);
   label11->set_name("label11");
   label11->set_alignment(0,0.5);
   label11->set_padding(0,0);
   label11->set_justify(Gtk::JUSTIFY_LEFT);
   label11->set_line_wrap(false);
   label11->set_use_markup(false);
   label11->set_selectable(false);
   label12->set_name("label12");
   label12->set_alignment(0,0.5);
   label12->set_padding(0,0);
   label12->set_justify(Gtk::JUSTIFY_LEFT);
   label12->set_line_wrap(false);
   label12->set_use_markup(false);
   label12->set_selectable(false);
   combobox_savelocalaudio->set_name("combobox_savelocalaudio");
   spinbutton_oggbitrate->set_name("spinbutton_oggbitrate");
   spinbutton_oggbitrate->set_flags(Gtk::CAN_FOCUS);
   spinbutton_oggbitrate->set_update_policy(Gtk::UPDATE_ALWAYS);
   spinbutton_oggbitrate->set_numeric(true);
   spinbutton_oggbitrate->set_digits(0);
   spinbutton_oggbitrate->set_wrap(false);
   label14->set_name("label14");
   label14->set_alignment(0,0.5);
   label14->set_padding(0,0);
   label14->set_justify(Gtk::JUSTIFY_LEFT);
   label14->set_line_wrap(false);
   label14->set_use_markup(false);
   label14->set_selectable(false);
   checkbutton_savelog->set_name("checkbutton_savelog");
   checkbutton_savelog->set_flags(Gtk::CAN_FOCUS);
   checkbutton_savelog->set_relief(Gtk::RELIEF_NORMAL);
   checkbutton_savelog->set_mode(true);
   checkbutton_savelog->set_active(false);
   table3->set_name("table3");
   table3->set_border_width(4);
   table3->set_row_spacings(4);
   table3->set_col_spacings(4);
   table3->attach(*label10, 0, 1, 0, 1, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table3->attach(*combobox_audiodriver, 1, 2, 0, 1, Gtk::FILL, Gtk::FILL, 0, 0);
   table3->attach(*entry_sessiondir, 1, 2, 1, 2, Gtk::EXPAND|Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table3->attach(*label11, 0, 1, 1, 2, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table3->attach(*label12, 0, 1, 2, 3, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table3->attach(*combobox_savelocalaudio, 1, 2, 2, 3, Gtk::FILL, Gtk::FILL, 0, 0);
   table3->attach(*spinbutton_oggbitrate, 1, 2, 3, 4, Gtk::EXPAND|Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table3->attach(*label14, 0, 1, 3, 4, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table3->attach(*checkbutton_savelog, 1, 2, 4, 5, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   label18->set_name("label18");
   label18->set_alignment(0.5,0.5);
   label18->set_padding(0,0);
   label18->set_justify(Gtk::JUSTIFY_LEFT);
   label18->set_line_wrap(false);
   label18->set_use_markup(false);
   label18->set_selectable(false);
   label15->set_name("label15");
   label15->set_alignment(0,0.5);
   label15->set_padding(0,0);
   label15->set_justify(Gtk::JUSTIFY_LEFT);
   label15->set_line_wrap(false);
   label15->set_use_markup(false);
   label15->set_selectable(false);
   label16->set_name("label16");
   label16->set_alignment(0,0.5);
   label16->set_padding(0,0);
   label16->set_justify(Gtk::JUSTIFY_LEFT);
   label16->set_line_wrap(false);
   label16->set_use_markup(false);
   label16->set_selectable(false);
   spinbutton_jack_noutputchannels->set_name("spinbutton_jack_noutputchannels");
   spinbutton_jack_noutputchannels->set_flags(Gtk::CAN_FOCUS);
   spinbutton_jack_noutputchannels->set_update_policy(Gtk::UPDATE_ALWAYS);
   spinbutton_jack_noutputchannels->set_numeric(true);
   spinbutton_jack_noutputchannels->set_digits(0);
   spinbutton_jack_noutputchannels->set_wrap(false);
   spinbutton_jack_ninputchannels->set_name("spinbutton_jack_ninputchannels");
   spinbutton_jack_ninputchannels->set_flags(Gtk::CAN_FOCUS);
   spinbutton_jack_ninputchannels->set_update_policy(Gtk::UPDATE_ALWAYS);
   spinbutton_jack_ninputchannels->set_numeric(true);
   spinbutton_jack_ninputchannels->set_digits(0);
   spinbutton_jack_ninputchannels->set_wrap(false);
   entry_jack_client_name->set_name("entry_jack_client_name");
   entry_jack_client_name->set_flags(Gtk::CAN_FOCUS);
   entry_jack_client_name->set_visibility(true);
   entry_jack_client_name->set_editable(true);
   entry_jack_client_name->set_max_length(0);
   entry_jack_client_name->set_text("");
   entry_jack_client_name->set_has_frame(true);
   entry_jack_client_name->set_activates_default(false);
   label17->set_name("label17");
   label17->set_alignment(0,0.5);
   label17->set_padding(0,0);
   label17->set_justify(Gtk::JUSTIFY_LEFT);
   label17->set_line_wrap(false);
   label17->set_use_markup(false);
   label17->set_selectable(false);
   table5->set_name("table5");
   table5->set_border_width(4);
   table5->set_row_spacings(4);
   table5->set_col_spacings(4);
   table5->attach(*label15, 0, 1, 0, 1, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table5->attach(*label16, 0, 1, 1, 2, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table5->attach(*spinbutton_jack_noutputchannels, 1, 2, 2, 3, Gtk::EXPAND|Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table5->attach(*spinbutton_jack_ninputchannels, 1, 2, 1, 2, Gtk::EXPAND|Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table5->attach(*entry_jack_client_name, 1, 2, 0, 1, Gtk::EXPAND|Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table5->attach(*label17, 0, 1, 2, 3, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   label19->set_name("label19");
   label19->set_alignment(0.5,0.5);
   label19->set_padding(0,0);
   label19->set_justify(Gtk::JUSTIFY_LEFT);
   label19->set_line_wrap(false);
   label19->set_use_markup(false);
   label19->set_selectable(false);
   label13->set_name("label13");
   label13->set_alignment(0,0.5);
   label13->set_padding(0,0);
   label13->set_justify(Gtk::JUSTIFY_LEFT);
   label13->set_line_wrap(false);
   label13->set_use_markup(false);
   label13->set_selectable(false);
   entry_alsa_configstring->set_name("entry_alsa_configstring");
   entry_alsa_configstring->set_flags(Gtk::CAN_FOCUS);
   entry_alsa_configstring->set_visibility(true);
   entry_alsa_configstring->set_editable(true);
   entry_alsa_configstring->set_max_length(0);
   entry_alsa_configstring->set_text("");
   entry_alsa_configstring->set_has_frame(true);
   entry_alsa_configstring->set_activates_default(false);
   table4->set_name("table4");
   table4->set_border_width(4);
   table4->set_row_spacings(4);
   table4->set_col_spacings(4);
   table4->attach(*label13, 0, 1, 0, 1, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table4->attach(*entry_alsa_configstring, 1, 2, 0, 1, Gtk::EXPAND|Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   label20->set_name("label20");
   label20->set_alignment(0.5,0.5);
   label20->set_padding(0,0);
   label20->set_justify(Gtk::JUSTIFY_LEFT);
   label20->set_line_wrap(false);
   label20->set_use_markup(false);
   label20->set_selectable(false);
   label21->set_name("label21");
   label21->set_alignment(0,0.5);
   label21->set_padding(0,0);
   label21->set_justify(Gtk::JUSTIFY_LEFT);
   label21->set_line_wrap(false);
   label21->set_use_markup(false);
   label21->set_selectable(false);
   label22->set_name("label22");
   label22->set_alignment(0,0.5);
   label22->set_padding(0,0);
   label22->set_justify(Gtk::JUSTIFY_LEFT);
   label22->set_line_wrap(false);
   label22->set_use_markup(false);
   label22->set_selectable(false);
   label23->set_name("label23");
   label23->set_alignment(0,0.5);
   label23->set_padding(0,0);
   label23->set_justify(Gtk::JUSTIFY_LEFT);
   label23->set_line_wrap(false);
   label23->set_use_markup(false);
   label23->set_selectable(false);
   entry_hostname->set_name("entry_hostname");
   entry_hostname->set_flags(Gtk::CAN_FOCUS);
   entry_hostname->set_visibility(true);
   entry_hostname->set_editable(true);
   entry_hostname->set_max_length(0);
   entry_hostname->set_text("");
   entry_hostname->set_has_frame(true);
   entry_hostname->set_activates_default(false);
   entry_username->set_name("entry_username");
   entry_username->set_flags(Gtk::CAN_FOCUS);
   entry_username->set_visibility(true);
   entry_username->set_editable(true);
   entry_username->set_max_length(0);
   entry_username->set_text("");
   entry_username->set_has_frame(true);
   entry_username->set_activates_default(false);
   entry_password->set_name("entry_password");
   entry_password->set_flags(Gtk::CAN_FOCUS);
   entry_password->set_visibility(false);
   entry_password->set_editable(true);
   entry_password->set_max_length(0);
   entry_password->set_text("");
   entry_password->set_has_frame(true);
   entry_password->set_activates_default(false);
   table6->set_name("table6");
   table6->set_border_width(4);
   table6->set_row_spacings(4);
   table6->set_col_spacings(4);
   table6->attach(*label21, 0, 1, 0, 1, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table6->attach(*label22, 0, 1, 1, 2, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table6->attach(*label23, 0, 1, 2, 3, Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table6->attach(*entry_hostname, 1, 2, 0, 1, Gtk::EXPAND|Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table6->attach(*entry_username, 1, 2, 1, 2, Gtk::EXPAND|Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   table6->attach(*entry_password, 1, 2, 2, 3, Gtk::EXPAND|Gtk::FILL, Gtk::AttachOptions(), 0, 0);
   label24->set_name("label24");
   label24->set_alignment(0.5,0.5);
   label24->set_padding(0,0);
   label24->set_justify(Gtk::JUSTIFY_LEFT);
   label24->set_line_wrap(false);
   label24->set_use_markup(false);
   label24->set_selectable(false);
   notebook1->set_name("notebook1");
   notebook1->set_flags(Gtk::CAN_FOCUS);
   notebook1->set_show_tabs(true);
   notebook1->set_show_border(true);
   notebook1->set_tab_pos(Gtk::POS_TOP);
   notebook1->set_scrollable(false);
   notebook1->append_page(*table3, *label18);
   notebook1->pages().back().set_tab_label_packing(false, true, Gtk::PACK_START);
   notebook1->append_page(*table5, *label19);
   notebook1->pages().back().set_tab_label_packing(false, true, Gtk::PACK_START);
   notebook1->append_page(*table4, *label20);
   notebook1->pages().back().set_tab_label_packing(false, true, Gtk::PACK_START);
   notebook1->append_page(*table6, *label24);
   notebook1->pages().back().set_tab_label_packing(false, true, Gtk::PACK_START);
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
   vbox5->set_name("vbox5");
   vbox5->set_border_width(4);
   vbox5->pack_start(*notebook1);
   vbox5->pack_start(*hbuttonbox1, Gtk::PACK_SHRINK, 0);
   window_preferences->set_name("window_preferences");
   window_preferences->set_title("Preferences");
   window_preferences->set_modal(false);
   window_preferences->property_window_position().set_value(Gtk::WIN_POS_NONE);
   window_preferences->set_resizable(true);
   window_preferences->property_destroy_with_parent().set_value(false);
   window_preferences->add(*vbox5);
   label10->show();
   combobox_audiodriver->show();
   entry_sessiondir->show();
   label11->show();
   label12->show();
   combobox_savelocalaudio->show();
   spinbutton_oggbitrate->show();
   label14->show();
   checkbutton_savelog->show();
   table3->show();
   label18->show();
   label15->show();
   label16->show();
   spinbutton_jack_noutputchannels->show();
   spinbutton_jack_ninputchannels->show();
   entry_jack_client_name->show();
   label17->show();
   table5->show();
   label19->show();
   label13->show();
   entry_alsa_configstring->show();
   table4->show();
   label20->show();
   label21->show();
   label22->show();
   label23->show();
   entry_hostname->show();
   entry_username->show();
   entry_password->show();
   table6->show();
   label24->show();
   notebook1->show();
   button_abort->show();
   button_apply->show();
   button_ok->show();
   hbuttonbox1->show();
   vbox5->show();
   button_abort->signal_clicked().connect(SigC::slot(*this, &window_preferences_glade::on_button_abort_clicked), false);
   button_apply->signal_clicked().connect(SigC::slot(*this, &window_preferences_glade::on_button_apply_clicked), false);
   button_ok->signal_clicked().connect(SigC::slot(*this, &window_preferences_glade::on_button_ok_clicked), false);
   window_preferences->signal_show().connect(SigC::slot(*this, &window_preferences_glade::on_window_preferences_show), false);
}

window_preferences_glade::~window_preferences_glade()
{  delete gmm_data;
}