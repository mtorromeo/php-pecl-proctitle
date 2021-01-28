/*
  +----------------------------------------------------------------------+
  | PHP Version 5 / proctitle                                            |
  +----------------------------------------------------------------------+
  | Copyright (c) 2005-2009 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available at through the world-wide-web at                           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>
#include "ext/standard/info.h"
#include <SAPI.h>
#include <dlfcn.h>
#include <string.h>

#ifdef HAVE_PRCTL
#include <sys/prctl.h>
#endif

#ifdef HAVE_SETPROCTITLE
#include <bsd/unistd.h>
#endif

#include "php_proctitle.h"

static char *argv0 = NULL;

#ifndef HAVE_SETPROCTITLE
#define MAX_TITLE_LENGTH 128

static void setproctitle(char *title, int tlen)
{
	char    buffer[MAX_TITLE_LENGTH];

	if(!argv0) {
		return; /* no point running all this if we got no argv0 */
	}

	/* space padding */
	memset(buffer, 0x20, MAX_TITLE_LENGTH);
	buffer[MAX_TITLE_LENGTH-1] = '\0';

	/* title too long => truncate */
	if (tlen >= (MAX_TITLE_LENGTH-1)) tlen = (MAX_TITLE_LENGTH-1);

	memcpy(buffer, title, tlen);

	snprintf(argv0, MAX_TITLE_LENGTH, "%s", buffer);
}

PHP_MINIT_FUNCTION(proctitle)
{
	sapi_module_struct *symbol = NULL;

	symbol = &sapi_module;

	if( symbol )
		argv0 = symbol->executable_location;
}
#endif

ZEND_BEGIN_ARG_INFO_EX(arginfo_title, 0, 0, 1)
    ZEND_ARG_INFO(0, title)
ZEND_END_ARG_INFO()

/*
 * Changes the current process' title in system's list of processes
 */
PHP_FUNCTION(setproctitle)
{
	char *title;
	long tlen;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &title, &tlen) == FAILURE) {
		RETURN_NULL();
	}

#ifndef HAVE_SETPROCTITLE
	/* local (incompatible) setproctitle */
	setproctitle(title, tlen);
#else
	/* let's use system setproctitle() (BSD or compatible) */
	setproctitle("%s", title);
#endif
}

#if HAVE_PRCTL
/* Sets the thread name */
PHP_FUNCTION(setthreadtitle)
{
	char *title;
	int tlen;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &title, &tlen) == FAILURE) {
		RETURN_NULL();
	}

	if (0 == prctl(PR_SET_NAME, title, 0, 0, 0)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}
#endif

/*
 * Every user visible function must have an entry in proctitle_functions[].
 */
static const zend_function_entry proctitle_functions[] = {
	PHP_FE(setproctitle, arginfo_title)
#if HAVE_PRCTL
	PHP_FE(setthreadtitle, arginfo_title)
#endif
	{NULL, NULL, NULL}
};

PHP_MINFO_FUNCTION(proctitle)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "proctitle support", "enabled");
	php_info_print_table_end();
}

zend_module_entry proctitle_module_entry = {
	STANDARD_MODULE_HEADER,
	"proctitle",
	proctitle_functions,
#ifndef HAVE_SETPROCTITLE
	PHP_MINIT(proctitle),
#else
	NULL,
#endif
	NULL,
	NULL,
	NULL,
	PHP_MINFO(proctitle),
	PHP_PROCTITLE_VERSION,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_PROCTITLE
ZEND_GET_MODULE(proctitle)
#endif
