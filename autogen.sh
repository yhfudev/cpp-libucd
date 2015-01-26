#!/bin/sh

./autoclean.sh

rm -f configure

rm -f Makefile.in

rm -f config.guess
rm -f config.sub
rm -f install-sh
rm -f missing
rm -f depcomp


touch README NEWS AUTHORS LICENSE config.h.in

#################
#autoscan
#################
libtoolize --copy --force
aclocal
automake -a -c
autoconf

#./configure --enable-static --enable-shared --prefix=/usr --disable-icu
./configure --enable-static --disable-shared --enable-debug --prefix=/usr --enable-icu
make clean
make ChangeLog
make dist
cp lib*-*.tar.gz pacman
#make
#make distclean

