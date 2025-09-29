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

#include "zend_smart_str.h"
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "php.h"
#include "Zend/zend_interfaces.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_attributes.h"
#include "Zend/zend_enum.h"

#include "php_uri.h"
#include "uri_parser_whatwg.h"
#include "uri_parser_rfc3986.h"
#include "uriparser/UriBase.h"

PHP_METHOD(Uri_Rfc3986_Uri, percentEncode)
{
	zend_string *str;
	zend_object *mode_object;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(str)
		Z_PARAM_OBJ_OF_CLASS(mode_object, php_uri_ce_rfc3986_uri_percent_encoding_mode)
	ZEND_PARSE_PARAMETERS_END();

	zval *case_name = zend_enum_fetch_case_name(mode_object);
	zend_string *result;

	if (zend_string_equals_literal(Z_STR_P(case_name), "UserInfo")) {
		result = php_uri_parser_rfc3986_percent_encode_user_info(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "Host")) {
		result = php_uri_parser_rfc3986_percent_encode_host(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "Path")) {
		result = php_uri_parser_rfc3986_percent_encode_path(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "PathSegment")) {
		result = php_uri_parser_rfc3986_percent_encode_path_segment(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "RelativeReferencePath")) {
		result = php_uri_parser_rfc3986_percent_encode_relative_reference_path(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "RelativeReferenceFirstPathSegment")) {
		result = php_uri_parser_rfc3986_percent_encode_relative_reference_first_path_segment(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "Query")) {
		result = php_uri_parser_rfc3986_percent_encode_query_or_fragment(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "FormQuery")) {
		result = php_uri_parser_rfc3986_percent_encode_query_or_fragment(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "Fragment")) {
		result = php_uri_parser_rfc3986_percent_encode_query_or_fragment(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "AllReservedCharacters")) {
		result = php_uri_parser_rfc3986_percent_encode_reserved_characters(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "All")) {
		result = php_uri_parser_rfc3986_percent_encode_all(str);
	} else {
		ZEND_UNREACHABLE();
	}

	RETVAL_NEW_STR(result);
}

PHP_METHOD(Uri_Rfc3986_Uri, percentDecode)
{
	zend_string *str;
	zend_object *mode_object;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(str)
		Z_PARAM_OBJ_OF_CLASS(mode_object, php_uri_ce_rfc3986_uri_percent_encoding_mode)
	ZEND_PARSE_PARAMETERS_END();

	zval *case_name = zend_enum_fetch_case_name(mode_object);
	zend_string *result;

	if (zend_string_equals_literal(Z_STR_P(case_name), "UserInfo")) {
		result = php_uri_parser_rfc3986_percent_encode_user_info(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "Host")) {
		result = php_uri_parser_rfc3986_percent_encode_host(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "Path")) {
		result = php_uri_parser_rfc3986_percent_encode_path(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "PathSegment")) {
		result = php_uri_parser_rfc3986_percent_encode_path_segment(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "RelativeReferencePath")) {
		result = php_uri_parser_rfc3986_percent_encode_relative_reference_path(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "RelativeReferenceFirstPathSegment")) {
		result = php_uri_parser_rfc3986_percent_encode_relative_reference_first_path_segment(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "Query")) {
		result = php_uri_parser_rfc3986_percent_encode_query_or_fragment(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "FormQuery")) {
		result = php_uri_parser_rfc3986_percent_encode_query_or_fragment(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "Fragment")) {
		result = php_uri_parser_rfc3986_percent_encode_query_or_fragment(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "AllReservedCharacters")) {
		result = php_uri_parser_rfc3986_percent_decode_reserved_characters(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "All")) {
		result = php_uri_parser_rfc3986_percent_decode(str);
	} else {
		ZEND_UNREACHABLE();
	}

	if (result == NULL) {
		RETURN_NULL();
	}

	RETVAL_NEW_STR(result);
}

/**
 * Pass the errors parameter by ref to errors_zv for userland, and frees it if
 * it is not not needed anymore.
 */
static zend_result pass_errors_by_ref_and_free(zval *errors_zv, zval *errors)
{
	ZEND_ASSERT(Z_TYPE_P(errors) == IS_UNDEF || Z_TYPE_P(errors) == IS_ARRAY);

	/* There was no error during parsing */
	if (Z_ISUNDEF_P(errors)) {
		return SUCCESS;
	}

	/* The errors parameter is an array, but the pass-by ref argument stored by
	 * errors_zv was not passed - the URI implementation either doesn't support
	 * returning additional error information, or the caller is not interested in it */
	if (errors_zv == NULL) {
		zval_ptr_dtor(errors);
		return SUCCESS;
	}

	ZEND_TRY_ASSIGN_REF_TMP(errors_zv, errors);
	if (EG(exception)) {
		return FAILURE;
	}

	return SUCCESS;
}

ZEND_ATTRIBUTE_NONNULL_ARGS(1, 2) PHPAPI void php_uri_instantiate_uri(
		INTERNAL_FUNCTION_PARAMETERS, const zend_string *uri_str, const php_uri_object *base_url_object,
		bool should_throw, bool should_update_this_object, zval *errors_zv
) {
	php_uri_object *uri_object;
	if (should_update_this_object) {
		uri_object = Z_URI_OBJECT_P(ZEND_THIS);
		if (uri_object->uri != NULL) {
			zend_throw_error(NULL, "Cannot modify readonly object of class %s", ZSTR_VAL(Z_OBJCE_P(ZEND_THIS)->name));
			RETURN_THROWS();
		}
	} else {
		if (EX(func)->common.fn_flags & ZEND_ACC_STATIC) {
			object_init_ex(return_value, Z_CE_P(ZEND_THIS));
		} else {
			object_init_ex(return_value, Z_OBJCE_P(ZEND_THIS));
		}
		uri_object = Z_URI_OBJECT_P(return_value);
	}

	const php_uri_parser *uri_parser = uri_object->parser;

	zval errors;
	ZVAL_UNDEF(&errors);

	void *base_url = NULL;
	if (base_url_object != NULL) {
		ZEND_ASSERT(base_url_object->std.ce == uri_object->std.ce);
		ZEND_ASSERT(base_url_object->uri != NULL);
		ZEND_ASSERT(base_url_object->parser == uri_parser);
		base_url = base_url_object->uri;
	}

	void *uri = uri_parser->parse(ZSTR_VAL(uri_str), ZSTR_LEN(uri_str), base_url, errors_zv != NULL ? &errors : NULL, !should_throw);
	if (UNEXPECTED(uri == NULL)) {
		if (should_throw) {
			zval_ptr_dtor(&errors);
			RETURN_THROWS();
		} else {
			if (pass_errors_by_ref_and_free(errors_zv, &errors) == FAILURE) {
				RETURN_THROWS();
			}
			zval_ptr_dtor(return_value);
			RETURN_NULL();
		}
	}

	if (pass_errors_by_ref_and_free(errors_zv, &errors) == FAILURE) {
		uri_parser->destroy(uri);
		RETURN_THROWS();
	}

	uri_object->uri = uri;
}

static void create_rfc3986_uri(INTERNAL_FUNCTION_PARAMETERS, bool is_constructor)
{
	zend_string *uri_str;
	zend_object *base_url_object = NULL;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(uri_str)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJ_OF_CLASS_OR_NULL(base_url_object, php_uri_ce_rfc3986_uri)
	ZEND_PARSE_PARAMETERS_END();

	php_uri_instantiate_uri(INTERNAL_FUNCTION_PARAM_PASSTHRU,
		uri_str, base_url_object ? php_uri_object_from_obj(base_url_object) : NULL, is_constructor, is_constructor, NULL);
}

PHP_METHOD(Uri_Rfc3986_Uri, __construct)
{
	create_rfc3986_uri(INTERNAL_FUNCTION_PARAM_PASSTHRU, true);
}

PHP_METHOD(Uri_Rfc3986_Uri, parse)
{
	create_rfc3986_uri(INTERNAL_FUNCTION_PARAM_PASSTHRU, false);
}

static bool is_list_of_whatwg_validation_errors(const HashTable *array)
{
	if (!zend_array_is_list(array)) {
		return false;
	}

	ZEND_HASH_FOREACH_VAL(array, zval *val) {
		/* Do not allow references as they may change types after checking. */

		if (Z_TYPE_P(val) != IS_OBJECT) {
			return false;
		}

		if (!instanceof_function(Z_OBJCE_P(val), php_uri_ce_whatwg_url_validation_error)) {
			return false;
		}
	} ZEND_HASH_FOREACH_END();

	return true;
}

PHP_METHOD(Uri_WhatWg_InvalidUrlException, __construct)
{
	zend_string *message = NULL;
	zval *errors = NULL;
	zend_long code = 0;
	zval *previous = NULL;

	ZEND_PARSE_PARAMETERS_START(0, 4)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR(message)
		Z_PARAM_ARRAY(errors)
		Z_PARAM_LONG(code)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(previous, zend_ce_throwable)
	ZEND_PARSE_PARAMETERS_END();

	if (zend_update_exception_properties(INTERNAL_FUNCTION_PARAM_PASSTHRU, message, code, previous) == FAILURE) {
		RETURN_THROWS();
	}

	if (errors == NULL) {
		zval tmp;
		ZVAL_EMPTY_ARRAY(&tmp);
		zend_update_property(php_uri_ce_whatwg_invalid_url_exception, Z_OBJ_P(ZEND_THIS), ZEND_STRL("errors"), &tmp);
	} else {
		if (!is_list_of_whatwg_validation_errors(Z_ARR_P(errors))) {
			zend_argument_value_error(2, "must be a list of %s", ZSTR_VAL(php_uri_ce_whatwg_url_validation_error->name));
			RETURN_THROWS();
		}

		zend_update_property(php_uri_ce_whatwg_invalid_url_exception, Z_OBJ_P(ZEND_THIS), ZEND_STRL("errors"), errors);
	}
	if (EG(exception)) {
		RETURN_THROWS();
	}
}

PHP_METHOD(Uri_WhatWg_UrlValidationError, __construct)
{
	zend_string *context;
	zval *type;
	bool failure;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_STR(context)
		Z_PARAM_OBJECT_OF_CLASS(type, php_uri_ce_whatwg_url_validation_error_type)
		Z_PARAM_BOOL(failure)
	ZEND_PARSE_PARAMETERS_END();

	zend_update_property_str(php_uri_ce_whatwg_url_validation_error, Z_OBJ_P(ZEND_THIS), ZEND_STRL("context"), context);
	if (EG(exception)) {
		RETURN_THROWS();
	}

	zend_update_property_ex(php_uri_ce_whatwg_url_validation_error, Z_OBJ_P(ZEND_THIS), ZSTR_KNOWN(ZEND_STR_TYPE), type);
	if (EG(exception)) {
		RETURN_THROWS();
	}

	zval failure_zv;
	ZVAL_BOOL(&failure_zv, failure);
	zend_update_property(php_uri_ce_whatwg_url_validation_error, Z_OBJ_P(ZEND_THIS), ZEND_STRL("failure"), &failure_zv);
	if (EG(exception)) {
		RETURN_THROWS();
	}
}

static void create_whatwg_uri(INTERNAL_FUNCTION_PARAMETERS, bool is_constructor)
{
	zend_string *uri_str;
	zend_object *base_url_object = NULL;
	zval *errors = NULL;

	ZEND_PARSE_PARAMETERS_START(1, 3)
		Z_PARAM_STR(uri_str)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJ_OF_CLASS_OR_NULL(base_url_object, php_uri_ce_whatwg_url)
		Z_PARAM_ZVAL(errors)
	ZEND_PARSE_PARAMETERS_END();

	php_uri_instantiate_uri(INTERNAL_FUNCTION_PARAM_PASSTHRU,
		uri_str, base_url_object ? php_uri_object_from_obj(base_url_object) : NULL, is_constructor, is_constructor, errors);
}

PHP_METHOD(Uri_WhatWg_Url, __construct)
{
	create_whatwg_uri(INTERNAL_FUNCTION_PARAM_PASSTHRU, true);
}

PHP_METHOD(Uri_WhatWg_Url, parse)
{
	create_whatwg_uri(INTERNAL_FUNCTION_PARAM_PASSTHRU, false);
}

PHP_METHOD(Uri_Rfc3986_Uri, getUriType)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_uri_object *uri_object = Z_URI_OBJECT_P(ZEND_THIS);
	ZEND_ASSERT(uri_object->uri != NULL);

	php_uri_parser_rfc3986_uri_type_read(uri_object->uri, return_value);
}

PHP_METHOD(Uri_Rfc3986_Uri, getScheme)
{
	php_uri_property_read_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_SCHEME, PHP_URI_COMPONENT_READ_MODE_NORMALIZED_ASCII);
}

PHP_METHOD(Uri_Rfc3986_Uri, getRawScheme)
{
	php_uri_property_read_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_SCHEME, PHP_URI_COMPONENT_READ_MODE_RAW);
}

PHP_METHOD(Uri_Rfc3986_Uri, withScheme)
{
	php_uri_property_write_str_or_null_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_SCHEME);
}

static void rfc3986_userinfo_read(INTERNAL_FUNCTION_PARAMETERS, php_uri_component_read_mode read_mode)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_uri_object *uri_object = Z_URI_OBJECT_P(ZEND_THIS);
	ZEND_ASSERT(uri_object->uri != NULL);

	if (UNEXPECTED(php_uri_parser_rfc3986_userinfo_read(uri_object->uri, read_mode, return_value) == FAILURE)) {
		zend_throw_error(NULL, "The userinfo component cannot be retrieved");
		RETURN_THROWS();
	}
}

PHP_METHOD(Uri_Rfc3986_Uri, getUserInfo)
{
	rfc3986_userinfo_read(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_COMPONENT_READ_MODE_NORMALIZED_ASCII);
}

PHP_METHOD(Uri_Rfc3986_Uri, getRawUserInfo)
{
	rfc3986_userinfo_read(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_COMPONENT_READ_MODE_RAW);
}

PHP_METHOD(Uri_Rfc3986_Uri, withUserInfo)
{
	zend_string *value;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR_OR_NULL(value)
	ZEND_PARSE_PARAMETERS_END();

	zval zv;
	if (value == NULL) {
		ZVAL_NULL(&zv);
	} else {
		ZVAL_STR(&zv, value);
	}

	php_uri_object *old_uri_object = php_uri_object_from_obj(Z_OBJ_P(ZEND_THIS));
	ZEND_ASSERT(old_uri_object->uri != NULL);

	zend_object *new_object = old_uri_object->std.handlers->clone_obj(&old_uri_object->std);
	if (new_object == NULL) {
		RETURN_THROWS();
	}

	/* Assign the object early. The engine will take care of destruction in
	 * case of an exception being thrown. */
	RETVAL_OBJ(new_object);

	php_uri_object *new_uri_object = php_uri_object_from_obj(new_object);
	ZEND_ASSERT(new_uri_object->uri != NULL);

	if (UNEXPECTED(php_uri_parser_rfc3986_userinfo_write(new_uri_object->uri, &zv, NULL) == FAILURE)) {
		RETURN_THROWS();
	}
}

PHP_METHOD(Uri_Rfc3986_Uri, getUsername)
{
	php_uri_property_read_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_USERNAME, PHP_URI_COMPONENT_READ_MODE_NORMALIZED_ASCII);
}

PHP_METHOD(Uri_Rfc3986_Uri, getRawUsername)
{
	php_uri_property_read_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_USERNAME, PHP_URI_COMPONENT_READ_MODE_RAW);
}

PHP_METHOD(Uri_Rfc3986_Uri, getPassword)
{
	php_uri_property_read_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_PASSWORD, PHP_URI_COMPONENT_READ_MODE_NORMALIZED_ASCII);
}

PHP_METHOD(Uri_Rfc3986_Uri, getRawPassword)
{
	php_uri_property_read_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_PASSWORD, PHP_URI_COMPONENT_READ_MODE_RAW);
}

PHP_METHOD(Uri_Rfc3986_Uri, getHost)
{
	php_uri_property_read_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_HOST, PHP_URI_COMPONENT_READ_MODE_NORMALIZED_ASCII);
}

PHP_METHOD(Uri_Rfc3986_Uri, getRawHost)
{
	php_uri_property_read_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_HOST, PHP_URI_COMPONENT_READ_MODE_RAW);
}

PHP_METHOD(Uri_Rfc3986_Uri, getHostType)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_uri_object *uri_object = Z_URI_OBJECT_P(ZEND_THIS);
	ZEND_ASSERT(uri_object->uri != NULL);

	php_uri_parser_rfc3986_host_type_read(uri_object->uri, PHP_URI_COMPONENT_READ_MODE_RAW, return_value);
}

PHP_METHOD(Uri_Rfc3986_Uri, withHost)
{
	php_uri_property_write_str_or_null_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_HOST);
}

PHP_METHOD(Uri_Rfc3986_Uri, getPort)
{
	php_uri_property_read_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_PORT, PHP_URI_COMPONENT_READ_MODE_RAW);
}

PHP_METHOD(Uri_Rfc3986_Uri, withPort)
{
	php_uri_property_write_long_or_null_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_PORT);
}

PHP_METHOD(Uri_Rfc3986_Uri, getPath)
{
	php_uri_property_read_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_PATH, PHP_URI_COMPONENT_READ_MODE_NORMALIZED_ASCII);
}

PHP_METHOD(Uri_Rfc3986_Uri, getRawPath)
{
	php_uri_property_read_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_PATH, PHP_URI_COMPONENT_READ_MODE_RAW);
}

PHP_METHOD(Uri_Rfc3986_Uri, getPathSegments)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_uri_object *uri_object = Z_URI_OBJECT_P(ZEND_THIS);
	ZEND_ASSERT(uri_object->uri != NULL);

	if (php_uri_parser_rfc3986_path_segments_read(uri_object->uri, PHP_URI_COMPONENT_READ_MODE_NORMALIZED_ASCII, return_value) == FAILURE) {
		zend_throw_exception_ex(php_uri_ce_error, 0, "The path component cannot be retrieved");
		RETURN_THROWS();
	}
}

PHP_METHOD(Uri_Rfc3986_Uri, getRawPathSegments)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_uri_object *uri_object = Z_URI_OBJECT_P(ZEND_THIS);
	ZEND_ASSERT(uri_object->uri != NULL);

	if (php_uri_parser_rfc3986_path_segments_read(uri_object->uri, PHP_URI_COMPONENT_READ_MODE_RAW, return_value) == FAILURE) {
		zend_throw_exception_ex(php_uri_ce_error, 0, "The path component cannot be retrieved");
		RETURN_THROWS();
	}
}

PHP_METHOD(Uri_Rfc3986_Uri, withPath)
{
	php_uri_property_write_str_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_PATH);
}

PHP_METHOD(Uri_Rfc3986_Uri, withPathSegments)
{
	zval *segments;
	bool include_leading_slash_for_non_empty_relative_uri = true;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ARRAY(segments)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(include_leading_slash_for_non_empty_relative_uri)
	ZEND_PARSE_PARAMETERS_END();

	php_uri_object *old_uri_object = Z_URI_OBJECT_P(ZEND_THIS);
	ZEND_ASSERT(old_uri_object->uri != NULL);

	zend_object *new_object = old_uri_object->std.handlers->clone_obj(&old_uri_object->std);
	if (new_object == NULL) {
		RETURN_THROWS();
	}

	/* Assign the object early. The engine will take care of destruction in
	 * case of an exception being thrown. */
	RETVAL_OBJ(new_object);

	php_uri_object *new_uri_object = php_uri_object_from_obj(new_object);
	ZEND_ASSERT(new_uri_object->uri != NULL);

	if (UNEXPECTED(php_uri_parser_rfc3986_path_segments_write(new_uri_object->uri, segments, include_leading_slash_for_non_empty_relative_uri) == FAILURE)) {
		RETURN_THROWS();
	}
}

PHP_METHOD(Uri_Rfc3986_Uri, getQuery)
{
	php_uri_property_read_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_QUERY, PHP_URI_COMPONENT_READ_MODE_NORMALIZED_ASCII);
}

PHP_METHOD(Uri_Rfc3986_Uri, getRawQuery)
{
	php_uri_property_read_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_QUERY, PHP_URI_COMPONENT_READ_MODE_RAW);
}

PHP_METHOD(Uri_Rfc3986_Uri, withQuery)
{
	php_uri_property_write_str_or_null_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_QUERY);
}

PHP_METHOD(Uri_Rfc3986_Uri, getRawQueryParams)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_uri_object *uri_object = Z_URI_OBJECT_P(ZEND_THIS);
	ZEND_ASSERT(uri_object->uri != NULL);

	if (UNEXPECTED(php_uri_parser_rfc3986_query_params_read(uri_object->uri, PHP_URI_COMPONENT_READ_MODE_RAW, return_value) == FAILURE)) {
		RETURN_THROWS();
	}
}

PHP_METHOD(Uri_Rfc3986_Uri, getQueryParams)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_uri_object *uri_object = Z_URI_OBJECT_P(ZEND_THIS);
	ZEND_ASSERT(uri_object->uri != NULL);

	if (UNEXPECTED(php_uri_parser_rfc3986_query_params_read(uri_object->uri, PHP_URI_COMPONENT_READ_MODE_NORMALIZED_ASCII, return_value) == FAILURE)) {
		RETURN_THROWS();
	}
}

PHP_METHOD(Uri_Rfc3986_Uri, withQueryParams)
{
	zval *zv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(zv, php_uri_ce_rfc3986_uri_query_params)
	ZEND_PARSE_PARAMETERS_END();

	php_uri_object *old_uri_object = php_uri_object_from_obj(Z_OBJ_P(ZEND_THIS));
	ZEND_ASSERT(old_uri_object->uri != NULL);

	zend_object *new_object = old_uri_object->std.handlers->clone_obj(&old_uri_object->std);
	if (new_object == NULL) {
		RETURN_THROWS();
	}

	/* Assign the object early. The engine will take care of destruction in
	 * case of an exception being thrown. */
	RETVAL_OBJ(new_object);

	php_uri_object *new_uri_object = php_uri_object_from_obj(new_object);
	ZEND_ASSERT(new_uri_object->uri != NULL);

	if (UNEXPECTED(php_uri_parser_rfc3986_query_params_write(new_uri_object->uri, zv) == FAILURE)) {
		RETURN_THROWS();
	}
}

PHP_METHOD(Uri_Rfc3986_Uri, getFragment)
{
	php_uri_property_read_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_FRAGMENT, PHP_URI_COMPONENT_READ_MODE_NORMALIZED_ASCII);
}

PHP_METHOD(Uri_Rfc3986_Uri, getRawFragment)
{
	php_uri_property_read_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_FRAGMENT, PHP_URI_COMPONENT_READ_MODE_RAW);
}

PHP_METHOD(Uri_Rfc3986_Uri, withFragment)
{
	php_uri_property_write_str_or_null_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_FRAGMENT);
}

static void throw_cannot_recompose_uri_to_string(zend_object *object)
{
	zend_throw_exception_ex(php_uri_ce_error, 0, "Cannot recompose %s to a string", ZSTR_VAL(object->ce->name));
}

static void uri_equals(INTERNAL_FUNCTION_PARAMETERS, php_uri_object *that_object, zend_object *comparison_mode)
{
	php_uri_object *this_object = Z_URI_OBJECT_P(ZEND_THIS);
	ZEND_ASSERT(this_object->uri != NULL);
	ZEND_ASSERT(that_object->uri != NULL);

	if (this_object->std.ce != that_object->std.ce &&
		!instanceof_function(this_object->std.ce, that_object->std.ce) &&
		!instanceof_function(that_object->std.ce, this_object->std.ce)
	) {
		RETURN_FALSE;
	}

	bool exclude_fragment = true;
	if (comparison_mode) {
		zval *case_name = zend_enum_fetch_case_name(comparison_mode);
		exclude_fragment = zend_string_equals_literal(Z_STR_P(case_name), "ExcludeFragment");
	}

	zend_string *this_str = this_object->parser->to_string(
		this_object->uri, PHP_URI_RECOMPOSITION_MODE_NORMALIZED_ASCII, exclude_fragment);
	if (this_str == NULL) {
		throw_cannot_recompose_uri_to_string(&this_object->std);
		RETURN_THROWS();
	}

	zend_string *that_str = that_object->parser->to_string(
		that_object->uri, PHP_URI_RECOMPOSITION_MODE_NORMALIZED_ASCII, exclude_fragment);
	if (that_str == NULL) {
		zend_string_release(this_str);
		throw_cannot_recompose_uri_to_string(&that_object->std);
		RETURN_THROWS();
	}

	RETVAL_BOOL(zend_string_equals(this_str, that_str));

	zend_string_release(this_str);
	zend_string_release(that_str);
}

PHP_METHOD(Uri_Rfc3986_Uri, equals)
{
	zend_object *that_object;
	zend_object *comparison_mode = NULL;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_OBJ_OF_CLASS(that_object, php_uri_ce_rfc3986_uri)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJ_OF_CLASS(comparison_mode, php_uri_ce_comparison_mode)
	ZEND_PARSE_PARAMETERS_END();

	uri_equals(INTERNAL_FUNCTION_PARAM_PASSTHRU, php_uri_object_from_obj(that_object), comparison_mode);
}

PHP_METHOD(Uri_Rfc3986_Uri, toRawString)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_uri_object *uri_object = Z_URI_OBJECT_P(ZEND_THIS);
	ZEND_ASSERT(uri_object->uri != NULL);

	zend_string *uri_str = uri_object->parser->to_string(uri_object->uri, PHP_URI_RECOMPOSITION_MODE_RAW_ASCII, false);
	if (uri_str == NULL) {
		throw_cannot_recompose_uri_to_string(&uri_object->std);
		RETURN_THROWS();
	}

	RETURN_STR(uri_str);
}

PHP_METHOD(Uri_Rfc3986_Uri, toString)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_uri_object *uri_object = Z_URI_OBJECT_P(ZEND_THIS);
	ZEND_ASSERT(uri_object->uri != NULL);

	zend_string *uri_str = uri_object->parser->to_string(uri_object->uri, PHP_URI_RECOMPOSITION_MODE_NORMALIZED_ASCII, false);
	if (uri_str == NULL) {
		throw_cannot_recompose_uri_to_string(&uri_object->std);
		RETURN_THROWS();
	}

	RETURN_STR(uri_str);
}

PHP_METHOD(Uri_Rfc3986_Uri, resolve)
{
	zend_string *uri_str;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(uri_str)
	ZEND_PARSE_PARAMETERS_END();

	php_uri_instantiate_uri(INTERNAL_FUNCTION_PARAM_PASSTHRU,
		uri_str, Z_URI_OBJECT_P(ZEND_THIS), true, false, NULL);
}

PHP_METHOD(Uri_Rfc3986_Uri, __serialize)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_uri_object *uri_object = Z_URI_OBJECT_P(ZEND_THIS);
	ZEND_ASSERT(uri_object->uri != NULL);

	/* Serialize state: "uri" key in the first array */
	zend_string *uri_str = uri_object->parser->to_string(uri_object->uri, PHP_URI_RECOMPOSITION_MODE_RAW_ASCII, false);
	if (uri_str == NULL) {
		throw_cannot_recompose_uri_to_string(&uri_object->std);
		RETURN_THROWS();
	}
	zval tmp;
	ZVAL_STR(&tmp, uri_str);

	array_init(return_value);

	zval arr;
	array_init(&arr);
	zend_hash_str_add_new(Z_ARRVAL(arr), PHP_URI_SERIALIZE_URI_FIELD_NAME, sizeof(PHP_URI_SERIALIZE_URI_FIELD_NAME) - 1, &tmp);
	zend_hash_next_index_insert(Z_ARRVAL_P(return_value), &arr);

	/* Serialize regular properties: second array */
	ZVAL_EMPTY_ARRAY(&arr);
	zend_hash_next_index_insert(Z_ARRVAL_P(return_value), &arr);
}

static void uri_unserialize(INTERNAL_FUNCTION_PARAMETERS)
{
	HashTable *data;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY_HT(data)
	ZEND_PARSE_PARAMETERS_END();

	php_uri_object *uri_object = php_uri_object_from_obj(Z_OBJ_P(ZEND_THIS));
	if (uri_object->uri != NULL) {
		/* Intentionally throw two exceptions for proper chaining. */
		zend_throw_error(NULL, "Cannot modify readonly object of class %s", ZSTR_VAL(uri_object->std.ce->name));
		zend_throw_exception_ex(NULL, 0, "Invalid serialization data for %s object", ZSTR_VAL(uri_object->std.ce->name));
		RETURN_THROWS();
	}

	/* Verify the expected number of elements, this implicitly ensures that no additional elements are present. */
	if (zend_hash_num_elements(data) != 2) {
		zend_throw_exception_ex(NULL, 0, "Invalid serialization data for %s object", ZSTR_VAL(uri_object->std.ce->name));
		RETURN_THROWS();
	}

	/* Unserialize state: "uri" key in the first array */
	zval *arr = zend_hash_index_find(data, 0);
	if (arr == NULL || Z_TYPE_P(arr) != IS_ARRAY) {
		zend_throw_exception_ex(NULL, 0, "Invalid serialization data for %s object", ZSTR_VAL(uri_object->std.ce->name));
		RETURN_THROWS();
	}

	/* Verify the expected number of elements inside the first array, this implicitly ensures that no additional elements are present. */
	if (zend_hash_num_elements(Z_ARRVAL_P(arr)) != 1) {
		zend_throw_exception_ex(NULL, 0, "Invalid serialization data for %s object", ZSTR_VAL(uri_object->std.ce->name));
		RETURN_THROWS();
	}

	zval *uri_zv = zend_hash_str_find(Z_ARRVAL_P(arr), ZEND_STRL(PHP_URI_SERIALIZE_URI_FIELD_NAME));
	if (uri_zv == NULL || Z_TYPE_P(uri_zv) != IS_STRING) {
		zend_throw_exception_ex(NULL, 0, "Invalid serialization data for %s object", ZSTR_VAL(uri_object->std.ce->name));
		RETURN_THROWS();
	}

	uri_object->uri = uri_object->parser->parse(Z_STRVAL_P(uri_zv), Z_STRLEN_P(uri_zv), NULL, NULL, true);
	if (uri_object->uri == NULL) {
		zend_throw_exception_ex(NULL, 0, "Invalid serialization data for %s object", ZSTR_VAL(uri_object->std.ce->name));
		RETURN_THROWS();
	}

	/* Unserialize regular properties: second array */
	arr = zend_hash_index_find(data, 1);
	if (arr == NULL || Z_TYPE_P(arr) != IS_ARRAY) {
		zend_throw_exception_ex(NULL, 0, "Invalid serialization data for %s object", ZSTR_VAL(uri_object->std.ce->name));
		RETURN_THROWS();
	}

	/* Verify that there is no regular property in the second array, because the URI classes have no properties and they are final. */
	if (zend_hash_num_elements(Z_ARRVAL_P(arr)) > 0) {
		zend_throw_exception_ex(NULL, 0, "Invalid serialization data for %s object", ZSTR_VAL(uri_object->std.ce->name));
		RETURN_THROWS();
	}
}

PHP_METHOD(Uri_Rfc3986_Uri, __unserialize)
{
	uri_unserialize(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

static HashTable *uri_get_debug_properties(php_uri_object *object)
{
	const HashTable *std_properties = zend_std_get_properties(&object->std);
	HashTable *result = zend_array_dup(std_properties);

	const php_uri_parser * const parser = object->parser;
	void * const uri = object->uri;

	if (UNEXPECTED(uri == NULL)) {
		return result;
	}

	zval tmp;
	if (parser->property_handler.scheme.read(uri, PHP_URI_COMPONENT_READ_MODE_RAW, &tmp) == SUCCESS) {
		zend_hash_update(result, ZSTR_KNOWN(ZEND_STR_SCHEME), &tmp);
	}

	if (parser->property_handler.username.read(uri, PHP_URI_COMPONENT_READ_MODE_RAW, &tmp) == SUCCESS) {
		zend_hash_update(result, ZSTR_KNOWN(ZEND_STR_USERNAME), &tmp);
	}

	if (parser->property_handler.password.read(uri, PHP_URI_COMPONENT_READ_MODE_RAW, &tmp) == SUCCESS) {
		zend_hash_update(result, ZSTR_KNOWN(ZEND_STR_PASSWORD), &tmp);
	}

	if (parser->property_handler.host.read(uri, PHP_URI_COMPONENT_READ_MODE_RAW, &tmp) == SUCCESS) {
		zend_hash_update(result, ZSTR_KNOWN(ZEND_STR_HOST), &tmp);
	}

	if (parser->property_handler.port.read(uri, PHP_URI_COMPONENT_READ_MODE_RAW, &tmp) == SUCCESS) {
		zend_hash_update(result, ZSTR_KNOWN(ZEND_STR_PORT), &tmp);
	}

	if (parser->property_handler.path.read(uri, PHP_URI_COMPONENT_READ_MODE_RAW, &tmp) == SUCCESS) {
		zend_hash_update(result, ZSTR_KNOWN(ZEND_STR_PATH), &tmp);
	}

	if (parser->property_handler.query.read(uri, PHP_URI_COMPONENT_READ_MODE_RAW, &tmp) == SUCCESS) {
		zend_hash_update(result, ZSTR_KNOWN(ZEND_STR_QUERY), &tmp);
	}

	if (parser->property_handler.fragment.read(uri, PHP_URI_COMPONENT_READ_MODE_RAW, &tmp) == SUCCESS) {
		zend_hash_update(result, ZSTR_KNOWN(ZEND_STR_FRAGMENT), &tmp);
	}

	return result;
}

PHP_METHOD(Uri_Rfc3986_Uri, __debugInfo)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_uri_object *uri_object = Z_URI_OBJECT_P(ZEND_THIS);

	RETURN_ARR(uri_get_debug_properties(uri_object));
}

PHP_METHOD(Uri_WhatWg_Url, percentEncode)
{
	zend_string *str;
	zend_object *mode_object;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(str)
		Z_PARAM_OBJ_OF_CLASS(mode_object, php_uri_ce_whatwg_url_percent_encoding_mode)
	ZEND_PARSE_PARAMETERS_END();

	zval *case_name = zend_enum_fetch_case_name(mode_object);
	zend_string *result;

	if (zend_string_equals_literal(Z_STR_P(case_name), "UserInfo")) {
		result = php_uri_parser_whatwg_percent_encode_user_info(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "Host")) {
		result = php_uri_parser_rfc3986_percent_encode_host(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "OpaqueHost")) {
		result = php_uri_parser_rfc3986_percent_encode_host(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "Path")) {
		result = php_uri_parser_rfc3986_percent_encode_path(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "OpaquePath")) {
		result = php_uri_parser_rfc3986_percent_encode_path_segment(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "PathSegment")) {
		result = php_uri_parser_rfc3986_percent_encode_path_segment(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "OpaquePathSegment")) {
		result = php_uri_parser_rfc3986_percent_encode_path_segment(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "Query")) {
		result = php_uri_parser_rfc3986_percent_encode_relative_reference_path(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "SpecialQuery")) {
		result = php_uri_parser_rfc3986_percent_encode_relative_reference_first_path_segment(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "FormQuery")) {
		result = php_uri_parser_rfc3986_percent_encode_query_or_fragment(str);
	} else if (zend_string_equals_literal(Z_STR_P(case_name), "Fragment")) {
		result = php_uri_parser_rfc3986_percent_encode_query_or_fragment(str);
	} else {
		ZEND_UNREACHABLE();
	}

	RETVAL_NEW_STR(result);
}

PHP_METHOD(Uri_WhatWg_Url, percentDecode)
{
	zend_string *str;
	zend_object *mode_object;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(str)
		Z_PARAM_OBJ_OF_CLASS(mode_object, php_uri_ce_whatwg_url_percent_encoding_mode)
	ZEND_PARSE_PARAMETERS_END();

	zend_throw_exception(NULL, "Not implemented", 0);
}

PHP_METHOD(Uri_WhatWg_Url, getScheme)
{
	php_uri_property_read_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_SCHEME, PHP_URI_COMPONENT_READ_MODE_NORMALIZED_ASCII);
}

PHP_METHOD(Uri_WhatWg_Url, withScheme)
{
	php_uri_property_write_str_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_SCHEME);
}

PHP_METHOD(Uri_WhatWg_Url, isSpecialScheme)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_uri_object *uri_object = Z_URI_OBJECT_P(ZEND_THIS);
	ZEND_ASSERT(uri_object->uri != NULL);

	RETVAL_BOOL(php_uri_parser_whatwg_is_special(uri_object->uri));
}

PHP_METHOD(Uri_WhatWg_Url, withUsername)
{
	php_uri_property_write_str_or_null_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_USERNAME);
}

PHP_METHOD(Uri_WhatWg_Url, withPassword)
{
	php_uri_property_write_str_or_null_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_PASSWORD);
}

PHP_METHOD(Uri_WhatWg_Url, getAsciiHost)
{
	php_uri_property_read_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_HOST, PHP_URI_COMPONENT_READ_MODE_NORMALIZED_ASCII);
}

PHP_METHOD(Uri_WhatWg_Url, getUnicodeHost)
{
	php_uri_property_read_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_HOST, PHP_URI_COMPONENT_READ_MODE_NORMALIZED_UNICODE);
}

PHP_METHOD(Uri_WhatWg_Url, getHostType)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_uri_object *uri_object = Z_URI_OBJECT_P(ZEND_THIS);
	ZEND_ASSERT(uri_object->uri != NULL);

	php_uri_parser_whatwg_host_type_read(uri_object->uri, PHP_URI_COMPONENT_READ_MODE_RAW, return_value);
}

PHP_METHOD(Uri_WhatWg_Url, getPathSegments)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_uri_object *uri_object = Z_URI_OBJECT_P(ZEND_THIS);
	ZEND_ASSERT(uri_object->uri != NULL);

	if (php_uri_parser_whatwg_path_segments_read(uri_object->uri, PHP_URI_COMPONENT_READ_MODE_RAW, return_value) == FAILURE) {
		zend_throw_exception_ex(php_uri_ce_error, 0, "The path component cannot be retrieved");
		RETURN_THROWS();
	}
}

PHP_METHOD(Uri_WhatWg_Url, withPathSegments)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_uri_object *uri_object = Z_URI_OBJECT_P(ZEND_THIS);
	ZEND_ASSERT(uri_object->uri != NULL);

	zend_throw_exception(NULL, "Not implemented", 0);
}

PHP_METHOD(Uri_WhatWg_Url, getQueryParams)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_uri_object *uri_object = Z_URI_OBJECT_P(ZEND_THIS);
	ZEND_ASSERT(uri_object->uri != NULL);

	if (UNEXPECTED(php_uri_parser_whatwg_query_params_read(uri_object->uri, PHP_URI_COMPONENT_READ_MODE_RAW, return_value) == FAILURE)) {
		RETURN_THROWS();
	}
}

PHP_METHOD(Uri_WhatWg_Url, withQueryParams)
{
	zval *zv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS_OR_NULL(zv, php_uri_ce_whatwg_url_query_params)
	ZEND_PARSE_PARAMETERS_END();

	php_uri_object *old_uri_object = php_uri_object_from_obj(Z_OBJ_P(ZEND_THIS));
	ZEND_ASSERT(old_uri_object->uri != NULL);

	zend_object *new_object = old_uri_object->std.handlers->clone_obj(&old_uri_object->std);
	if (new_object == NULL) {
		RETURN_THROWS();
	}

	/* Assign the object early. The engine will take care of destruction in
	 * case of an exception being thrown. */
	RETVAL_OBJ(new_object);

	php_uri_object *new_uri_object = php_uri_object_from_obj(new_object);
	ZEND_ASSERT(new_uri_object->uri != NULL);

	zval errors;
	ZVAL_UNDEF(&errors);

	if (UNEXPECTED(php_uri_parser_whatwg_query_params_write(new_uri_object->uri, zv, &errors) == FAILURE)) {
		zval_ptr_dtor(&errors);
		RETURN_THROWS();
	}

	ZEND_ASSERT(Z_ISUNDEF(errors));
}

PHP_METHOD(Uri_WhatWg_Url, getFragment)
{
	php_uri_property_read_helper(INTERNAL_FUNCTION_PARAM_PASSTHRU, PHP_URI_PROPERTY_NAME_FRAGMENT, PHP_URI_COMPONENT_READ_MODE_NORMALIZED_UNICODE);
}

PHP_METHOD(Uri_WhatWg_Url, equals)
{
	zend_object *that_object;
	zend_object *comparison_mode = NULL;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_OBJ_OF_CLASS(that_object, php_uri_ce_whatwg_url)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJ_OF_CLASS(comparison_mode, php_uri_ce_comparison_mode)
	ZEND_PARSE_PARAMETERS_END();

	uri_equals(INTERNAL_FUNCTION_PARAM_PASSTHRU, php_uri_object_from_obj(that_object), comparison_mode);
}

PHP_METHOD(Uri_WhatWg_Url, toUnicodeString)
{
	ZEND_PARSE_PARAMETERS_NONE();

	zend_object *this_object = Z_OBJ_P(ZEND_THIS);
	php_uri_object *uri_object = php_uri_object_from_obj(this_object);
	ZEND_ASSERT(uri_object->uri != NULL);

	RETURN_STR(uri_object->parser->to_string(uri_object->uri, PHP_URI_RECOMPOSITION_MODE_RAW_UNICODE, false));
}

PHP_METHOD(Uri_WhatWg_Url, toAsciiString)
{
	ZEND_PARSE_PARAMETERS_NONE();

	zend_object *this_object = Z_OBJ_P(ZEND_THIS);
	php_uri_object *uri_object = php_uri_object_from_obj(this_object);
	ZEND_ASSERT(uri_object->uri != NULL);

	RETURN_STR(uri_object->parser->to_string(uri_object->uri, PHP_URI_RECOMPOSITION_MODE_RAW_ASCII, false));
}

PHP_METHOD(Uri_WhatWg_Url, resolve)
{
	zend_string *uri_str;
	zval *errors = NULL;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(uri_str)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL(errors)
	ZEND_PARSE_PARAMETERS_END();

	php_uri_instantiate_uri(INTERNAL_FUNCTION_PARAM_PASSTHRU,
		uri_str, Z_URI_OBJECT_P(ZEND_THIS), true, false, errors);
}

PHP_METHOD(Uri_WhatWg_Url, __serialize)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_uri_object *this_object = Z_URI_OBJECT_P(ZEND_THIS);
	ZEND_ASSERT(this_object->uri != NULL);

	/* Serialize state: "uri" key in the first array */
	zend_string *uri_str = this_object->parser->to_string(this_object->uri, PHP_URI_RECOMPOSITION_MODE_RAW_ASCII, false);
	if (uri_str == NULL) {
		throw_cannot_recompose_uri_to_string(&this_object->std);
		RETURN_THROWS();
	}
	zval tmp;
	ZVAL_STR(&tmp, uri_str);

	array_init(return_value);

	zval arr;
	array_init(&arr);
	zend_hash_str_add_new(Z_ARRVAL(arr), ZEND_STRL(PHP_URI_SERIALIZE_URI_FIELD_NAME), &tmp);
	zend_hash_next_index_insert(Z_ARRVAL_P(return_value), &arr);

	/* Serialize regular properties: second array */
	ZVAL_EMPTY_ARRAY(&arr);
	zend_hash_next_index_insert(Z_ARRVAL_P(return_value), &arr);
}

PHP_METHOD(Uri_WhatWg_Url, __unserialize)
{
	uri_unserialize(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(Uri_WhatWg_Url, __debugInfo)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_uri_object *uri_object = Z_URI_OBJECT_P(ZEND_THIS);

	RETURN_ARR(uri_get_debug_properties(uri_object));
}
