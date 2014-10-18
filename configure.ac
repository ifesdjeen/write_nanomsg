AC_INIT([write_nanomsg], [0.0.1])

# 'foreign' specifies not to create the default GNU-style project files,
# such as touch, NEWS, README, AUTHORS ...
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_MACRO_DIR([m4])

# Defaults make verbosity to V=0, so that make generates a prettier, less
# verbose output. To go back to normal comment out the line, or call make with
# the 'V=1' argument to see a normal output.
AM_SILENT_RULES([yes])

AM_PROG_AR
LT_INIT
AC_PROG_CC
AM_PROG_LIBTOOL

AC_CONFIG_HEADERS([src/config.h])

dnl AC_CONFIG_FILES([
dnl     Makefile
dnl     src/Makefile
dnl ])

PKG_CHECK_MODULES([NANOMSG], [libnanomsg],, AC_MSG_ERROR([libnanomsg is required.]))

AC_C_CONST

AC_CHECK_FUNCS([strcasecmp strspn strstr])

AC_OUTPUT(Makefile src/Makefile)
