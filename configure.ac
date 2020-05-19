#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([gninjam],[0.1.2],[https://github.com/libninjam/gninjam/issues])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/gninjam.cc])
AC_CONFIG_HEADERS(config.h)
AC_CONFIG_MACRO_DIRS([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
LT_PREREQ
LT_INIT

# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_self], ,
  [AC_MSG_ERROR(can not find the libpthread library)])
AC_CHECK_LIB([asound], [main], ,
  [AC_MSG_ERROR(can not find the ALSA library (libasound2).
    http://www.alsa-project.org/)])
AC_CHECK_LIB([jack], [jack_client_open], ,
  [AC_MSG_ERROR(can not find the JACK audio library (libjack).
    https://jackaudio.org/)])

# Checks for header files.
AC_CHECK_HEADERS([float.h libintl.h memory.h stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_CHECK_FUNCS([memset mkdir strncasecmp])

dnl gettext support
GETTEXT_PACKAGE=gninjam
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE",
		[the gettext domain])
AM_GLIB_GNU_GETTEXT

dnl Set PACKAGE_LOCALE_DIR in config.h.
if test "x${prefix}" = "xNONE"; then
  AC_DEFINE_UNQUOTED(PACKAGE_LOCALE_DIR, "${ac_default_prefix}/${DATADIRNAME}/locale",
		[the location of locale files])
else
  AC_DEFINE_UNQUOTED(PACKAGE_LOCALE_DIR, "${prefix}/${DATADIRNAME}/locale",
		[the location of locale files])
fi

AC_LANG([C++])

PKG_CHECK_MODULES(GTKMM, [gtkmm-2.4 >= 2.4.0])
AC_SUBST(GTKMM_CFLAGS)
AC_SUBST(GTKMM_LIBS)

PKG_CHECK_MODULES(GCONF, [gconfmm-2.6 >= 2.6.0])
AC_SUBST(GCONF_CFLAGS)
AC_SUBST(GCONF_LIBS)

PKG_CHECK_MODULES(NINJAM, [ninjam-client >= 0.02-6])
AC_SUBST(NINJAM_CFLAGS)
AC_SUBST(NINJAM_LIBS)

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 po/Makefile.in])
AC_OUTPUT
