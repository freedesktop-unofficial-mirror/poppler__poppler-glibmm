## Copyright (c) 2010  Glenn Rice <glennricster@gmail.com>
##
## This file is part of poppler-glibmm.
##
## poppler-glibmm is free software: you can redistribute it and/or modify it
## under the terms of the GNU Lesser General Public License as published
## by the Free Software Foundation, either version 2.1 of the License,
## or (at your option) any later version.
##
## poppler-glibmm is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
## See the GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.

AC_INIT([poppler-glibmm], [0.21.1], [https://bugs.freedesktop.org/enter_bug.cgi?product=poppler],
		[poppler-glibmm], [http://poppler.freedesktop.org/])
AC_PREREQ([2.59])

AC_CONFIG_SRCDIR([poppler-glib/poppler-glibmm.h])
AC_CONFIG_AUX_DIR([build])
AC_CONFIG_MACRO_DIR([build])
AC_CONFIG_HEADERS([build/config.h poppler-glib/poppler-glibmmconfig.h])

AM_INIT_AUTOMAKE([1.9 -Wno-portability check-news dist-bzip2 no-define nostdinc tar-pax])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES])
AM_MAINTAINER_MODE
AC_ARG_VAR([ACLOCAL_FLAGS], [aclocal flags, e.g. -I <macro dir>])

MM_PREREQ([0.9])
MM_INIT_MODULE([poppler-glibmm-1.0])

# Copy the mm-common .pl scripts into doc/, and use them from there,
# so we can dist them to avoid a tarball-build dependency.
MM_CONFIG_DOCTOOL_DIR([doc])

# http://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
AC_SUBST([POPPLER_GLIBMM_SO_VERSION], [1:0:1])

AC_PROG_CXX
AC_DISABLE_STATIC
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL


AC_SUBST([POPPLER_GLIBMM_MODULES], ['poppler-glib >= 0.21 glibmm-2.4 >= 2.33 cairomm-1.0 >= 1.10 giomm-2.4 >= 2.33'])
PKG_CHECK_MODULES([POPPLER_GLIBMM], [$POPPLER_GLIBMM_MODULES])

# This is only needed for the demos
AC_SUBST([GTKMM_MODULES], ['gtkmm-3.0 >= 3.5.0'])
PKG_CHECK_MODULES([GTKMM], [$GTKMM_MODULES])

MM_PKG_CONFIG_SUBST([GTHREAD_CFLAGS], [--cflags-only-other gthread-2.0])
MM_PKG_CONFIG_SUBST([GMMPROC_DIR], [--variable=gmmprocdir glibmm-2.4])

MM_ARG_ENABLE_DOCUMENTATION
MM_ARG_WITH_TAGFILE_DOC([libstdc++.tag], [mm-common-libstdc++])
MM_ARG_WITH_TAGFILE_DOC([libsigc++-2.0.tag], [sigc++-2.0])
MM_ARG_WITH_TAGFILE_DOC([glibmm-2.4.tag], [glibmm-2.4])
MM_ARG_WITH_TAGFILE_DOC([cairomm-1.0.tag], [cairomm-1.0])

AC_LANG([C++])
MM_ARG_ENABLE_WARNINGS([POPPLER_GLIBMM_WXXFLAGS],
                       [-Wall],
                       [-pedantic -Wall -Wextra -Wno-long-long],
                       [G POPPLER_GLIB GLIBMM CAIROMM GTKMM])

AC_CONFIG_FILES([Makefile
                 codegen/Makefile
                 poppler-glib/${POPPLER_GLIBMM_MODULE_NAME}.pc:poppler-glib/poppler-glibmm.pc.in
                 poppler-glib/${POPPLER_GLIBMM_MODULE_NAME}-uninstalled.pc:poppler-glib/poppler-glibmm-uninstalled.pc.in
                 poppler-glib/src/Makefile
                 poppler-glib/poppler-glibmm/Makefile
                 examples/Makefile
                 doc/Makefile
                 doc/reference/Doxyfile])
AC_OUTPUT
