// generated 2006/11/22 3:22:16 CET by tobias@THINKPAD-T43.(none)
// using glademm V2.6.0
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/tobias/src/svn/gninjam/trunk/gninjam.glade
// for gtk 2.10.6 and gtkmm 2.10.3
//
// Please modify the corresponding derived classes in ./src/RemoteUserFrame.hh and./src/RemoteUserFrame.cc

#ifndef _REMOTEUSERFRAME_GLADE_HH
#  define _REMOTEUSERFRAME_GLADE_HH


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

#include <gtkmm/frame.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>

class RemoteUserFrame_glade : public Gtk::Frame
{  
protected:
        
        class Gtk::VBox * vbox;
        class Gtk::Label * label;
        
        RemoteUserFrame_glade(GlademmData *gmm_data);
        
        ~RemoteUserFrame_glade();
};
#endif