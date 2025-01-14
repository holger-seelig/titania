dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(titania, 0.1)

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([subdir-objects])

AM_SILENT_RULES([yes])

AM_MAINTAINER_MODE

AC_PROG_INSTALL

AC_PROG_CXX



dnl ***************************************************************************
dnl Configurations
dnl ***************************************************************************

AC_ARG_ENABLE(debug,
	AC_HELP_STRING([--enable-debug],
		[Use 'DEBUG' Configuration [default=NO]]),
		enable_debug=yes, enable_debug=no)

AC_ARG_ENABLE(feature,
	AC_HELP_STRING([--enable-feature],
		[Enable experimental features [default=NO]]),
		enable_feature=yes, enable_feature=no)

AC_ARG_ENABLE(flatpak_spawn,
	AC_HELP_STRING([--enable-flatpak-spawn],
		[Enable flatpak spawn [default=NO]]),
		enable_flatpak_spawn=yes, enable_flatpak_spawn=no)


dnl ***************************************************************************
dnl Compile options
dnl ***************************************************************************

dnl ******* add "-flto --plugin=$(gcc --print-file-name=liblto_plugin.so)" flags (link time optimization) to CXXFLAGS or LDFLAGS if it is working
dnl ******* CXXFLAGS=$CXXFLAGS" -DTITANIA_DEBUG -Wno-deprecated-declarations"

CXXFLAGS="-std=c++17 -O3 -Wall -Wno-narrowing -Wno-deprecated -Wno-unused-result"

### add GTKMM_DISABLE_DEPRECATED

if test "x$enable_debug" = "xyes" ; then
	CXXFLAGS=$CXXFLAGS" -DTITANIA_DEBUG"
fi

if test "x$enable_feature" = "xyes" ; then
	CXXFLAGS=$CXXFLAGS" -DTITANIA_FEATURE"
fi

if test "x$enable_flatpak_spawn" = "xyes" ; then
	CXXFLAGS=$CXXFLAGS" -DTITANIA_FLATPAK_SPAWN"
fi

LDFLAGS="-rdynamic"

if uname -s | grep -q "Darwin"; then
   export PKG_CONFIG_PATH="/opt/X11/lib/pkgconfig:"`pwd`/brew/mozjs/lib/pkgconfig:`pwd`/brew/gstreamermm/lib/pkgconfig
	CXXFLAGS=$CXXFLAGS" -static"
	CXXFLAGS=$CXXFLAGS" -I/usr/local/include"
	CXXFLAGS=$CXXFLAGS" -D_LIBCPP_ENABLE_CXX17_REMOVED_AUTO_PTR -D_LIBCPP_ENABLE_CXX17_REMOVED_FEATURES -DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED"
	LDFLAGS=$LDFLAGS" -L/usr/local/lib -lomp -framework OpenGL"
else
	CXXFLAGS=$CXXFLAGS" -latomic -fopenmp"
fi



dnl ***************************************************************************
dnl Internationalization
dnl ***************************************************************************

IT_PROG_INTLTOOL([0.35.0])

GETTEXT_PACKAGE=titania
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])
AM_GLIB_GNU_GETTEXT

LT_INIT



dnl ***************************************************************************
dnl Check for C++17 compiler support
dnl ***************************************************************************

if test x$ax_cxx_compile_cxx17_required = xtrue; then
	if test x$ac_success = xno; then
		AC_MSG_ERROR([*** A compiler with support for C++17 language features is required.])
	fi
fi



dnl ***************************************************************************
dnl Check for GLX
dnl ***************************************************************************

USE_GLX="no"
PKG_CHECK_MODULES([GLX], [gl x11], [USE_GLX="yes"], [:])
AC_CHECK_HEADERS([GL/gl.h GL/glx.h], [:], [USE_GLX="no"])
AC_CHECK_LIB([GL], [glXCreateContext], [:] [USE_GLX="no"])
AM_CONDITIONAL(USE_GLX, test "$USE_GLX" = "yes")

if ! uname -s | grep -q "Darwin"; then
   if test "$USE_GLX" = "no"; then
      AC_MSG_ERROR([*** Please install X11 backend.])
   fi
fi


dnl ***************************************************************************
dnl Check required libraries that do not have a pkgconfig
dnl ***************************************************************************

# Does not work with FlatPak
#USE_CGAL="no"
#AC_CHECK_LIB(CGAL, _init, [USE_CGAL="yes"])
#if test "$USE_CGAL" = "no"; then
#	AC_MSG_ERROR([*** Please install CGAL library.])
#fi



dnl ***************************************************************************
dnl Check required libraries
dnl ***************************************************************************

PKG_CHECK_MODULES(TITANIA,
gtkmm-3.0
gtksourceviewmm-3.0)

PKG_CHECK_MODULES(TITANIA_X3D,
gtkmm-3.0
glibmm-2.4
gstreamermm-1.0
fontconfig
mozjs-68
libxml++-2.6
json-c
libgtop-2.0
bullet
x11)

if ! uname -s | grep -q "Darwin"; then
	PKG_CHECK_MODULES(TITANIA_GL,
	gl
	glu
	glew)
fi

PKG_CHECK_MODULES(TITANIA_FTGL,
freetype2
fontconfig)

PKG_CHECK_MODULES(TITANIA_STREAM,
libcurl
giomm-2.4)

PKG_CHECK_MODULES(TITANIA_SQLITE,
sqlite3)

PKG_CHECK_MODULES(TITANIA_STANDARD,
giomm-2.4
glibmm-2.4
zlib)

PKG_CHECK_MODULES(TITANIA_EXPERIMENTAL,
gtkmm-3.0)


AC_OUTPUT([
libtitania-standard/Makefile
libtitania-sqlite/Makefile
libtitania-stream/Makefile
libtitania-ftgl/Makefile
libtitania-x3d/Makefile
Titania/Makefile
experimental/Makefile
po/Makefile.in
Makefile
])
