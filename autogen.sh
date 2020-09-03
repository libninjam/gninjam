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

echo "Patching file po/Makefile.in.in"
PACKAGE_VERSION=0.1.3
AUTHOR="Tobias Gehrig <tobias@gehrignet.de>"
BUG_TRACKER=https://github.com/libninjam/gninjam/issues
XGETTEXT_PATCH='s|\t  --msgid-bugs-address=.*|'
XGETTEXT_PATCH+='\t  --default-domain=gninjam \\'
XGETTEXT_PATCH+='\n\t  --package-name=gNinjam \\'
XGETTEXT_PATCH+='\n\t  --package-version='${PACKAGE_VERSION}' \\'
XGETTEXT_PATCH+='\n\t  --copyright-holder='"'${AUTHOR}'"' \\'
XGETTEXT_PATCH+='\n\t  --msgid-bugs-address='"'${BUG_TRACKER}'"' \\|'
MSGFMT_PATCH='s|&& $(GMSGFMT)|'
MSGFMT_PATCH+='\n\tmsgcmp --use-untranslated --use-fuzzy $< $(GETTEXT_PACKAGE).pot \|\| \\'
MSGFMT_PATCH+='\n\t  ! echo "[MAINTENANCE]: translation file '"'\$<'"' requires attention"'
MSGFMT_PATCH+='\n\tfalse'
MSGFMT_PATCH+='\n\t$(GMSGFMT) --check --use-fuzzy|'
UPDATE_PO_PATCH='s|update-po: Makefile|update-po: Makefile'
UPDATE_PO_PATCH+='\n\tmv $(GETTEXT_PACKAGE).pot{,.orig}'
UPDATE_PO_PATCH+='\n\t$(MAKE) $(GETTEXT_PACKAGE).pot'
UPDATE_PO_PATCH+='\n\t'"sed -i 's/# SOME DESCRIPTIVE TITLE./# gNinjam - Gtk client for NINJAM - translation template/' \$(GETTEXT_PACKAGE).pot"
UPDATE_PO_PATCH+='\n\t'"sed -i 's/# Copyright (C) YEAR /# Copyright (C) 2006-2008 /' \$(GETTEXT_PACKAGE).pot"
UPDATE_PO_PATCH+='\n\t! diff $(GETTEXT_PACKAGE).pot{,.orig} \| grep -Ev "[<>] \\\"POT-Creation-Date: " \&> \/dev\/null \|\| \\'
UPDATE_PO_PATCH+='\n\t  ! echo "[MAINTENANCE]: translation template '"'\$(GETTEXT_PACKAGE).pot'"' requires attention"|'
for patch in "${XGETTEXT_PATCH}" "${MSGFMT_PATCH}" "${UPDATE_PO_PATCH}"
do  sed -i "${patch}" po/Makefile.in.in
done

# ./configure $* && $MAKE
