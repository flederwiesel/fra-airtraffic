#!/bin/bash

if ! [[ "$(uname -s)" =~ "MINGW" ]]; then
	echo "This file is intended to be run in a MinGW shell, however $(uname -s) was found." >&2
	exit 1
fi

for arg
do
	case "$arg" in
	--clean)
		clean=1
		;;
	*)
		args[${#args}]="$arg"
		;;
	esac
done

set -- "${args[@]}"

# As msw makefiles do not provide ability to override gcc prefix,
# make sure we will not find cygwin gcc...
export PATH=$(echo $PATH | sed -r 's#^:*[^:]*/cygwin[^:]+:*|:+[^:]*/cygwin[^:]+|[^:]*/cygwin[^:]+:+|[^:]*/cygwin[^:]+:*$##g')

scriptdir=$(dirname "${BASH_SOURCE[0]}")
cd "$scriptdir/wxWidgets"

[ -n "$clean" ] && rm -rf build/msw/gcc_* lib/gcc_*

# https://trac.wxwidgets.org/ticket/17844
if ! [ -f patch-stamp ]; then
	patch -p1 <<"EOF" &&
diff -raupN a/build/bakefiles/wx.bkl b/build/bakefiles/wx.bkl
--- a/build/bakefiles/wx.bkl	2018-02-19 18:56:24.069159900 +0100
+++ b/build/bakefiles/wx.bkl	2018-02-19 18:59:33.623001800 +0100
@@ -205,7 +205,7 @@
             <depends-on-file>$(SRCDIR)/include/wx/msw/genrcdefs.h</depends-on-file>

             <command>
-                $(DOLLAR)(CPP) "$(nativePaths(SRCDIR))\include\wx\msw\genrcdefs.h" > "$(SETUPHDIR)\wx\msw\rcdefs.h"
+                $(DOLLAR)(CPP) $(DOLLAR)(CPPFLAGS) "$(nativePaths(SRCDIR))\include\wx\msw\genrcdefs.h" > "$(SETUPHDIR)\wx\msw\rcdefs.h"
             </command>

         </action>
EOF
	touch patch-stamp
fi

[ -n "$clean" ] && rm -rf build/msw/gcc_* lib/gcc_* include/wx/msw/setup.h

cd build

# After changing bakefiles, we need to re-generate make/project files
(
	cd bakefiles
	bakefile_gen
) || exit 1

cd msw

export OFFICIAL_BUILD=0
export VENDOR=flederwiesel
export MONOLITHIC=0
export UNICODE=1

for current in Debug Release Release32 Release64 # Debug-Static Release-Static
do
	case "${current%%-Static}" in
	Debug)
		infix=/mswud
		export BUILD=debug
		export SHARED=1
		export CXXFLAGS='-fno-keep-inline-dllexport -std=gnu++11'
		;;
	Debug32)
		infix=32/mswud
		export CFG=32
		export BUILD=debug
		export SHARED=1
		export CPPFLAGS=-m32
		export CXXFLAGS='-fno-keep-inline-dllexport -std=gnu++11'
		export LDFLAGS=-m32
		export WINDRES='windres --output-format=coff --target=pe-i386'
		;;
	Debug64)
		infix=64/mswud
		export CFG=64
		export BUILD=debug
		export SHARED=1
		export CXXFLAGS='-fno-keep-inline-dllexport -std=gnu++11'
		;;
	Release)
		infix=/mswu
		export BUILD=release
		export SHARED=1
		export CXXFLAGS='-fno-keep-inline-dllexport -std=gnu++11'
		;;
	Release32)
		infix=32/mswu
		export CFG=32
		export BUILD=release
		export SHARED=1
		export CPPFLAGS=-m32
		export CXXFLAGS='-fno-keep-inline-dllexport -std=gnu++11'
		export LDFLAGS=-m32
		export WINDRES='windres --output-format=coff --target=pe-i386'
		;;
	Release64)
		infix=64/mswu
		export CFG=64
		export BUILD=release
		export SHARED=1
		export CXXFLAGS='-fno-keep-inline-dllexport -std=gnu++11'
		;;
	RelWithDebInfo)
		infix=/mswud
		export BUILD=release
		export SHARED=1
		export DEBUG_INFO=1
		export CXXFLAGS='-fno-keep-inline-dllexport -std=gnu++11'
		;;
	esac

	if [ "Static" = "${current##*-}" ]; then
		export SHARED=0
		export RUNTIME_LIBS=static
	fi

	# As makefile.gcc seems to have dependency problems when using parallel build,
	# build setup.h - which everything else depends on - beforehand
	mingw32-make -f makefile.gcc setup_h
	mingw32-make -f makefile.gcc -j 8

	unset infix
	unset CFG
	unset BUILD
	unset SHARED
	unset DEBUG_INFO
	unset RUNTIME_LIBS
	unset CPPFLAGS
	unset CXXFLAGS
	unset LDFLAGS
	unset WINDRES
done
