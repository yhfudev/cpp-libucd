#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(libucd, 1.0.0, https://github.com/yhfudev/cpp-libucd.git)
AC_CONFIG_SRCDIR([utils/ucd.c])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE(-Wall subdir-objects foreign color-tests dist-bzip2 parallel-tests)

#magic for conditional check in Makefile:
MK=''; AC_SUBST(MK)
SED=sed

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

# Checks for libraries.
LT_INIT([shared static])

#AC_CHECK_LIB([dl], [dlopen])
AC_PROG_RANLIB

#PKG_CHECK_EXISTS(icu,[
        #PKG_CHECK_MODULES([LIBICU0], [icu])
    #], [
        #PKG_CHECK_MODULES([LIBICU1], [icu-i18n])
    #])
#AM_CONDITIONAL(ICU_IS_LONG, [test x"$LIBICU0_CFLAGS" = x])

AM_CONDITIONAL(USE_ICU, [test x = y])
AC_ARG_ENABLE([icu],[AS_HELP_STRING([--disable-icu],[Disable ICU (no comparision function in ucd)])])
AS_IF([test "x$enable_icu" != "xno"],[
    PKG_CHECK_MODULES([ICU],[icu-i18n],[
      AC_DEFINE([LCF_SUPPORT_ICU],[1],[Enable ICU support])
      AM_CONDITIONAL(USE_ICU, [test x = x])
      ])
])


AC_ARG_WITH([iconv],
        AC_HELP_STRING([--with-iconv],
                [Use the libiconv (default=no)]),[
	with_iconv=$withval
],[
	with_iconv=no
])

have_iconv=no
# if no iconv, then icu ...
if test x$with_iconv = xno; then
    echo ""
else
    LIBICONV_INCLUDEDIR=""
    for d in \
        $with_iconv/include \
        $prefix/include \
        /usr/include
    do
        if test -f "$d/iconv.h" ; then
	    LIBICONV_INCLUDEDIR="$d"
	    break
        fi
    done
    if test "x$LIBICONV_INCLUDEDIR" = "x"; then
        echo "checking for iconv.h...no"
        have_iconv=no

    else
        echo "checking for ucnv_err.h...yes"
        have_iconv=yes
        AM_CONDITIONAL([USE_ICONV], [test $have_iconv = "yes"])
        # AC_DEFINE(USE_ICONV,1,[Defined when iconv can be used.])
    fi
fi

if test x$have_iconv = xno; then
    AM_CONDITIONAL([USE_ICONV], [test $have_iconv = "yes"])
    LIBICU_INCLUDEDIR=""
    for d in 			\
        /usr/include/unicode \
        /usr/include/x86_64-linux-gnu/unicode \
        /usr/include/i386-linux-gnu/unicode \
        $prefix/include/unicode \
        /usr/include
    do
        if test -f "$d/ucnv_err.h" ; then
          LIBICU_INCLUDEDIR="$d"
          break
        fi
    done
    if test "x$LIBICU_INCLUDEDIR" = "x"; then
        #AC_MSG_ERROR([You should have iconv or ICU header files! Install libicu-dev])
        have_iconv=no
    else
        echo "checking for ucnv_err.h...yes"
        have_iconv=yes
    fi
fi
AM_CONDITIONAL([BUILD_WITH_ICULIB], [test "$have_iconv" = "yes"])


# debug
AC_ARG_ENABLE([debug],
	AS_HELP_STRING([--enable-debug],[Compile the debug version (default: disabled)]),
	[enable_debug=$enableval],
	[enable_debug=no])
AM_CONDITIONAL([DEBUG], [test $enable_debug = "yes"])
if test "x$enable_debug" = "xyes"; then
  changequote({,})
  CFLAGS=`echo "$CFLAGS" | $SED -e 's/-O[0-9s]*//g'`
  CXXFLAGS=`echo "$CXXFLAGS" | $SED -e 's/-O[0-9s]*//g'`
  changequote([,])
  dnl add -O0 only if GCC or ICC is used
  if test "$GCC" = "yes" || test "$ICC" = "yes"; then
    CFLAGS="$CFLAGS -O0"
    CXXFLAGS="$CXXFLAGS -O0"
  fi
else
  changequote({,})
  CFLAGS=`echo "$CFLAGS" | $SED -e 's/-g//g'`
  CXXFLAGS=`echo "$CXXFLAGS" | $SED -e 's/-g//g'`
  changequote([,])
fi

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stddef.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strdup])

AC_OUTPUT(Makefile
	src/Makefile
	pkgconfig/Makefile
	pkgconfig/libucd.pc
	rpm/libucd.spec
	utils/Makefile
)
AC_OUTPUT
