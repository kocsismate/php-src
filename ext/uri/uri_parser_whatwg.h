/*
   +----------------------------------------------------------------------+
   | Copyright (c) The PHP Group                                          |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | https://www.php.net/license/3_01.txt                                 |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Máté Kocsis <kocsismate@php.net>                            |
   +----------------------------------------------------------------------+
*/

#ifndef PHP_URI_PARSER_WHATWG_H
#define PHP_URI_PARSER_WHATWG_H

#include "php_uri_common.h"
#include "lexbor/url/url.h"

PHPAPI extern const php_uri_parser php_uri_parser_whatwg;

typedef struct php_uri_parser_whatwg_url_query_params_object {
	lxb_url_search_params_t *query_params;
	bool is_initialized;
	zend_object std;
} php_uri_parser_whatwg_url_query_params_object;

#define Z_URL_QUERY_PARAMS_OBJECT_P(zv) php_uri_whatwg_url_query_params_object_from_obj(Z_OBJ_P((zv)))

static inline php_uri_parser_whatwg_url_query_params_object *php_uri_whatwg_url_query_params_object_from_obj(zend_object *object) {
	return (php_uri_parser_whatwg_url_query_params_object*)((char*)(object) - XtOffsetOf(php_uri_parser_whatwg_url_query_params_object, std));
}

ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_whatwg_percent_encode_user_info(const zend_string *str);
ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_whatwg_percent_encode_host(const zend_string *str);
ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_whatwg_percent_encode_opaque_host(const zend_string *str);
ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_whatwg_percent_encode_path(const zend_string *str);
ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_whatwg_percent_encode_path_segment(const zend_string *str);
ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_whatwg_percent_encode_opaque_path(const zend_string *str);
ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_whatwg_percent_encode_opaque_path_segment(const zend_string *str);

ZEND_ATTRIBUTE_NONNULL  bool php_uri_parser_whatwg_is_special(lxb_url_t *lexbor_uri);
ZEND_ATTRIBUTE_NONNULL void php_uri_parser_whatwg_host_type_read(void *uri, php_uri_component_read_mode read_mode, zval *retval);
ZEND_ATTRIBUTE_NONNULL zend_result php_uri_parser_whatwg_path_segments_read(void *uri, php_uri_component_read_mode read_mode, zval *retval);
ZEND_ATTRIBUTE_NONNULL zend_result php_uri_parser_whatwg_query_params_read(void *uri, php_uri_component_read_mode read_mode, zval *retval);
ZEND_ATTRIBUTE_NONNULL zend_result php_uri_parser_whatwg_query_params_write(void *uri, zval *value, zval *errors);

lxb_url_t *php_uri_parser_whatwg_parse_ex(const char *uri_str, size_t uri_str_len, const lxb_url_t *lexbor_base_url, zval *errors, bool silent);

PHPAPI ZEND_ATTRIBUTE_NONNULL void php_uri_whatwg_url_query_params_free(php_uri_parser_whatwg_url_query_params_object *url_query_params_object);
PHPAPI ZEND_ATTRIBUTE_NONNULL void php_uri_whatwg_url_query_params_object_handler_free(zend_object *object);
ZEND_ATTRIBUTE_NONNULL zend_object *php_uri_whatwg_url_query_params_object_handler_clone(zend_object *object);
ZEND_ATTRIBUTE_NONNULL lxb_url_search_params_t *php_uri_whatwg_url_query_params_from_str(const char *query_str, size_t query_str_len, bool silent);
ZEND_ATTRIBUTE_NONNULL zend_result php_uri_whatwg_url_query_params_append(lxb_url_search_params_t *query_params,
	const char *name, size_t name_len, const char *value, size_t value_len
);
ZEND_ATTRIBUTE_NONNULL_ARGS(1, 2) void php_uri_whatwg_url_query_params_delete(lxb_url_search_params_t *query_params,
	const char *name, size_t name_len, const char *value, size_t value_len
);
ZEND_ATTRIBUTE_NONNULL_ARGS(1, 2) bool php_uri_whatwg_url_query_params_exists(lxb_url_search_params_t *query_params,
	const char *name, size_t name_len, const char *value, size_t value_len
);
ZEND_ATTRIBUTE_NONNULL zend_string * php_uri_whatwg_url_query_params_get_first(lxb_url_search_params_t *query_params,
	const char *name, size_t name_len
);
zend_string *php_uri_whatwg_url_query_params_to_string(lxb_url_search_params_t *query_list);
HashTable *php_uri_whatwg_url_query_params_get_debug_properties(php_uri_parser_whatwg_url_query_params_object *object);

PHP_RINIT_FUNCTION(uri_parser_whatwg);
ZEND_MODULE_POST_ZEND_DEACTIVATE_D(uri_parser_whatwg);

#endif
