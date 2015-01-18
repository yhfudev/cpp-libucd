#!/bin/sh

./autoclean.sh

rm -f configure

rm -f Makefile.in

rm -f config.guess
rm -f config.sub
rm -f install-sh
rm -f missing
rm -f depcomp


touch README NEWS AUTHORS ChangeLog config.h.in

#################
#autoscan
#################
libtoolize --copy --force
aclocal
automake -a -c
autoconf

./configure --enable-static --enable-shared --enable-debug --prefix=`pwd`/aaa --disable-icu
make distclean
#make

