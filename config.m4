PHP_ARG_ENABLE([proctitle],
  [whether to enable proctitle support],
  [AS_HELP_STRING([--enable-proctitle],
    [Enable proctitle support])],
  [no])

if test "$PHP_PROCTITLE" != "no"; then
  AC_MSG_CHECKING([for prctl])
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/prctl.h>]], [[prctl(0, 0, 0, 0, 0);]])], [
    AC_DEFINE([HAVE_PRCTL], 1, [do we have prctl?])
    AC_MSG_RESULT([yes])
  ], [
    AC_MSG_RESULT([no])
  ])

  AC_ARG_WITH([libbsd],
    AS_HELP_STRING(
      [--with-libbsd],
      [Use libbsd @<:@default=auto@:>@]),
    [],
    [with_libbsd=auto])
  if test x"$with_libbsd" != x"no"; then
    PKG_CHECK_MODULES([libbsd], [libbsd-overlay], [
      _save_CFLAGS="$CFLAGS"
      _save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $libbsd_CFLAGS"
      LIBS="$LIBS $libbsd_LIBS"
      LDFLAGS="$LDFLAGS $libbsd_LDFLAGS"
      AC_MSG_CHECKING([if libbsd can be linked correctly])
      AC_TRY_LINK([
    @%:@include <sys/time.h>
    @%:@include <sys/types.h>
    ],[],
        [
          AC_MSG_RESULT(yes)
          PHP_EVAL_INCLINE($libbsd_CFLAGS)
          PHP_EVAL_LIBLINE($libbsd_LIBS, PROCTITLE_SHARED_LIBADD)
          AC_DEFINE([HAVE_SETPROCTITLE], 1, [do we have libbsd setproctitle?])
        ],[
          AC_MSG_RESULT(no)
          CFLAGS="$_save_CFLAGS"
          LIBS="$_save_LIBS"
          if test x"$with_libbsd" = x"yes"; then
            AC_MSG_FAILURE([*** no libbsd support found])
          fi
        ])
    ], [
      if test x"$with_libbsd" = x"yes"; then
        AC_MSG_FAILURE([*** no libbsd support found])
      fi
    ])
  fi

  PHP_SUBST(PROCTITLE_SHARED_LIBADD)
  PHP_NEW_EXTENSION(proctitle, proctitle.c, $ext_shared)
fi
