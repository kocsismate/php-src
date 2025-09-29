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

#ifndef PHP_URI_PARSER_RFC3986_H
#define PHP_URI_PARSER_RFC3986_H

#include "php_uri_common.h"

PHPAPI extern const php_uri_parser php_uri_parser_rfc3986;

typedef struct php_uri_parser_rfc3986_uris php_uri_parser_rfc3986_uris;
typedef struct UriQueryListStructA UriQueryListA;

typedef struct php_uri_parser_rfc3986_uri_query_params_object {
	UriQueryListA *query_list;
	bool is_initialized;
	zend_object std;
} php_uri_parser_rfc3986_uri_query_params_object;

#define Z_URI_QUERY_PARAMS_OBJECT_P(zv) php_uri_rfc3986_uri_query_params_object_from_obj(Z_OBJ_P((zv)))

static inline php_uri_parser_rfc3986_uri_query_params_object *php_uri_rfc3986_uri_query_params_object_from_obj(zend_object *object) {
	return (php_uri_parser_rfc3986_uri_query_params_object*)((char*)(object) - XtOffsetOf(php_uri_parser_rfc3986_uri_query_params_object, std));
}

ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_rfc3986_percent_encode_user_info(const zend_string *str);
ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_rfc3986_percent_encode_host(const zend_string *str);
ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_rfc3986_percent_encode_path(const zend_string *str);
ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_rfc3986_percent_encode_path_segment(const zend_string *str);
ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_rfc3986_percent_encode_relative_reference_path(const zend_string *str);
ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_rfc3986_percent_encode_relative_reference_first_path_segment(const zend_string *str);
ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_rfc3986_percent_encode_query_or_fragment(const zend_string *str);
ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_rfc3986_percent_encode_reserved_characters(const zend_string *str);
ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_rfc3986_percent_encode_all(const zend_string *str);

ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_rfc3986_percent_decode(const zend_string *str);
ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_rfc3986_percent_decode_reserved_characters(const zend_string *str);

ZEND_ATTRIBUTE_NONNULL void php_uri_parser_rfc3986_uri_type_read(void *uri, zval *retval);
zend_result php_uri_parser_rfc3986_userinfo_read(php_uri_parser_rfc3986_uris *uri, php_uri_component_read_mode read_mode, zval *retval);
zend_result php_uri_parser_rfc3986_userinfo_write(php_uri_parser_rfc3986_uris *uri, zval *value, zval *errors);
void php_uri_parser_rfc3986_host_type_read(void *uri, php_uri_component_read_mode read_mode, zval *retval);
zend_result php_uri_parser_rfc3986_path_segments_read(void *uri, php_uri_component_read_mode read_mode, zval *retval);
zend_result php_uri_parser_rfc3986_path_segments_write(php_uri_parser_rfc3986_uris *uri, zval *value, bool include_leading_slash_for_non_empty_relative_uri);
zend_result php_uri_parser_rfc3986_path_query_params_read(void *uri, php_uri_component_read_mode read_mode, zval *retval);
ZEND_ATTRIBUTE_NONNULL zend_result php_uri_parser_rfc3986_query_params_read(void *uri, php_uri_component_read_mode read_mode, zval *retval);
ZEND_ATTRIBUTE_NONNULL zend_result php_uri_parser_rfc3986_query_params_write(void *uri, zval *value);

php_uri_parser_rfc3986_uris *php_uri_parser_rfc3986_parse_ex(const char *uri_str, size_t uri_str_len, const php_uri_parser_rfc3986_uris *uriparser_base_url, bool silent);

PHPAPI void php_uri_rfc3986_uri_query_params_free(php_uri_parser_rfc3986_uri_query_params_object *uri_query_params_object);
PHPAPI void php_uri_rfc3986_uri_query_params_object_handler_free(zend_object *object);
zend_object *php_uri_rfc3986_uri_query_params_object_handler_clone(zend_object *object);
zend_result php_uri_rfc3986_uri_query_params_from_str(const char *query_str_first, size_t query_str_length, UriQueryListA **query_list, bool silent);
ZEND_ATTRIBUTE_NONNULL_ARGS(1,2) zend_result php_uri_rfc3986_uri_query_params_append(UriQueryListA **query_list,
	const char *name, size_t name_len, const char *value, size_t value_len
);
zend_string *php_uri_rfc3986_uri_query_params_to_string(const UriQueryListA *query_list);
HashTable *php_uri_rfc3986_uri_query_params_get_debug_properties(php_uri_parser_rfc3986_uri_query_params_object *object);

#endif
