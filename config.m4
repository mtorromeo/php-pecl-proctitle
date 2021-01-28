PHP_ARG_ENABLE(proctitle, whether to enable proctitle support,
	[  --enable-proctitle      Enable proctitle support])

if test "$PHP_PROCTITLE" != "no"; then
	AC_MSG_CHECKING([for prctl])
	AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/prctl.h>]], [[prctl(0, 0, 0, 0, 0);]])], [
		AC_DEFINE([HAVE_PRCTL], 1, [do we have prctl?])
		AC_MSG_RESULT([yes])
	], [
		AC_MSG_RESULT([no])
	])

	AC_MSG_CHECKING([for libbsd setproctitle])
	AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <bsd/unistd.h>]], [[setproctitle("%s", "");]])], [
		AC_DEFINE([HAVE_SETPROCTITLE], 1, [do we have libbsd setproctitle?])
		AC_MSG_RESULT([yes])
	], [
		AC_MSG_RESULT([no])
	])

	PHP_NEW_EXTENSION(proctitle, proctitle.c, $ext_shared)
fi
