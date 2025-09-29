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

#include "zend_enum.h"
#include "zend_smart_str.h"
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "php.h"
#include "Zend/zend_exceptions.h"

#include "php_uri_common.h"
#include "uri_parser_whatwg.h"
#include "uri_parser_rfc3986.h"

static void throw_cannot_recompose_uri_to_string(zend_object *object)
{
	zend_throw_exception_ex(php_uri_ce_error, 0, "Cannot recompose %s to a string", ZSTR_VAL(object->ce->name));
}

PHP_METHOD(Uri_Rfc3986_UriQueryParams, parseRfc3986)
{
	zend_string *query_str;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(query_str)
	ZEND_PARSE_PARAMETERS_END();

	object_init_ex(return_value, Z_CE_P(ZEND_THIS));
	php_uri_parser_rfc3986_uri_query_params_object *uri_query_params_object = Z_URI_QUERY_PARAMS_OBJECT_P(return_value);

	if (php_uri_rfc3986_uri_query_params_from_str(ZSTR_VAL(query_str), ZSTR_LEN(query_str), &uri_query_params_object->query_list, false) == FAILURE) {
		php_uri_rfc3986_uri_query_params_free(Z_URI_QUERY_PARAMS_OBJECT_P(return_value));
		RETURN_NULL();
	}

	uri_query_params_object->is_initialized = true;
}

PHP_METHOD(Uri_Rfc3986_UriQueryParams, parseFormData)
{
	zend_string *query_str;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(query_str)
	ZEND_PARSE_PARAMETERS_END();

	object_init_ex(return_value, Z_CE_P(ZEND_THIS));
	php_uri_parser_rfc3986_uri_query_params_object *uri_query_params_object = Z_URI_QUERY_PARAMS_OBJECT_P(return_value);

	if (php_uri_rfc3986_uri_query_params_from_str(ZSTR_VAL(query_str), ZSTR_LEN(query_str), &uri_query_params_object->query_list, false) == FAILURE) {
		php_uri_rfc3986_uri_query_params_free(Z_URI_QUERY_PARAMS_OBJECT_P(return_value));
		RETURN_NULL();
	}

	uri_query_params_object->is_initialized = true;
}

PHP_METHOD(Uri_Rfc3986_UriQueryParams, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();

	zval zv;
	object_init_ex(&zv, Z_CE_P(ZEND_THIS));
	php_uri_parser_rfc3986_uri_query_params_object *uri_query_params_object = Z_URI_QUERY_PARAMS_OBJECT_P(return_value);

	if (php_uri_rfc3986_uri_query_params_from_str("", 0, &uri_query_params_object->query_list, false) == FAILURE) {
		php_uri_rfc3986_uri_query_params_free(Z_URI_QUERY_PARAMS_OBJECT_P(&zv));
	}

	uri_query_params_object->is_initialized = true;
}

PHP_METHOD(Uri_Rfc3986_UriQueryParams, fromArray)
{
	HashTable *query_params_arr;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY_HT(query_params_arr)
	ZEND_PARSE_PARAMETERS_END();

	zend_throw_exception(NULL, "Not implemented", 0);
}

static zend_string *php_uri_build_name(const zend_string *name_array_prefix, zend_string *name)
{
	if (EXPECTED(name_array_prefix == NULL)) {
		ZEND_ASSERT(name != NULL);

		return name;
	}

	smart_str result = {0};
	smart_str_append(&result, name_array_prefix);

	/* List suffix */
	if (name == NULL) {
		smart_str_appendl(&result, "[", strlen("["));
		smart_str_appendl(&result, "]", strlen("]"));

		return smart_str_extract(&result);
	}

	smart_str_appendl(&result, "[", strlen("["));
	smart_str_append(&result, name);
	smart_str_appendl(&result, "]", strlen("]"));

	return smart_str_extract(&result);
}

static zend_result uri_query_params_append(php_uri_parser_rfc3986_uri_query_params_object *query_params_object,
	zend_string *name, const zval *value, const zend_string *name_array_prefix)
{
	zend_result result = FAILURE;
	zend_string *final_name = php_uri_build_name(name_array_prefix, name);

	switch (Z_TYPE_P(value)) {
		case IS_STRING: {
			result = php_uri_rfc3986_uri_query_params_append(&query_params_object->query_list,
				ZSTR_VAL(final_name), ZSTR_LEN(final_name), Z_STRVAL_P(value), Z_STRLEN_P(value)
			);
			break;
		}
		case IS_FALSE:
			ZEND_FALLTHROUGH;
		case IS_TRUE: {
			zend_string *bool_tmp = zend_string_init(Z_TYPE_P(value) == IS_FALSE ? "0" : "1", 1, false);

			result = php_uri_rfc3986_uri_query_params_append(&query_params_object->query_list,
				ZSTR_VAL(final_name), ZSTR_LEN(final_name), ZSTR_VAL(bool_tmp), ZSTR_LEN(bool_tmp)
			);

			zend_string_release_ex(bool_tmp, false);
			break;
		}
		case IS_LONG: {
			zend_string *long_tmp = zend_long_to_str(Z_LVAL_P(value));

			result = php_uri_rfc3986_uri_query_params_append(&query_params_object->query_list,
				ZSTR_VAL(final_name), ZSTR_LEN(final_name), ZSTR_VAL(long_tmp), ZSTR_LEN(long_tmp)
			);

			zend_string_release_ex(long_tmp, false);
			break;
		}
		case IS_DOUBLE: {
			zend_string *double_tmp = zend_double_to_str(Z_LVAL_P(value));

			result = php_uri_rfc3986_uri_query_params_append(&query_params_object->query_list,
				ZSTR_VAL(final_name), ZSTR_LEN(final_name), ZSTR_VAL(double_tmp), ZSTR_LEN(double_tmp)
			);

			zend_string_release_ex(double_tmp, false);
			break;
		}
		case IS_NULL: {
			result = php_uri_rfc3986_uri_query_params_append(&query_params_object->query_list,
			ZSTR_VAL(final_name), ZSTR_LEN(final_name), NULL, 0
			);
			break;
		}
		case IS_RESOURCE:
			zend_argument_value_error(2, "must not contain a resource");
			break;
		case IS_ARRAY: {
			if (zend_array_count(Z_ARR_P(value)) == 0) {
				result = php_uri_rfc3986_uri_query_params_append(&query_params_object->query_list,
				ZSTR_VAL(final_name), ZSTR_LEN(final_name), NULL, 0
				);
				break;
			}

			zend_string *key;
			zval *data;
			zend_ulong index;

			if (zend_array_is_list(Z_ARR_P(value))) {
				ZEND_HASH_FOREACH_VAL(Z_ARR_P(value), data) {
					result = uri_query_params_append(query_params_object, NULL, data, final_name);
					if (result == FAILURE) {
						return FAILURE;
					}
				} ZEND_HASH_FOREACH_END();
			} else {
				ZEND_HASH_FOREACH_KEY_VAL(Z_ARR_P(value), index, key, data) {
					if (key == NULL) {
						key = zend_ulong_to_str(index);
					}

					result = uri_query_params_append(query_params_object, key, data, final_name);
					if (result == FAILURE) {
						return FAILURE;
					}
				} ZEND_HASH_FOREACH_END();
			}
			break;
		}
		case IS_OBJECT:
			if (Z_OBJCE_P(value)->ce_flags & ZEND_ACC_ENUM && Z_OBJCE_P(value)->enum_backing_type != IS_UNDEF) {
				result = uri_query_params_append(query_params_object, final_name, zend_enum_fetch_case_value(Z_OBJ_P(value)), NULL);
			} else {
				zend_argument_value_error(2, "must not contain an object");
				break;
			}

			break;
	}

	zend_string_release(final_name);

	return result;
}

PHP_METHOD(Uri_Rfc3986_UriQueryParams, append)
{
	zend_string *name;
	zval *value;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(name)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();

	php_uri_parser_rfc3986_uri_query_params_object *uri_query_params_object = Z_URI_QUERY_PARAMS_OBJECT_P(ZEND_THIS);

	if (uri_query_params_append(uri_query_params_object, name, value, NULL) == FAILURE) {
		throw_cannot_recompose_uri_to_string(Z_OBJ_P(ZEND_THIS));
		RETURN_THROWS();
	}

	RETVAL_COPY(ZEND_THIS);
}

PHP_METHOD(Uri_Rfc3986_UriQueryParams, delete)
{
	zend_string *name;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();

	zend_throw_exception(NULL, "Not implemented", 0);
	RETVAL_COPY(ZEND_THIS);
}

PHP_METHOD(Uri_Rfc3986_UriQueryParams, deleteValue)
{
	zend_string *name;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();

	zend_throw_exception(NULL, "Not implemented", 0);
	RETVAL_COPY(ZEND_THIS);
}

PHP_METHOD(Uri_Rfc3986_UriQueryParams, has)
{
	zend_string *name;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();

	zend_throw_exception(NULL, "Not implemented", 0);
}

PHP_METHOD(Uri_Rfc3986_UriQueryParams, hasValue)
{
	zend_string *name;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();

	zend_throw_exception(NULL, "Not implemented", 0);
}

PHP_METHOD(Uri_Rfc3986_UriQueryParams, getFirst)
{
	zend_string *name;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();

	zend_throw_exception(NULL, "Not implemented", 0);
}

PHP_METHOD(Uri_Rfc3986_UriQueryParams, getLast)
{
	zend_string *name;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();

	zend_throw_exception(NULL, "Not implemented", 0);
}

PHP_METHOD(Uri_Rfc3986_UriQueryParams, getAll)
{
	zend_string *name = NULL;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR_OR_NULL(name)
	ZEND_PARSE_PARAMETERS_END();

	zend_throw_exception(NULL, "Not implemented", 0);
}

PHP_METHOD(Uri_Rfc3986_UriQueryParams, count)
{
	ZEND_PARSE_PARAMETERS_NONE();

	zend_throw_exception(NULL, "Not implemented", 0);
}

PHP_METHOD(Uri_Rfc3986_UriQueryParams, set)
{
	zend_string *name;
	zval *value;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(name)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();

	zend_throw_exception(NULL, "Not implemented", 0);
	RETVAL_COPY(ZEND_THIS);
}

PHP_METHOD(Uri_Rfc3986_UriQueryParams, sort)
{
	ZEND_PARSE_PARAMETERS_NONE();

	zend_throw_exception(NULL, "Not implemented", 0);
	RETVAL_COPY(ZEND_THIS);
}

PHP_METHOD(Uri_Rfc3986_UriQueryParams, toRfc3986String)
{
	ZEND_PARSE_PARAMETERS_NONE();

	const php_uri_parser_rfc3986_uri_query_params_object *uri_query_params_object = Z_URI_QUERY_PARAMS_OBJECT_P(ZEND_THIS);

	zend_string *result = php_uri_rfc3986_uri_query_params_to_string(uri_query_params_object->query_list);
	if (result == NULL) {
		throw_cannot_recompose_uri_to_string(Z_OBJ_P(ZEND_THIS));
		RETURN_THROWS();
	}

	ZVAL_STR(return_value, result);
}

PHP_METHOD(Uri_Rfc3986_UriQueryParams, toFormDataString)
{
	ZEND_PARSE_PARAMETERS_NONE();

	const php_uri_parser_rfc3986_uri_query_params_object *uri_query_params_object = Z_URI_QUERY_PARAMS_OBJECT_P(ZEND_THIS);

	zend_string *result = php_uri_rfc3986_uri_query_params_to_string(uri_query_params_object->query_list);
	if (result == NULL) {
		throw_cannot_recompose_uri_to_string(Z_OBJ_P(ZEND_THIS));
		RETURN_THROWS();
	}

	ZVAL_STR(return_value, result);
}

PHP_METHOD(Uri_Rfc3986_UriQueryParams, __serialize)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_uri_parser_rfc3986_uri_query_params_object *uri_query_params_object = Z_URI_QUERY_PARAMS_OBJECT_P(ZEND_THIS);

	/* Serialize state: "query" key in the first array */
	zend_string *uri_query_param_str = php_uri_rfc3986_uri_query_params_to_string(uri_query_params_object->query_list);
	if (uri_query_param_str == NULL) {
		throw_cannot_recompose_uri_to_string(Z_OBJ_P(ZEND_THIS));
		RETURN_THROWS();
	}
	zval tmp;
	ZVAL_STR(&tmp, uri_query_param_str);

	array_init(return_value);

	zval arr;
	array_init(&arr);
	zend_hash_str_add_new(Z_ARRVAL(arr), PHP_URI_SERIALIZE_URI_QUERY_PARAM_FIELD_NAME, sizeof(PHP_URI_SERIALIZE_URI_QUERY_PARAM_FIELD_NAME) - 1, &tmp);
	zend_hash_next_index_insert(Z_ARRVAL_P(return_value), &arr);

	/* Serialize regular properties: second array */
	ZVAL_ARR(&arr, uri_query_params_object->std.handlers->get_properties(&uri_query_params_object->std));
	Z_TRY_ADDREF(arr);
	zend_hash_next_index_insert(Z_ARRVAL_P(return_value), &arr);
}

static void uri_query_params_unserialize(INTERNAL_FUNCTION_PARAMETERS)
{
	HashTable *data;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY_HT(data)
	ZEND_PARSE_PARAMETERS_END();

	php_uri_parser_rfc3986_uri_query_params_object *uri_query_params_object = Z_URI_QUERY_PARAMS_OBJECT_P(ZEND_THIS);
	if (uri_query_params_object->query_list != NULL) {
		/* Intentionally throw two exceptions for proper chaining. */
		zend_throw_error(NULL, "Cannot modify readonly object of class %s", ZSTR_VAL(uri_query_params_object->std.ce->name));
		zend_throw_exception_ex(NULL, 0, "Invalid serialization data for %s object", ZSTR_VAL(uri_query_params_object->std.ce->name));
		RETURN_THROWS();
	}

	/* Verify the expected number of elements, this implicitly ensures that no additional elements are present. */
	if (zend_hash_num_elements(data) != 2) {
		zend_throw_exception_ex(NULL, 0, "Invalid serialization data for %s object", ZSTR_VAL(uri_query_params_object->std.ce->name));
		RETURN_THROWS();
	}

	/* Unserialize state: "query" key in the first array */
	zval *arr = zend_hash_index_find(data, 0);
	if (arr == NULL || Z_TYPE_P(arr) != IS_ARRAY) {
		zend_throw_exception_ex(NULL, 0, "Invalid serialization data for %s object", ZSTR_VAL(uri_query_params_object->std.ce->name));
		RETURN_THROWS();
	}

	/* Verify the expected number of elements inside the first array, this implicitly ensures that no additional elements are present. */
	if (zend_hash_num_elements(Z_ARRVAL_P(arr)) != 1) {
		zend_throw_exception_ex(NULL, 0, "Invalid serialization data for %s object", ZSTR_VAL(uri_query_params_object->std.ce->name));
		RETURN_THROWS();
	}

	const zval *uri_query_param_zv = zend_hash_str_find_ind(Z_ARRVAL_P(arr), ZEND_STRL(PHP_URI_SERIALIZE_URI_QUERY_PARAM_FIELD_NAME));
	if (uri_query_param_zv == NULL || Z_TYPE_P(uri_query_param_zv) != IS_STRING) {
		zend_throw_exception_ex(NULL, 0, "Invalid serialization data for %s object", ZSTR_VAL(uri_query_params_object->std.ce->name));
		RETURN_THROWS();
	}

	const zend_result result = php_uri_rfc3986_uri_query_params_from_str(
		Z_STRVAL_P(uri_query_param_zv),
		Z_STRLEN_P(uri_query_param_zv),
		&uri_query_params_object->query_list,
		false
	);

	if (result == FAILURE) {
		RETURN_THROWS();
	}

	uri_query_params_object->is_initialized = true;

	/* Unserialize regular properties: second array */
	arr = zend_hash_index_find(data, 1);
	if (arr == NULL || Z_TYPE_P(arr) != IS_ARRAY) {
		zend_throw_exception_ex(NULL, 0, "Invalid serialization data for %s object", ZSTR_VAL(uri_query_params_object->std.ce->name));
		RETURN_THROWS();
	}

	/* Verify that there is no regular property in the second array, because the query param classes have no properties and they are final. */
	if (zend_hash_num_elements(Z_ARRVAL_P(arr)) > 0) {
		zend_throw_exception_ex(NULL, 0, "Invalid serialization data for %s object", ZSTR_VAL(uri_query_params_object->std.ce->name));
		RETURN_THROWS();
	}
}

PHP_METHOD(Uri_Rfc3986_UriQueryParams, __unserialize)
{
	uri_query_params_unserialize(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(Uri_Rfc3986_UriQueryParams, __debugInfo)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_uri_parser_rfc3986_uri_query_params_object *uri_query_params_object = Z_URI_QUERY_PARAMS_OBJECT_P(ZEND_THIS);

	RETURN_ARR(php_uri_rfc3986_uri_query_params_get_debug_properties(uri_query_params_object));
}

PHP_METHOD(Uri_WhatWg_UrlQueryParams, parse)
{
	zend_string *query_str;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(query_str)
	ZEND_PARSE_PARAMETERS_END();

	object_init_ex(return_value, Z_CE_P(ZEND_THIS));
	php_uri_parser_whatwg_url_query_params_object *url_query_params_object = Z_URL_QUERY_PARAMS_OBJECT_P(return_value);

	url_query_params_object->query_params = php_uri_whatwg_url_query_params_from_str(
		ZSTR_VAL(query_str),
		ZSTR_LEN(query_str),
		true
	);

	if (url_query_params_object->query_params == NULL) {
		RETURN_NULL();
	}

	url_query_params_object->is_initialized = true;
}

PHP_METHOD(Uri_WhatWg_UrlQueryParams, __construct)
{
	ZEND_PARSE_PARAMETERS_NONE();

	zval zv;
	object_init_ex(&zv, Z_CE_P(ZEND_THIS));
	php_uri_parser_whatwg_url_query_params_object *url_query_params_object = Z_URL_QUERY_PARAMS_OBJECT_P(&zv);

	url_query_params_object->query_params = php_uri_whatwg_url_query_params_from_str(
		"",
		0,
		true
	);

	if (url_query_params_object->query_params == NULL) {
		return;
	}

	url_query_params_object->is_initialized = true;
}

PHP_METHOD(Uri_WhatWg_UrlQueryParams, fromArray)
{
	HashTable *query_params_arr;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY_HT(query_params_arr)
	ZEND_PARSE_PARAMETERS_END();

	zend_throw_exception(NULL, "Not implemented", 0);
}

static zend_result php_uri_whatwg_query_params_append(const php_uri_parser_whatwg_url_query_params_object *query_params_object,
	zend_string *name, const zval *value, const zend_string *name_array_prefix)
{
	zend_result result = FAILURE;
	const zend_string *tmp;
	zend_string *final_name = php_uri_build_name(name_array_prefix, name);

	switch (Z_TYPE_P(value)) {
		case IS_STRING:
			result = php_uri_whatwg_url_query_params_append(query_params_object->query_params,
			ZSTR_VAL(final_name), ZSTR_LEN(final_name), Z_STRVAL_P(value), Z_STRLEN_P(value)
			);
			break;
		case IS_FALSE:
			ZEND_FALLTHROUGH;
		case IS_TRUE: {
			zend_string *bool_tmp = zend_string_init(Z_TYPE_P(value) == IS_FALSE ? "0" : "1", 1, false);
			result = php_uri_whatwg_url_query_params_append(query_params_object->query_params,
			ZSTR_VAL(final_name), ZSTR_LEN(final_name), ZSTR_VAL(bool_tmp), ZSTR_LEN(bool_tmp)
			);
			zend_string_release_ex(bool_tmp, false);
			break;
		}
		case IS_LONG: {
			zend_string *long_tmp = zend_long_to_str(Z_LVAL_P(value));
			result = php_uri_whatwg_url_query_params_append(query_params_object->query_params,
			ZSTR_VAL(final_name), ZSTR_LEN(final_name), ZSTR_VAL(long_tmp), ZSTR_LEN(long_tmp)
			);
			zend_string_release_ex(long_tmp, false);
			break;
		}
		case IS_DOUBLE: {
			zend_string *double_tmp = zend_double_to_str(Z_LVAL_P(value));
			result = php_uri_whatwg_url_query_params_append(query_params_object->query_params,
			ZSTR_VAL(final_name), ZSTR_LEN(final_name), ZSTR_VAL(double_tmp), ZSTR_LEN(double_tmp)
			);
			zend_string_release_ex(double_tmp, false);
			break;
		}
		case IS_NULL:
			tmp = zend_empty_string;

			result = php_uri_whatwg_url_query_params_append(query_params_object->query_params,
			ZSTR_VAL(final_name), ZSTR_LEN(final_name), ZSTR_VAL(tmp), ZSTR_LEN(tmp)
			);
			break;
		case IS_RESOURCE:
			zend_argument_value_error(2, "must not contain a resource");
			break;
		case IS_ARRAY: {
			zend_string *key;
			zval *data;
			zend_ulong index;

			if (zend_array_count(Z_ARR_P(value)) == 0) {
				// TODO what to do here?
				result = php_uri_whatwg_url_query_params_append(query_params_object->query_params,
			ZSTR_VAL(final_name), ZSTR_LEN(final_name), "", 0
				);
				break;
			}

			if (zend_array_is_list(Z_ARR_P(value))) {
				ZEND_HASH_FOREACH_VAL(Z_ARR_P(value), data) {
					result = php_uri_whatwg_query_params_append(query_params_object, NULL, data, final_name);
					if (result == FAILURE) {
						return FAILURE;
					}
				} ZEND_HASH_FOREACH_END();
			} else {
				ZEND_HASH_FOREACH_KEY_VAL(Z_ARR_P(value), index, key, data) {
					if (key == NULL) {
						key = zend_ulong_to_str(index);
					}

					result = php_uri_whatwg_query_params_append(query_params_object, key, data, final_name);
					if (result == FAILURE) {
						return FAILURE;
					}
				} ZEND_HASH_FOREACH_END();
			}
			break;
		}
		case IS_OBJECT:
			if (Z_OBJCE_P(value)->ce_flags & ZEND_ACC_ENUM && Z_OBJCE_P(value)->enum_backing_type != IS_UNDEF) {
				result = php_uri_whatwg_query_params_append(query_params_object, final_name, zend_enum_fetch_case_value(Z_OBJ_P(value)), NULL);
			} else {
				zend_argument_value_error(2, "must not contain an object");
			}

			break;
	}

	zend_string_release(final_name);

	return result;
}

PHP_METHOD(Uri_WhatWg_UrlQueryParams, append)
{
	zend_string *name;
	zval *value;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(name)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();

	const php_uri_parser_whatwg_url_query_params_object *uri_query_params_object = Z_URL_QUERY_PARAMS_OBJECT_P(ZEND_THIS);

	if (php_uri_whatwg_query_params_append(uri_query_params_object, name, value, NULL) == FAILURE) {
		throw_cannot_recompose_uri_to_string(Z_OBJ_P(ZEND_THIS));
		RETURN_THROWS();
	}

	RETVAL_COPY(ZEND_THIS);
}

PHP_METHOD(Uri_WhatWg_UrlQueryParams, delete)
{
	zend_string *name;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();

	const php_uri_parser_whatwg_url_query_params_object *uri_query_params_object = Z_URL_QUERY_PARAMS_OBJECT_P(ZEND_THIS);

	php_uri_whatwg_url_query_params_delete(uri_query_params_object->query_params, ZSTR_VAL(name), ZSTR_LEN(name), NULL, 0);

	RETVAL_COPY(ZEND_THIS);
}

PHP_METHOD(Uri_WhatWg_UrlQueryParams, deleteValue)
{
	zend_string *name, *value;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(name)
		Z_PARAM_STR(value)
	ZEND_PARSE_PARAMETERS_END();

	const php_uri_parser_whatwg_url_query_params_object *uri_query_params_object = Z_URL_QUERY_PARAMS_OBJECT_P(ZEND_THIS);

	php_uri_whatwg_url_query_params_delete(uri_query_params_object->query_params,
		ZSTR_VAL(name), ZSTR_LEN(name), ZSTR_VAL(value), ZSTR_LEN(value)
	);

	RETVAL_COPY(ZEND_THIS);
}

PHP_METHOD(Uri_WhatWg_UrlQueryParams, has)
{
	zend_string *name;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();

	const php_uri_parser_whatwg_url_query_params_object *uri_query_params_object = Z_URL_QUERY_PARAMS_OBJECT_P(ZEND_THIS);

	RETURN_BOOL(php_uri_whatwg_url_query_params_exists(uri_query_params_object->query_params,
		ZSTR_VAL(name), ZSTR_LEN(name), NULL, 0
	));
}

PHP_METHOD(Uri_WhatWg_UrlQueryParams, hasValue)
{
	zend_string *name, *value;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(name)
		Z_PARAM_STR(value)
	ZEND_PARSE_PARAMETERS_END();

	const php_uri_parser_whatwg_url_query_params_object *uri_query_params_object = Z_URL_QUERY_PARAMS_OBJECT_P(ZEND_THIS);

	RETURN_BOOL(php_uri_whatwg_url_query_params_exists(uri_query_params_object->query_params,
		ZSTR_VAL(name), ZSTR_LEN(name), ZSTR_VAL(value), ZSTR_LEN(value)
	));
}

PHP_METHOD(Uri_WhatWg_UrlQueryParams, getFirst)
{
	zend_string *name;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();

	const php_uri_parser_whatwg_url_query_params_object *uri_query_params_object = Z_URL_QUERY_PARAMS_OBJECT_P(ZEND_THIS);

	zend_string *result = php_uri_whatwg_url_query_params_get_first(uri_query_params_object->query_params, ZSTR_VAL(name), ZSTR_LEN(name));
	if (result == NULL) {
		RETURN_NULL();
	}

	RETVAL_STR(result);
}

PHP_METHOD(Uri_WhatWg_UrlQueryParams, getLast)
{
	zend_string *name;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();

	zend_throw_exception(NULL, "Not implemented", 0);
}

PHP_METHOD(Uri_WhatWg_UrlQueryParams, getAll)
{
	zend_string *name = NULL;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR_OR_NULL(name)
	ZEND_PARSE_PARAMETERS_END();

	zend_throw_exception(NULL, "Not implemented", 0);
}

PHP_METHOD(Uri_WhatWg_UrlQueryParams, count)
{
	ZEND_PARSE_PARAMETERS_NONE();

	zend_throw_exception(NULL, "Not implemented", 0);
}

PHP_METHOD(Uri_WhatWg_UrlQueryParams, set)
{
	zend_string *name;
	zval *value;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(name)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();

	zend_throw_exception(NULL, "Not implemented", 0);
	RETVAL_COPY(ZEND_THIS);
}

PHP_METHOD(Uri_WhatWg_UrlQueryParams, sort)
{
	ZEND_PARSE_PARAMETERS_NONE();

	zend_throw_exception(NULL, "Not implemented", 0);
	RETVAL_COPY(ZEND_THIS);
}

PHP_METHOD(Uri_WhatWg_UrlQueryParams, toString)
{
	ZEND_PARSE_PARAMETERS_NONE();

	const php_uri_parser_whatwg_url_query_params_object *uri_query_params_object = Z_URL_QUERY_PARAMS_OBJECT_P(ZEND_THIS);

	zend_string *result = php_uri_whatwg_url_query_params_to_string(uri_query_params_object->query_params);
	if (result == NULL) {
		throw_cannot_recompose_uri_to_string(Z_OBJ_P(ZEND_THIS));
		RETURN_THROWS();
	}

	ZVAL_STR(return_value, result);
}

PHP_METHOD(Uri_WhatWg_UrlQueryParams, __serialize)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_uri_parser_whatwg_url_query_params_object *uri_query_params_object = Z_URL_QUERY_PARAMS_OBJECT_P(ZEND_THIS);

	/* Serialize state: "query" key in the first array */
	zend_string *url_query_param_str = php_uri_whatwg_url_query_params_to_string(uri_query_params_object->query_params);
	if (url_query_param_str == NULL) {
		throw_cannot_recompose_uri_to_string(Z_OBJ_P(ZEND_THIS));
		RETURN_THROWS();
	}
	zval tmp;
	ZVAL_STR(&tmp, url_query_param_str);

	array_init(return_value);

	zval arr;
	array_init(&arr);
	zend_hash_str_add_new(Z_ARRVAL(arr), PHP_URI_SERIALIZE_URI_QUERY_PARAM_FIELD_NAME, sizeof(PHP_URI_SERIALIZE_URI_QUERY_PARAM_FIELD_NAME) - 1, &tmp);
	zend_hash_next_index_insert(Z_ARRVAL_P(return_value), &arr);

	/* Serialize regular properties: second array */
	ZVAL_ARR(&arr, uri_query_params_object->std.handlers->get_properties(&uri_query_params_object->std));
	Z_TRY_ADDREF(arr);
	zend_hash_next_index_insert(Z_ARRVAL_P(return_value), &arr);
}

PHP_METHOD(Uri_WhatWg_UrlQueryParams, __unserialize)
{
	HashTable *data;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY_HT(data)
	ZEND_PARSE_PARAMETERS_END();

	zend_throw_exception(NULL, "Not implemented", 0);
}

PHP_METHOD(Uri_WhatWg_UrlQueryParams, __debugInfo)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_uri_parser_whatwg_url_query_params_object *url_query_params_object = Z_URL_QUERY_PARAMS_OBJECT_P(ZEND_THIS);

	RETURN_ARR(php_uri_whatwg_url_query_params_get_debug_properties(url_query_params_object));
}

zend_object *php_uri_object_create_rfc3986_uri_query_params(zend_class_entry *ce)
{
	php_uri_parser_rfc3986_uri_query_params_object *uri_query_params_object = zend_object_alloc(sizeof(*php_uri_ce_rfc3986_uri_query_params), ce);

	zend_object_std_init(&uri_query_params_object->std, ce);
	object_properties_init(&uri_query_params_object->std, ce);

	uri_query_params_object->query_list = NULL;
	uri_query_params_object->is_initialized = false;

	return &uri_query_params_object->std;
}

zend_object *php_uri_object_create_whatwg_url_query_params(zend_class_entry *ce)
{
	php_uri_parser_whatwg_url_query_params_object *url_query_params_object = zend_object_alloc(sizeof(*url_query_params_object), ce);

	zend_object_std_init(&url_query_params_object->std, ce);
	object_properties_init(&url_query_params_object->std, ce);

	url_query_params_object->query_params = NULL;
	url_query_params_object->is_initialized = false;

	return &url_query_params_object->std;
}
