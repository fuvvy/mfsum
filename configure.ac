AC_INIT([mfsum], [0.1], [fuvvy-github@nym.hush.com],
             [mfsum], [http://github.com/fuvvy/mfsum])
AC_PREREQ([2.59])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([1.10 -Wall no-define foreign])
AC_PROG_CC([gcc cl cc])
AC_PROG_CC_STDC
AC_SEARCH_LIBS([dlsym], [dl dld], [], [
  AC_MSG_ERROR([unable to find the dlsym() function])
])
AC_CONFIG_HEADERS([config.h])
AC_PROG_CXX
LT_INIT([disable-static])
AC_SUBST([SO_VERSION], [1])
AC_SUBST([API_VERSION], [0.1])
AC_CONFIG_FILES([Makefile
		 mfsum-${API_VERSION}.pc:mfsum.pc.in])
AC_OUTPUT
