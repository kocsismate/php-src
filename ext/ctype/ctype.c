/*
   +----------------------------------------------------------------------+
   | Copyright (c) The PHP Group                                          |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Hartmut Holzgraefe <hholzgra@php.net>                        |
   +----------------------------------------------------------------------+
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "php_ctype.h"
#include "ctype_arginfo.h"
#include "SAPI.h"
#include "ext/standard/info.h"

#include <ctype.h>

#ifdef HAVE_CTYPE

static PHP_MINFO_FUNCTION(ctype);

/* }}} */

/* {{{ ctype_module_entry */
zend_module_entry ctype_module_entry = {
	STANDARD_MODULE_HEADER,
	"ctype",
	ext_functions,
	NULL,
	NULL,
	NULL,
	NULL,
	PHP_MINFO(ctype),
    PHP_CTYPE_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_CTYPE
ZEND_GET_MODULE(ctype)
#endif

/* {{{ PHP_MINFO_FUNCTION */
static PHP_MINFO_FUNCTION(ctype)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "ctype functions", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ ctype */
#define CTYPE(iswhat, allow_digits, allow_minus) \
	zend_string *c_str; \
	zend_long c_long; \
	ZEND_PARSE_PARAMETERS_START(1, 1); \
		Z_PARAM_STR_OR_LONG(c_str, c_long) \
	ZEND_PARSE_PARAMETERS_END(); \
	if (c_str) { \
		char *p = ZSTR_VAL(c_str), *e = ZSTR_VAL(c_str) + ZSTR_LEN(c_str); \
		if (e == p) {	\
			RETURN_FALSE;	\
		}	\
		while (p < e) { \
			if(!iswhat((int)*(unsigned char *)(p++))) { \
				RETURN_FALSE; \
			} \
		} \
		RETURN_TRUE; \
	} else { \
		if (c_long <= 255 && c_long >= 0) { \
			RETURN_BOOL(iswhat((int) c_long)); \
		} else if (c_long >= -128 && c_long < 0) { \
			RETURN_BOOL(iswhat((int) c_long + 256)); \
		} else if (c_long >= 0) { \
			RETURN_BOOL(allow_digits); \
		} else { \
			RETURN_BOOL(allow_minus); \
		} \
	}

/* }}} */

/* {{{ Checks for alphanumeric character(s) */
PHP_FUNCTION(ctype_alnum)
{
	CTYPE(isalnum, 1, 0);
}
/* }}} */

/* {{{ Checks for alphabetic character(s) */
PHP_FUNCTION(ctype_alpha)
{
	CTYPE(isalpha, 0, 0);
}
/* }}} */

/* {{{ Checks for control character(s) */
PHP_FUNCTION(ctype_cntrl)
{
	CTYPE(iscntrl, 0, 0);
}
/* }}} */

/* {{{ Checks for numeric character(s) */
PHP_FUNCTION(ctype_digit)
{
	CTYPE(isdigit, 1, 0);
}
/* }}} */

/* {{{ Checks for lowercase character(s)  */
PHP_FUNCTION(ctype_lower)
{
	CTYPE(islower, 0, 0);
}
/* }}} */

/* {{{ Checks for any printable character(s) except space */
PHP_FUNCTION(ctype_graph)
{
	CTYPE(isgraph, 1, 1);
}
/* }}} */

/* {{{ Checks for printable character(s) */
PHP_FUNCTION(ctype_print)
{
	CTYPE(isprint, 1, 1);
}
/* }}} */

/* {{{ Checks for any printable character which is not whitespace or an alphanumeric character */
PHP_FUNCTION(ctype_punct)
{
	CTYPE(ispunct, 0, 0);
}
/* }}} */

/* {{{ Checks for whitespace character(s)*/
PHP_FUNCTION(ctype_space)
{
	CTYPE(isspace, 0, 0);
}
/* }}} */

/* {{{ Checks for uppercase character(s) */
PHP_FUNCTION(ctype_upper)
{
	CTYPE(isupper, 0, 0);
}
/* }}} */

/* {{{ Checks for character(s) representing a hexadecimal digit */
PHP_FUNCTION(ctype_xdigit)
{
	CTYPE(isxdigit, 1, 0);
}
/* }}} */

#endif	/* HAVE_CTYPE */
