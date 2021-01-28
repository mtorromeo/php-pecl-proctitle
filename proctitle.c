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

#include <SAPI.h>
#include <php.h>
#include "ext/standard/info.h"
#include "php_proctitle.h"
#include "proctitle_arginfo.h"

#ifdef HAVE_PRCTL
#include <sys/prctl.h>
#endif

#ifdef HAVE_SETPROCTITLE
#include <bsd/unistd.h>
#endif

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

static char *argv0 = NULL;

#ifndef HAVE_SETPROCTITLE
#define MAX_TITLE_LENGTH 128

static void setproctitle(char *title, int tlen)
{
	char buffer[MAX_TITLE_LENGTH];

	if (!argv0)
	{
		return; /* no point running all this if we got no argv0 */
	}

	/* space padding */
	memset(buffer, 0x20, MAX_TITLE_LENGTH);
	buffer[MAX_TITLE_LENGTH - 1] = '\0';

	/* title too long => truncate */
	if (tlen >= (MAX_TITLE_LENGTH - 1))
	{
		tlen = (MAX_TITLE_LENGTH - 1);
	}

	memcpy(buffer, title, tlen);

	snprintf(argv0, MAX_TITLE_LENGTH, "%s", buffer);
}

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(proctitle)
{
	sapi_module_struct *symbol = NULL;

	symbol = &sapi_module;

	if (symbol)
	{
		argv0 = symbol->executable_location;
	}
}
/* }}} */
#endif

/* {{{ void setproctitle( string $title ) */
PHP_FUNCTION(setproctitle)
{
	char *title;
	size_t title_len;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_STRING(title, title_len)
	ZEND_PARSE_PARAMETERS_END();

#ifndef HAVE_SETPROCTITLE
	/* local (incompatible) setproctitle */
	setproctitle(title, title_len);
#else
	/* let's use system setproctitle() (BSD or compatible) */
	setproctitle("%s", title);
#endif
/* }}}*/
}

#if HAVE_PRCTL
/* {{{ bool setthreadtitle( string $title ) */
PHP_FUNCTION(setthreadtitle)
{
	char *title;
	size_t title_len;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_STRING(title, title_len)
	ZEND_PARSE_PARAMETERS_END();

	if (0 == prctl(PR_SET_NAME, title, 0, 0, 0))
	{
		RETURN_TRUE;
	}
	else
	{
		RETURN_FALSE;
	}
}
/* }}}*/
#endif

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(proctitle)
{
#if defined(ZTS) && defined(COMPILE_DL_PROCTITLE)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(proctitle)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "proctitle support", "enabled");
	php_info_print_table_row(2, "Version", PHP_PROCTITLE_VERSION);
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */

/* {{{ proctitle_module_entry */
zend_module_entry proctitle_module_entry = {
	STANDARD_MODULE_HEADER,
	"proctitle",					/* Extension name */
	ext_functions,					/* zend_function_entry */
#ifndef HAVE_SETPROCTITLE
	PHP_MINIT(proctitle),			/* PHP_MINIT - Module initialization */
#else
	NULL,							/* PHP_MINIT - Module initialization */
#endif
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(proctitle),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(proctitle),			/* PHP_MINFO - Module info */
	PHP_PROCTITLE_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_PROCTITLE
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(proctitle)
#endif
