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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "php.h"
#include "Zend/zend_interfaces.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_attributes.h"
#include "Zend/zend_enum.h"
#include "ext/standard/info.h"

#include "php_uri.h"
#include "php_uri_query.h"
#include "uri_parser_whatwg.h"
#include "uri_parser_rfc3986.h"
#include "uri_parser_php_parse_url.h"
#include "php_uri_arginfo.h"
#include "uriparser/Uri.h"

static zend_object_handlers object_handlers_rfc3986_uri;
static zend_object_handlers object_handlers_rfc3986_uri_query_params;
static zend_object_handlers object_handlers_whatwg_uri;
static zend_object_handlers object_handlers_whatwg_url_query_params;

static const zend_module_dep uri_deps[] = {
	ZEND_MOD_REQUIRED("lexbor")
	ZEND_MOD_END
};


PHPAPI zend_result php_uri_parser_register(const php_uri_parser *uri_parser)
{
	zend_string *key = zend_string_init_interned(uri_parser->name, strlen(uri_parser->name), true);

	ZEND_ASSERT(uri_parser->name != NULL);
	ZEND_ASSERT(uri_parser->parse != NULL);
	ZEND_ASSERT(uri_parser->clone != NULL || strcmp(uri_parser->name, PHP_URI_PARSER_PHP_PARSE_URL) == 0);
	ZEND_ASSERT(uri_parser->to_string != NULL || strcmp(uri_parser->name, PHP_URI_PARSER_PHP_PARSE_URL) == 0);
	ZEND_ASSERT(uri_parser->destroy != NULL);

	zend_result result = zend_hash_add_ptr(&uri_parsers, key, (void *) uri_parser) != NULL ? SUCCESS : FAILURE;

	zend_string_release_ex(key, true);

	return result;
}

PHPAPI php_uri_object *php_uri_object_create(zend_class_entry *class_type, const php_uri_parser *parser)
{
	php_uri_object *uri_object = zend_object_alloc(sizeof(*uri_object), class_type);

	zend_object_std_init(&uri_object->std, class_type);
	object_properties_init(&uri_object->std, class_type);

	uri_object->parser = parser;
	uri_object->uri = NULL;

	return uri_object;
}

PHPAPI void php_uri_object_handler_free(zend_object *object)
{
	php_uri_object *uri_object = php_uri_object_from_obj(object);

	uri_object->parser->destroy(uri_object->uri);
	zend_object_std_dtor(&uri_object->std);
}

PHPAPI zend_object *php_uri_object_handler_clone(zend_object *object)
{
	php_uri_object *uri_object = php_uri_object_from_obj(object);

	ZEND_ASSERT(uri_object->uri != NULL);

	php_uri_object *new_uri_object = php_uri_object_from_obj(object->ce->create_object(object->ce));
	ZEND_ASSERT(new_uri_object->parser == uri_object->parser);

	void *uri = uri_object->parser->clone(uri_object->uri);
	ZEND_ASSERT(uri != NULL);

	new_uri_object->uri = uri;

	zend_objects_clone_members(&new_uri_object->std, &uri_object->std);

	return &new_uri_object->std;
}

PHPAPI const php_uri_parser *php_uri_get_parser(zend_string *uri_parser_name)
{
	if (uri_parser_name == NULL) {
		return zend_hash_str_find_ptr(&uri_parsers, PHP_URI_PARSER_PHP_PARSE_URL, sizeof(PHP_URI_PARSER_PHP_PARSE_URL) - 1);
	}

	return zend_hash_find_ptr(&uri_parsers, uri_parser_name);
}

ZEND_ATTRIBUTE_NONNULL PHPAPI php_uri_internal *php_uri_parse(const php_uri_parser *uri_parser, const char *uri_str, size_t uri_str_len, bool silent)
{
	void *uri = uri_parser->parse(uri_str, uri_str_len, NULL, NULL, silent);

	if (uri == NULL) {
		return NULL;
	}

	php_uri_internal *internal_uri = emalloc(sizeof(*internal_uri));
	internal_uri->parser = uri_parser;
	internal_uri->uri = uri;

	return internal_uri;
}

ZEND_ATTRIBUTE_NONNULL PHPAPI zend_result php_uri_get_scheme(const php_uri_internal *internal_uri, php_uri_component_read_mode read_mode, zval *zv)
{
	return internal_uri->parser->property_handler.scheme.read(internal_uri->uri, read_mode, zv);
}

ZEND_ATTRIBUTE_NONNULL PHPAPI zend_result php_uri_get_username(const php_uri_internal *internal_uri, php_uri_component_read_mode read_mode, zval *zv)
{
	return internal_uri->parser->property_handler.username.read(internal_uri->uri, read_mode, zv);
}

ZEND_ATTRIBUTE_NONNULL PHPAPI zend_result php_uri_get_password(const php_uri_internal *internal_uri, php_uri_component_read_mode read_mode, zval *zv)
{
	return internal_uri->parser->property_handler.password.read(internal_uri->uri, read_mode, zv);
}

ZEND_ATTRIBUTE_NONNULL PHPAPI zend_result php_uri_get_host(const php_uri_internal *internal_uri, php_uri_component_read_mode read_mode, zval *zv)
{
	return internal_uri->parser->property_handler.host.read(internal_uri->uri, read_mode, zv);
}

ZEND_ATTRIBUTE_NONNULL PHPAPI zend_result php_uri_get_port(const php_uri_internal *internal_uri, php_uri_component_read_mode read_mode, zval *zv)
{
	return internal_uri->parser->property_handler.port.read(internal_uri->uri, read_mode, zv);
}

ZEND_ATTRIBUTE_NONNULL PHPAPI zend_result php_uri_get_path(const php_uri_internal *internal_uri, php_uri_component_read_mode read_mode, zval *zv)
{
	return internal_uri->parser->property_handler.path.read(internal_uri->uri, read_mode, zv);
}

ZEND_ATTRIBUTE_NONNULL PHPAPI zend_result php_uri_get_query(const php_uri_internal *internal_uri, php_uri_component_read_mode read_mode, zval *zv)
{
	return internal_uri->parser->property_handler.query.read(internal_uri->uri, read_mode, zv);
}

ZEND_ATTRIBUTE_NONNULL PHPAPI zend_result php_uri_get_fragment(const php_uri_internal *internal_uri, php_uri_component_read_mode read_mode, zval *zv)
{
	return internal_uri->parser->property_handler.fragment.read(internal_uri->uri, read_mode, zv);
}

ZEND_ATTRIBUTE_NONNULL PHPAPI void php_uri_free(php_uri_internal *internal_uri)
{
	internal_uri->parser->destroy(internal_uri->uri);
	internal_uri->uri = NULL;
	internal_uri->parser = NULL;
	efree(internal_uri);
}

ZEND_ATTRIBUTE_NONNULL PHPAPI php_uri *php_uri_parse_to_struct(
	const php_uri_parser *uri_parser, const char *uri_str, size_t uri_str_len, php_uri_component_read_mode read_mode, bool silent
) {
	php_uri_internal *uri_internal = php_uri_parse(uri_parser, uri_str, uri_str_len, silent);
	if (uri_internal == NULL) {
		return NULL;
	}

	php_uri *uri = ecalloc(1, sizeof(*uri));
	zval tmp;
	zend_result result;

	result = php_uri_get_scheme(uri_internal, read_mode, &tmp);
	if (result == FAILURE) {
		goto error;
	}
	if (Z_TYPE(tmp) == IS_STRING) {
		uri->scheme = Z_STR(tmp);
	}

	result = php_uri_get_username(uri_internal, read_mode, &tmp);
	if (result == FAILURE) {
		goto error;
	}
	if (Z_TYPE(tmp) == IS_STRING) {
		uri->user = Z_STR(tmp);
	}

	result = php_uri_get_password(uri_internal, read_mode, &tmp);
	if (result == FAILURE) {
		goto error;
	}
	if (Z_TYPE(tmp) == IS_STRING) {
		uri->password = Z_STR(tmp);
	}

	result = php_uri_get_host(uri_internal, read_mode, &tmp);
	if (result == FAILURE) {
		goto error;
	}
	if (Z_TYPE(tmp) == IS_STRING) {
		uri->host = Z_STR(tmp);
	}

	result = php_uri_get_port(uri_internal, read_mode, &tmp);
	if (result == FAILURE) {
		goto error;
	}
	if (Z_TYPE(tmp) == IS_LONG) {
		uri->port = Z_LVAL(tmp);
	}

	result = php_uri_get_path(uri_internal, read_mode, &tmp);
	if (result == FAILURE) {
		goto error;
	}
	if (Z_TYPE(tmp) == IS_STRING) {
		uri->path = Z_STR(tmp);
	}

	result = php_uri_get_query(uri_internal, read_mode, &tmp);
	if (result == FAILURE) {
		goto error;
	}
	if (Z_TYPE(tmp) == IS_STRING) {
		uri->query = Z_STR(tmp);
	}

	result = php_uri_get_fragment(uri_internal, read_mode, &tmp);
	if (result == FAILURE) {
		goto error;
	}
	if (Z_TYPE(tmp) == IS_STRING) {
		uri->fragment = Z_STR(tmp);
	}

	php_uri_free(uri_internal);

	return uri;

error:
	php_uri_free(uri_internal);
	php_uri_struct_free(uri);

	return NULL;
}

ZEND_ATTRIBUTE_NONNULL PHPAPI void php_uri_struct_free(php_uri *uri)
{
	if (uri->scheme) {
		zend_string_release(uri->scheme);
	}
	if (uri->user) {
		zend_string_release(uri->user);
	}
	if (uri->password) {
		zend_string_release(uri->password);
	}
	if (uri->host) {
		zend_string_release(uri->host);
	}
	if (uri->path) {
		zend_string_release(uri->path);
	}
	if (uri->query) {
		zend_string_release(uri->query);
	}
	if (uri->fragment) {
		zend_string_release(uri->fragment);
	}

	efree(uri);
}

static zend_object *php_uri_object_create_rfc3986(zend_class_entry *ce)
{
	return &php_uri_object_create(ce, &php_uri_parser_rfc3986)->std;
}

static zend_object *php_uri_object_create_whatwg(zend_class_entry *ce)
{
	return &php_uri_object_create(ce, &php_uri_parser_whatwg)->std;
}

static PHP_MINIT_FUNCTION(uri)
{
	php_uri_ce_comparison_mode = register_class_Uri_UriComparisonMode();
	php_uri_ce_exception = register_class_Uri_UriException(zend_ce_exception);
	php_uri_ce_error = register_class_Uri_UriError(zend_ce_error);
	php_uri_ce_invalid_uri_exception = register_class_Uri_InvalidUriException(php_uri_ce_exception);

	php_uri_ce_rfc3986_uri = register_class_Uri_Rfc3986_Uri();
	php_uri_ce_rfc3986_uri->create_object = php_uri_object_create_rfc3986;
	php_uri_ce_rfc3986_uri->default_object_handlers = &object_handlers_rfc3986_uri;
	memcpy(&object_handlers_rfc3986_uri, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	object_handlers_rfc3986_uri.offset = XtOffsetOf(php_uri_object, std);
	object_handlers_rfc3986_uri.free_obj = php_uri_object_handler_free;
	object_handlers_rfc3986_uri.clone_obj = php_uri_object_handler_clone;

	php_uri_ce_rfc3986_uri_type = register_class_Uri_Rfc3986_UriType();
	php_uri_ce_rfc3986_uri_host_type = register_class_Uri_Rfc3986_UriHostType();
	php_uri_ce_rfc3986_uri_percent_encoding_mode = register_class_Uri_Rfc3986_UriPercentEncodingMode();
	php_uri_ce_rfc3986_uri_query_params = register_class_Uri_Rfc3986_UriQueryParams(zend_ce_countable);
	php_uri_ce_rfc3986_uri_query_params->create_object = php_uri_object_create_rfc3986_uri_query_params;
	php_uri_ce_rfc3986_uri_query_params->default_object_handlers = &object_handlers_rfc3986_uri_query_params;
	memcpy(&object_handlers_rfc3986_uri_query_params, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	object_handlers_rfc3986_uri_query_params.offset = XtOffsetOf(php_uri_parser_rfc3986_uri_query_params_object, std);
	object_handlers_rfc3986_uri_query_params.free_obj = php_uri_rfc3986_uri_query_params_object_handler_free;
	object_handlers_rfc3986_uri_query_params.clone_obj = php_uri_rfc3986_uri_query_params_object_handler_clone;

	php_uri_ce_whatwg_url = register_class_Uri_WhatWg_Url();
	php_uri_ce_whatwg_url->create_object = php_uri_object_create_whatwg;
	php_uri_ce_whatwg_url->default_object_handlers = &object_handlers_whatwg_uri;
	memcpy(&object_handlers_whatwg_uri, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	object_handlers_whatwg_uri.offset = XtOffsetOf(php_uri_object, std);
	object_handlers_whatwg_uri.free_obj = php_uri_object_handler_free;
	object_handlers_whatwg_uri.clone_obj = php_uri_object_handler_clone;

	php_uri_ce_whatwg_invalid_url_exception = register_class_Uri_WhatWg_InvalidUrlException(php_uri_ce_invalid_uri_exception);
	php_uri_ce_whatwg_url_validation_error = register_class_Uri_WhatWg_UrlValidationError();
	php_uri_ce_whatwg_url_validation_error_type = register_class_Uri_WhatWg_UrlValidationErrorType();

	php_uri_ce_whatwg_url_host_type = register_class_Uri_WhatWg_UrlHostType();
	php_uri_ce_whatwg_url_percent_encoding_mode = register_class_Uri_WhatWg_UrlPercentEncodingMode();
	php_uri_ce_whatwg_url_query_params = register_class_Uri_WhatWg_UrlQueryParams(zend_ce_countable);
	php_uri_ce_whatwg_url_query_params->create_object = php_uri_object_create_rfc3986_uri_query_params;
	php_uri_ce_whatwg_url_query_params->default_object_handlers = &object_handlers_whatwg_url_query_params;
	memcpy(&object_handlers_whatwg_url_query_params, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	object_handlers_whatwg_url_query_params.offset = XtOffsetOf(php_uri_parser_whatwg_url_query_params_object, std);
	object_handlers_whatwg_url_query_params.free_obj = php_uri_whatwg_url_query_params_object_handler_free;
	object_handlers_whatwg_url_query_params.clone_obj = php_uri_whatwg_url_query_params_object_handler_clone;

	zend_hash_init(&uri_parsers, 4, NULL, NULL, true);

	if (php_uri_parser_register(&php_uri_parser_rfc3986) == FAILURE) {
		return FAILURE;
	}

	if (php_uri_parser_register(&php_uri_parser_whatwg) == FAILURE) {
		return FAILURE;
	}

	if (php_uri_parser_register(&php_uri_parser_php_parse_url) == FAILURE) {
		return FAILURE;
	}

	return SUCCESS;
}

static PHP_MINFO_FUNCTION(uri)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "URI support", "active");
#ifdef URI_STATIC_BUILD
	php_info_print_table_row(2, "uriparser bundled version", URI_VER_ANSI);
#else
	php_info_print_table_row(2, "uriparser compiled version", URI_VER_ANSI);
	php_info_print_table_row(2, "uriparser loaded version", uriBaseRuntimeVersionA());
#endif
	php_info_print_table_end();
}

static PHP_MSHUTDOWN_FUNCTION(uri)
{
	zend_hash_destroy(&uri_parsers);

	return SUCCESS;
}

PHP_RINIT_FUNCTION(uri)
{
	if (PHP_RINIT(uri_parser_whatwg)(INIT_FUNC_ARGS_PASSTHRU) == FAILURE) {
		return FAILURE;
	}

	return SUCCESS;
}

ZEND_MODULE_POST_ZEND_DEACTIVATE_D(uri)
{
	if (ZEND_MODULE_POST_ZEND_DEACTIVATE_N(uri_parser_whatwg)() == FAILURE) {
		return FAILURE;
	}

	return SUCCESS;
}

zend_module_entry uri_module_entry = {
	STANDARD_MODULE_HEADER_EX, NULL,
	uri_deps,
	"uri",                          /* Extension name */
	NULL,                           /* zend_function_entry */
	PHP_MINIT(uri),                 /* PHP_MINIT - Module initialization */
	PHP_MSHUTDOWN(uri),             /* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(uri),                 /* PHP_RINIT - Request initialization */
	NULL,                           /* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(uri),                 /* PHP_MINFO - Module info */
	PHP_VERSION,                    /* Version */
	NO_MODULE_GLOBALS,
	ZEND_MODULE_POST_ZEND_DEACTIVATE_N(uri),
	STANDARD_MODULE_PROPERTIES_EX
};
