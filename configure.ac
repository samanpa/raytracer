AC_INIT(bpray, 0.0.1)

#to get the host, we do this early or we get a warning
AC_CANONICAL_SYSTEM

AC_CONFIG_SRCDIR(src/banna.cpp)
AM_CONFIG_HEADER(src/config.h)
AM_INIT_AUTOMAKE

AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CXX
AC_PROG_RANLIB
AM_PROG_LEX
AC_PROG_YACC

#need to check to see if we are building on mingw and set flags
#     for future stages so that right ld will be chosen
case "$host" in
     *-*-mingw*)
        CC="$CC -mno-cygwin"
        platform_win32=yes
        AC_DEFINE(PLATFORM_WIN32,1,[Platform is Win32])
        ;;
     *-*-linux*)
        AC_DEFINE(PLATFORM_LINUX,1,[Platform is linux])
        ;;
esac

case "$host" in
     *-*-mingw*)
          #we already have a porperly built mingw dll in the lib directory
          #we don't use AC_CHECK_LIB to find glu. opengl32 (a microsoft supplied library)
          #  cos they are defined __stdcall and so AC_CHECK_LIB willl fail
          #  FIX: --enable-stdcall-fixup might fix this
          LIBS="$LIBS -lgd -lopengl32 -lglu32"
        ;;
     *)
        AC_CHECK_LIB(gd, gdFontGetGiant, [], [
                AC_MSG_ERROR([Error! You need to have libgd around.])
        ])
        AC_CHECK_LIB(GL, glMatrixMode, [], [
                AC_MSG_ERROR([Error! You need to have opengl around.])
        ])
        AC_CHECK_LIB(GLU, gluOrtho2D, [], [
                AC_MSG_ERROR([Error! You need to have glu around.])
        ])
        ;;
esac

AC_PATH_PROG(PKG_CONFIG, [pkg-config], no)
if test "x$PKG_CONFIG" = "xno"; then
   AC_MSG_ERROR([You need to install pkg-config])
fi

pkg-config --atleast-version=2.8 glib-2.0
if [[ "x$?" != "x0" ]] ; then
      AC_MSG_ERROR([You need to have glib installed.])
fi

AC_CHECK_LIB(pthread, pthread_mutex_init,
,
        [
                AC_MSG_ERROR([You need to have pthreads installed.])
        ]
)

CFLAGS="$(pkg-config  --cflags glib-2.0) $CFLAGS"
LDFLAGS="$(pkg-config --libs glib-2.0) $LDFLAGS"

AC_OUTPUT([
Makefile src/Makefile doc/Makefile tests/Makefile
])

echo "
configuration result
      LDFLAGS   : $LDFLAGS
      CFLAGS    : $CFLAGS
      LIBS      : $LIBS
"

