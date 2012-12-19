#!/bin/bash

autogen_CMDLINE="$autogen_CMDLINE --prefix=$HOME/system/"
CFLAGS="-I$HOME/system/include $CFLAGS"
CPPFLAGS="-I$HOME/system/include $CPPFLAGS"
LDFLAGS="-L$HOME/system/lib $LDFLAGS"

for param in $@; do
    case $param in
	windows) 
	    autogen_CMDLINE="$autogen_CMDLINE --build=i686-pc-mingw32 "
            #we add  -lgdi32 -lcomdlg32 which are added when gcc is run with the
            #   -mwindows parameter. We don't use -mwindows flag because we want
            #   to build a console application
            export LDFLAGS="-L`pwd`/lib/i686-pc-mingw32  -lgdi32 -lcomdlg32"
	    ;;
	*)
	    ;;
    esac
done

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

echo "running: aclocal"
aclocal

echo "running: automake --add-missing"
automake --add-missing

echo "running: autoheader"
autoheader

echo "running: autoconf"
autoconf

echo "running: $srcdir/configure $*"
$srcdir/configure $*
