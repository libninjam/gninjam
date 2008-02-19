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

#ifndef _WINDOW_PREFERENCES_HH
#  include "window_preferences_glade.hh"
#  define _WINDOW_PREFERENCES_HH

#include <gconfmm.h>
#include <gtkmm/liststore.h>

class window_preferences : public window_preferences_glade
{
public:
  window_preferences();
private:
  void on_checkbutton_anonymous_toggled();
  void on_button_autosubscription_add_clicked();
  void on_button_autosubscription_delete_clicked();
  void on_button_autosubscription_clear_clicked();
  void on_button_abort_clicked();
  void on_button_apply_clicked();
  void on_button_ok_clicked();
  void on_window_preferences_show();
  Glib::RefPtr<Gnome::Conf::Client> _gconf_client;
  Glib::ustring _gconf_dir;
  Gtk::TreeModelColumn<Glib::ustring> _textcolumn;
  Glib::RefPtr<Gtk::ListStore> _refListStoreAutosubscription;
};
#endif
