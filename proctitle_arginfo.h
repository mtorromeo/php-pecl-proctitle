/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: a9a2d8933bc19e0b497535aa63fe96d901628100 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_setproctitle, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, title, IS_STRING, 0)
ZEND_END_ARG_INFO()

#if defined(HAVE_PRCTL)
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_setthreadtitle, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, title, IS_STRING, 0)
ZEND_END_ARG_INFO()
#endif


ZEND_FUNCTION(setproctitle);
#if defined(HAVE_PRCTL)
ZEND_FUNCTION(setthreadtitle);
#endif


static const zend_function_entry ext_functions[] = {
	ZEND_FE(setproctitle, arginfo_setproctitle)
#if defined(HAVE_PRCTL)
	ZEND_FE(setthreadtitle, arginfo_setthreadtitle)
#endif
	ZEND_FE_END
};
