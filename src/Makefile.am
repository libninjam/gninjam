# generated 2006/11/13 15:00:08 CET by tobias@THINKPAD-T43.(none)
# using glademm V2.6.0
#
# newer (non customized) versions of this file go to Makefile.am_new
bin_PROGRAMS = gninjam

gninjam_SOURCES = \
	gninjam.cc\
	gNinjamClient_glade.cc \
	gNinjamClient.cc \
	dialog_license_glade.cc \
	dialog_license.cc \
	window_preferences_glade.cc \
	window_preferences.cc \
	dialog_connect_glade.cc \
	dialog_connect.cc 

noinst_HEADERS = \
	gNinjamClient_glade.hh \
	gNinjamClient.hh \
	dialog_license_glade.hh \
	dialog_license.hh \
	window_preferences_glade.hh \
	window_preferences.hh \
	dialog_connect_glade.hh \
	dialog_connect.hh 

AM_CXXFLAGS = @GTKMM_CFLAGS@ -I/usr/include/libninjam -s -malign-double -lpthread

gninjam_LDADD = @LIBS@ @GTKMM_LIBS@ -lninjam-client
