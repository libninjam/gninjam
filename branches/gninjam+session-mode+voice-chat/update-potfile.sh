#!/bin/sh
xgettext -C -f po/POTFILES.in -d gninjam -p po -o gninjam.pot --keyword=_ --copyright-holder="Tobias Gehrig"

for lang in de ; do
    grep -v "^#:" po/$lang.po > po/$lang.po.tmp
    mv po/$lang.po.tmp po/$lang.po
    xgettext -C --omit-header -j -f po/POTFILES.in -d gninjam -p po -o $lang.po --keyword=_
done