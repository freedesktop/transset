#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.57)
AC_INIT([transset], [0.9.1], [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg])
AC_CONFIG_SRCDIR([transSet.c])
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE([dist-bzip2 foreign])
AM_MAINTAINER_MODE

# Require xorg-macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
	  [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

# Checks for library functions.
PKG_CHECK_MODULES(TRANSSET, x11)

AC_CONFIG_FILES([
	Makefile
	man/Makefile])
AC_OUTPUT
