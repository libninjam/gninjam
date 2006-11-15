// generated 2006/11/14 0:28:06 CET by tobias@THINKPAD-T43.(none)
// using glademm V2.6.0
//
// newer (non customized) versions of this file go to dialog_license.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "dialog_license.hh"

void dialog_license::set_text(char *text)
{
  Glib::RefPtr<Gtk::TextBuffer> buffer = textview2->get_buffer();
  buffer->set_text(text, text+strlen(text));
}
