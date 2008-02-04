#!/bin/sh
for i in *glade.cc *glade.hh; do
    mv $i $i.bak
    sed -e "s/_(\"\")/\"\"/g" $i.bak > $i
done
