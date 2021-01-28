/* proctitle extension for PHP */

#ifndef PHP_PROCTITLE_H
# define PHP_PROCTITLE_H

extern zend_module_entry proctitle_module_entry;
# define phpext_proctitle_ptr &proctitle_module_entry

# define PHP_PROCTITLE_VERSION "@pear_version@"

# if defined(ZTS) && defined(COMPILE_DL_PROCTITLE)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_PROCTITLE_H */
