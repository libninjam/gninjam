#!/bin/sh
# generated 2006/11/13 17:45:20 CET by tobias@THINKPAD-T43.(none)
# using glademm V2.6.0

if test ! -f install-sh ; then touch install-sh ; fi ;

MAKE=`which make`
HAVE_GNU_MAKE=`$MAKE --version|grep -c "Free Software Foundation"`

if   test "$HAVE_GNU_MAKE" != "1"
then echo Only non-GNU make found: $MAKE
else echo `$MAKE --version | head -1` found
fi

if   which autoconf2.50 >/dev/null 2>&1
then AC_POSTFIX=2.50
elif which autoconf     >/dev/null 2>&1
then AC_POSTFIX=""
else echo 'you need autoconfig (2.58+ recommended) to generate the Makefile'
     exit 1
fi
echo `autoconf$AC_POSTFIX --version | head -1` found

if   which automake-1.6  >/dev/null 2>&1
then AM_POSTFIX=-1.6
elif which automake-1.7  >/dev/null 2>&1
then AM_POSTFIX=-1.7
elif which automake-1.8  >/dev/null 2>&1
then AM_POSTFIX=-1.8
elif which automake-1.9  >/dev/null 2>&1
then AM_POSTFIX=-1.9
elif which automake-1.16 >/dev/null 2>&1
then AM_POSTFIX=-1.16
elif which automake      >/dev/null 2>&1
then AM_POSTFIX=""
else echo 'you need automake (1.8.3+ recommended) to generate the Makefile'
     exit 1
fi
echo `automake$AM_POSTFIX --version | head -1` found

# autoreconf$AC_POSTFIX -fim _might_ do the trick, too.
#  chose to your taste
aclocal$AM_POSTFIX --install -I m4
libtoolize --force --copy
glib-gettextize --force --copy
if   test ! -e po/Makevars
then cp        po/Makevars.template po/Makevars
fi
if   test ! -e po/LINGUAS
then touch     po/LINGUAS
fi
autoheader$AC_POSTFIX
automake$AM_POSTFIX --add-missing --copy --gnits
autoconf$AC_POSTFIX
# ./configure $* && $MAKE
