AC_PREREQ(2.59)
AC_INIT(svlock, 0.01)

AC_CANONICAL_BUILD
AC_CANONICAL_TARGET
AC_CANONICAL_HOST

AM_INIT_AUTOMAKE([foreign])
AM_CONFIG_HEADER(config.h)
AC_CONFIG_MACRO_DIR([m4])

LT_INIT

AM_MAINTAINER_MODE

AC_PROG_CC
AC_STDC_HEADERS
AC_FUNC_STRERROR_R
AM_PROG_LIBTOOL
AM_PROG_AS

AC_CHECK_HEADERS(sys/io.h, ,)

AM_CONDITIONAL([COND_LINUX], [test x$host_os = xlinux-gnu || test x$host_os = xlinux-gnueabihf])
if test -z "${COND_LINUX_TRUE}"
then
   AC_DEFINE(HAVE_LINUX, 1, [Define if building on a Linux system])
fi
AM_CONDITIONAL([COND_CYGWIN], [test x$host_os = xcygwin])
if test -z "${COND_CYGWIN_TRUE}"
then
   AC_DEFINE(HAVE_CYGWIN, 1, [Define if building on a Cygwin system])
fi
AM_CONDITIONAL([COND_WIN32], [test x$host_os = xmingw32])
if test -z "${COND_WIN32_TRUE}"
then
   AC_DEFINE(HAVE_WIN32, 1, [Define if building on a Mingw32 system])
fi
AM_CONDITIONAL([COND_MACOSX], [test x$version_type = xdarwin])
if test -z "${COND_MACOSX_TRUE}"
then
   AC_DEFINE(HAVE_MACOSX, 1, [Define if building on a Max OSX system])
fi

AM_CONDITIONAL([COND_X86], [test x$host_cpu = xx86_64 || test x$host_cpu = xi386])
if test -z "${COND_X86_TRUE}"
then
   AC_DEFINE(HAVE_X86, 1, [Define if building on an Intel architecture])
fi


dnl

PKG_CHECK_MODULES([libxml2],[libxml-2.0])

if test "x$with_html_dir" = "x" ; then
  HTML_DIR='$(prefix)/share/gtk-doc/html'
else
  HTML_DIR=$with_html_dir
fi

if test "$with_ref_debug" = "yes" ; then
    echo Enabling reference counting debug support
    WITH_REF_DEBUG=1
else    
    WITH_REF_DEBUG=0
fi
AC_DEFINE([DEBUG_REFCNT],[],[Desciption])

AC_SUBST(libxml2_LIBS)
AC_SUBST(libxml2_CFLAGS)


dnl
dnl Find some kernel headers. We only use the ACPI headers, so the kernel
dnl version does not really matter
dnl
if test -z "${COND_LINUX_TRUE}"
then
	AC_MSG_CHECKING(for Linux kernel ACPI headers) 
	linux_kernel_src_dir=""

	for d in /usr/src/kernels/* /usr/src/linux-headers* /usr/src/linux
	do
		if test -f $d/include/acpi/platform/aclinux.h
		then
			linux_kernel_src_dir=$d
		fi
	done
	if test -n "$linux_kernel_src_dir"
	then
		echo "Did not find kernel source"
		AC_DEFINE(HAVE_ACPI_H, 1, [Define if ACPI headers are available])
	fi
fi


AC_SUBST(LINKERNDIR,$linux_kernel_src_dir)

dnl ===============
dnl Checks for SWIG
dnl ===============
AC_PATH_PROG([SWIG], [swig])
if test ! "$SWIG"
then
	AC_PATH_PROG([SWIG], [swig2.0])
fi
AM_CONDITIONAL([HAVE_SWIG], [test "$SWIG"])


dnl =================
dnl Checks for Python
dnl =================
AM_PATH_PYTHON(3)
dnl AM_CONDITIONAL([HAVE_PYTHON], [test "$PYTHON" != ":" -a "x$have_python_header" = "xtrue"])
AM_CONDITIONAL([HAVE_PYTHON], [test "$PYTHON" != ":"])


dnl check for doxygen
AC_ARG_WITH(docs,
  AS_HELP_STRING([--without-docs], [disable doxygen usage]))
if test "x$with_docs" != "xno"; then
  AC_PATH_PROG(DOXYGEN, doxygen)
else
  DOXYGEN=
fi
AM_CONDITIONAL(HAVE_DOXYGEN, test -n $DOXYGEN)

WIN32_EXTRA_LIBADD=
WIN32_EXTRA_LDFLAGS=
CYGWIN_EXTRA_LDFLAGS=
case "$host" in
 *-*-mingw*)
 CPPFLAGS="$CPPFLAGS -DWIN32 -D__USE_MINGW_ANSI_STDIO=1"
 WIN32_EXTRA_LIBADD="-lws2_32"
 WIN32_EXTRA_LDFLAGS="-no-undefined"
 AC_DEFINE([_WINSOCKAPI_],1,[Using the Win32 Socket implementation])
 ;;
 *-*-cygwin*)
 CYGWIN_EXTRA_LDFLAGS="-no-undefined"
 ;;
esac
AC_SUBST(WIN32_EXTRA_LIBADD)
AC_SUBST(WIN32_EXTRA_LDFLAGS)
AC_SUBST(CYGWIN_EXTRA_LDFLAGS)


dnl
dnl Create the files
dnl

AC_CONFIG_FILES([
		 Makefile
		 svlock.pc
		 svlock-config
		 doc/Doxyfile
		 doc/Makefile
		 svlock/Makefile
		 python/Makefile
		 python/setup.py
		 tests/Makefile
		 tests/svlock/Makefile
		 tests/svlock-cli/Makefile
		 utils/Makefile
		 utils/svlock-cli/Makefile
])


AC_OUTPUT
