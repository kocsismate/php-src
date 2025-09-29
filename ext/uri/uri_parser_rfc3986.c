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

#include "php.h"
#include "uri_parser_rfc3986.h"
#include "php_uri_common.h"
#include "Zend/zend_enum.h"
#include "Zend/zend_smart_str.h"
#include "Zend/zend_exceptions.h"

#include <uriparser/Uri.h>

struct php_uri_parser_rfc3986_uris {
	UriUriA uri;
	UriUriA normalized_uri;
	bool normalized_uri_initialized;
};

static void *php_uri_parser_rfc3986_memory_manager_malloc(UriMemoryManager *memory_manager, size_t size)
{
	return emalloc(size);
}

static void *php_uri_parser_rfc3986_memory_manager_calloc(UriMemoryManager *memory_manager, size_t nmemb, size_t size)
{
	return ecalloc(nmemb, size);
}

static void *php_uri_parser_rfc3986_memory_manager_realloc(UriMemoryManager *memory_manager, void *ptr, size_t size)
{
	return erealloc(ptr, size);
}

static void *php_uri_parser_rfc3986_memory_manager_reallocarray(UriMemoryManager *memory_manager, void *ptr, size_t nmemb, size_t size)
{
	return safe_erealloc(ptr, nmemb, size, 0);
}

static void php_uri_parser_rfc3986_memory_manager_destroy(UriMemoryManager *memory_manager, void *ptr)
{
	efree(ptr);
}

static const UriMemoryManager php_uri_parser_rfc3986_memory_manager = {
	.malloc = php_uri_parser_rfc3986_memory_manager_malloc,
	.calloc = php_uri_parser_rfc3986_memory_manager_calloc,
	.realloc = php_uri_parser_rfc3986_memory_manager_realloc,
	.reallocarray = php_uri_parser_rfc3986_memory_manager_reallocarray,
	.free = php_uri_parser_rfc3986_memory_manager_destroy,
	.userData = NULL,
};

/* The library expects a pointer to a non-const UriMemoryManager, but does
 * not actually modify it (and neither does our implementation). Use a
 * const struct with a non-const pointer for convenience. */
static UriMemoryManager* const mm = (UriMemoryManager*)&php_uri_parser_rfc3986_memory_manager;

#define PERCENT_ENCODE_COLON_CASE(retval) \
	case ':': \
		smart_str_appends(&retval, "%3A"); \
		break;

#define PERCENT_ENCODE_SLASH_CASE(retval) \
	case '/': \
		smart_str_appends(&retval, "%2F"); \
		break;

#define PERCENT_ENCODE_AT_CASE(retval) \
	case '@': \
		smart_str_appends(&retval, "%40"); \
		break;

#define PERCENT_ENCODE_QUESTION_MARK_CASE(retval) \
	case '?': \
		smart_str_appends(&retval, "%3F"); \
		break;

#define PERCENT_ENCODE_REST_OF_GEN_DELIMS_CASES(retval) \
	case '#': \
		smart_str_appends(&retval, "%23"); \
		break; \
	case '[': \
		smart_str_appends(&retval, "%5B"); \
		break; \
	case ']': \
		smart_str_appends(&retval, "%5D"); \
		break;

#define PERCENT_ENCODE_PERCENT_CASE(retval) \
	case '%': \
		smart_str_appends(&retval, "%25"); \
		break;

#define PERCENT_ENCODE_PERCENT_AS_PLUS_CASE(retval) \
	case '%': \
		smart_str_appends(&retval, "+"); \
		break;

#define PERCENT_ENCODE_OTHER_CHARACTERS_CASES(retval) \
	case ' ': \
		smart_str_appends(&retval, "%20"); \
		break; \
	case '"': \
		smart_str_appends(&retval, "%22"); \
		break; \
	case '<': \
		smart_str_appends(&retval, "%3C"); \
		break; \
	case '>': \
		smart_str_appends(&retval, "%3E"); \
		break; \
	case '\\': \
		smart_str_appends(&retval, "%5C"); \
		break; \
	case '^': \
		smart_str_appends(&retval, "%5E"); \
		break; \
	case '`': \
		smart_str_appends(&retval, "%60"); \
		break; \
	case '{': \
		smart_str_appends(&retval, "%7B"); \
		break; \
	case '|': \
		smart_str_appends(&retval, "%7C"); \
		break; \
	case '}': \
		smart_str_appends(&retval, "%7D"); \
		break;

#define PERCENT_ENCODE_GEN_DELIMS_CASES(retval) \
	PERCENT_ENCODE_COLON_CASE(retval) \
	PERCENT_ENCODE_SLASH_CASE(retval) \
	PERCENT_ENCODE_QUESTION_MARK_CASE(retval) \
	PERCENT_ENCODE_AT_CASE(retval) \
	PERCENT_ENCODE_REST_OF_GEN_DELIMS_CASES(retval)

#define PERCENT_ENCODE_SUB_DELIMS_CASES(retval) \
	case '!': \
		smart_str_appends(&retval, "%21"); \
		break; \
	case '$': \
		smart_str_appends(&retval, "%24"); \
		break; \
	case '&': \
		smart_str_appends(&retval, "%26"); \
		break; \
	case '\'': \
		smart_str_appends(&retval, "%27"); \
		break; \
	case '(': \
		smart_str_appends(&retval, "%28"); \
		break; \
	case ')': \
		smart_str_appends(&retval, "%29"); \
		break; \
	case '*': \
		smart_str_appends(&retval, "%2A"); \
		break; \
	case '+': \
		smart_str_appends(&retval, "%2B"); \
		break; \
	case ',': \
		smart_str_appends(&retval, "%2C"); \
		break; \
	case ';': \
		smart_str_appends(&retval, "%3B"); \
		break; \
	case '=': \
		smart_str_appends(&retval, "%3D"); \
		break;

ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_rfc3986_percent_encode_user_info(const zend_string *str)
{
	/* userinfo      = *( unreserved / pct-encoded / sub-delims / ":" ) */
	smart_str retval = {0};

	for (int i = 0; i < ZSTR_LEN(str); i++) {
		switch (ZSTR_VAL(str)[i]) {
			/* PERCENT_ENCODE_COLON_CASE(retval) */
			PERCENT_ENCODE_SLASH_CASE(retval)
			PERCENT_ENCODE_QUESTION_MARK_CASE(retval)
			PERCENT_ENCODE_AT_CASE(retval)
			PERCENT_ENCODE_REST_OF_GEN_DELIMS_CASES(retval)
			PERCENT_ENCODE_PERCENT_CASE(retval)
			PERCENT_ENCODE_OTHER_CHARACTERS_CASES(retval)
			default:
				smart_str_appendc(&retval, ZSTR_VAL(str)[i]);
				break;
		}
	}

	return smart_str_extract(&retval);
}

ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_rfc3986_percent_encode_host(const zend_string *str)
{
	/* host         = IP-literal / IPv4address / reg-name
	 * IP-literal   = "[" ( IPv6address / IPvFuture  ) "]"
	 * IPvFuture    = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
	 * IPv6address  =                            6( h16 ":" ) ls32
	 *				/                       "::" 5( h16 ":" ) ls32
	 *				/ [               h16 ] "::" 4( h16 ":" ) ls32
	 *				/ [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
	 *				/ [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
	 *				/ [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
	 *				/ [ *4( h16 ":" ) h16 ] "::"              ls32
	 *				/ [ *5( h16 ":" ) h16 ] "::"              h16
	 *				/ [ *6( h16 ":" ) h16 ] "::"
	 * h16           = 1*4HEXDIG
	 * ls32          = ( h16 ":" h16 ) / IPv4address
	 * IPv4address   = dec-octet "." dec-octet "." dec-octet "." dec-octet
	 * dec-octet     = DIGIT                 ; 0-9
	 *				/ %x31-39 DIGIT         ; 10-99
	 *				/ "1" 2DIGIT            ; 100-199
	 *				/ "2" %x30-34 DIGIT     ; 200-249
	 *				/ "25" %x30-35          ; 250-255
	 * reg-name      = *( unreserved / pct-encoded / sub-delims )
	 */
	smart_str retval = {0};

	/* Is IP-literal */
	if (ZSTR_LEN(str) > 0 && ZSTR_VAL(str)[0] == '[') {
		smart_str_append(&retval, str);
	} else { /* reg-name or IPv4address -> only reg-name supports percent-encoding */
		for (int i = 0; i < ZSTR_LEN(str); i++) {
			switch (ZSTR_VAL(str)[i]) {
				PERCENT_ENCODE_GEN_DELIMS_CASES(retval)
				PERCENT_ENCODE_PERCENT_CASE(retval)
				PERCENT_ENCODE_OTHER_CHARACTERS_CASES(retval)
				default:
					smart_str_appendc(&retval, ZSTR_VAL(str)[i]);
					break;
			}
		}
	}

	return smart_str_extract(&retval);
}

ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_rfc3986_percent_encode_path(const zend_string *str)
{
	/* path-abempty  = *( "/" segment )
	 * path-absolute = "/" [ segment-nz *( "/" segment ) ]
	 * path-rootless = segment-nz *( "/" segment )
	 * path-empty    = 0<pchar>
	 * segment       = *pchar
	 * segment-nz    = 1*pchar
	 * pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"
	 */
	smart_str retval = {0};

	for (int i = 0; i < ZSTR_LEN(str); i++) {
		switch (ZSTR_VAL(str)[i]) {
			/* PERCENT_ENCODE_COLON_CASE(retval) */
			/* PERCENT_ENCODE_SLASH_CASE(retval) */
			PERCENT_ENCODE_QUESTION_MARK_CASE(retval)
			/* PERCENT_ENCODE_AT_CASE(retval) */
			PERCENT_ENCODE_REST_OF_GEN_DELIMS_CASES(retval)
			PERCENT_ENCODE_PERCENT_CASE(retval)
			PERCENT_ENCODE_OTHER_CHARACTERS_CASES(retval)
			default:
				smart_str_appendc(&retval, ZSTR_VAL(str)[i]);
				break;
		}
	}

	return smart_str_extract(&retval);
}

ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_rfc3986_percent_encode_path_segment(const zend_string *str)
{
	/* segment       = *pchar
	 * segment-nz    = 1*pchar
	 * pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"
	 */
	smart_str retval = {0};

	for (int i = 0; i < ZSTR_LEN(str); i++) {
		switch (ZSTR_VAL(str)[i]) {
			/* PERCENT_ENCODE_COLON_CASE(retval) */
			PERCENT_ENCODE_SLASH_CASE(retval)
			PERCENT_ENCODE_QUESTION_MARK_CASE(retval)
			/* PERCENT_ENCODE_AT_CASE(retval) */
			PERCENT_ENCODE_REST_OF_GEN_DELIMS_CASES(retval)
			PERCENT_ENCODE_PERCENT_CASE(retval)
			PERCENT_ENCODE_OTHER_CHARACTERS_CASES(retval)
			default:
			smart_str_appendc(&retval, ZSTR_VAL(str)[i]);
			break;
		}
	}

	return smart_str_extract(&retval);
}

ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_rfc3986_percent_encode_relative_reference_path(const zend_string *str)
{
	/* path-noscheme = segment-nz-nc *( "/" segment )
	 * segment-nz-nc = 1*( unreserved / pct-encoded / sub-delims / "@" )
	 * pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"
	 */
	smart_str retval = {0};
	bool first_segment = true;

	for (int i = 0; i < ZSTR_LEN(str); i++) {
		switch (ZSTR_VAL(str)[i]) {
			case '/':
				first_segment = false;
				break;
			case ':':
				if (first_segment) {
					smart_str_appends(&retval, "%3A");
				} else {
					smart_str_appendc(&retval, ZSTR_VAL(str)[i]);
				}
				break;
				/* PERCENT_ENCODE_SLASH_CASE(retval) */
				PERCENT_ENCODE_QUESTION_MARK_CASE(retval)
				/* PERCENT_ENCODE_AT_CASE(retval) */
				PERCENT_ENCODE_REST_OF_GEN_DELIMS_CASES(retval)
				PERCENT_ENCODE_PERCENT_CASE(retval)
				PERCENT_ENCODE_OTHER_CHARACTERS_CASES(retval)
				default:
					smart_str_appendc(&retval, ZSTR_VAL(str)[i]);
					break;
		}
	}

	return smart_str_extract(&retval);
}

ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_rfc3986_percent_encode_relative_reference_first_path_segment(const zend_string *str)
{
	/* segment-nz-nc = 1*( unreserved / pct-encoded / sub-delims / "@" ) */
	smart_str retval = {0};

	for (int i = 0; i < ZSTR_LEN(str); i++) {
		switch (ZSTR_VAL(str)[i]) {
			PERCENT_ENCODE_COLON_CASE(retval)
			PERCENT_ENCODE_SLASH_CASE(retval)
			PERCENT_ENCODE_QUESTION_MARK_CASE(retval)
			/* PERCENT_ENCODE_AT_CASE(retval) */
			PERCENT_ENCODE_REST_OF_GEN_DELIMS_CASES(retval)
			PERCENT_ENCODE_PERCENT_CASE(retval)
			PERCENT_ENCODE_OTHER_CHARACTERS_CASES(retval)
			default:
				smart_str_appendc(&retval, ZSTR_VAL(str)[i]);
				break;
		}
	}

	return smart_str_extract(&retval);
}

ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_rfc3986_percent_encode_query_or_fragment(const zend_string *str)
{
	/* query         = *( pchar / "/" / "?" ) */
	/* fragment      = *( pchar / "/" / "?" ) */
	/* pchar         = unreserved / pct-encoded / sub-delims / ":" / "@" */
	smart_str retval = {0};

	for (int i = 0; i < ZSTR_LEN(str); i++) {
		switch (ZSTR_VAL(str)[i]) {
			/* PERCENT_ENCODE_COLON_CASE(retval) */
			/* PERCENT_ENCODE_SLASH_CASE(retval) */
			/* PERCENT_ENCODE_QUESTION_MARK_CASE(retval) */
			/* PERCENT_ENCODE_AT_CASE(retval) */
			PERCENT_ENCODE_REST_OF_GEN_DELIMS_CASES(retval)
			PERCENT_ENCODE_PERCENT_CASE(retval)
			PERCENT_ENCODE_OTHER_CHARACTERS_CASES(retval)
			default:
				smart_str_appendc(&retval, ZSTR_VAL(str)[i]);
				break;
		}
	}

	return smart_str_extract(&retval);
}

ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_rfc3986_percent_encode_form_query(const zend_string *str)
{
	/* query         = *( pchar / "/" / "?" ) */
	/* pchar         = unreserved / pct-encoded / sub-delims / ":" / "@" */
	smart_str retval = {0};

	for (int i = 0; i < ZSTR_LEN(str); i++) {
		switch (ZSTR_VAL(str)[i]) {
			/* PERCENT_ENCODE_COLON_CASE(retval) */
			/* PERCENT_ENCODE_SLASH_CASE(retval) */
			/* PERCENT_ENCODE_QUESTION_MARK_CASE(retval) */
			/* PERCENT_ENCODE_AT_CASE(retval) */
			PERCENT_ENCODE_REST_OF_GEN_DELIMS_CASES(retval)
			PERCENT_ENCODE_PERCENT_AS_PLUS_CASE(retval)
			PERCENT_ENCODE_OTHER_CHARACTERS_CASES(retval)
			default:
				smart_str_appendc(&retval, ZSTR_VAL(str)[i]);
				break;
		}
	}

	return smart_str_extract(&retval);
}

ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_rfc3986_percent_encode_reserved_characters(const zend_string *str)
{
	smart_str retval = {0};

	/* gen-delims    ":" / "/" / "?" / "#" / "[" / "]" / "@"
	 * sub-delims    = "!" / "$" / "&" / "'" / "(" / ")"
	 *			     / "*" / "+" / "," / ";" / "="
	 */

	for (int i = 0; i < ZSTR_LEN(str); i++) {
		switch (ZSTR_VAL(str)[i]) {
			PERCENT_ENCODE_GEN_DELIMS_CASES(retval)
			PERCENT_ENCODE_SUB_DELIMS_CASES(retval)
			default:
			smart_str_appendc(&retval, ZSTR_VAL(str)[i]);
			break;
		}
	}

	return smart_str_extract(&retval);
}

ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_rfc3986_percent_encode_all(const zend_string *str)
{
	smart_str retval = {0};

	/* gen-delims    ":" / "/" / "?" / "#" / "[" / "]" / "@"
	 * sub-delims    = "!" / "$" / "&" / "'" / "(" / ")"
	 *			     / "*" / "+" / "," / ";" / "="
	 */

	for (int i = 0; i < ZSTR_LEN(str); i++) {
		switch (ZSTR_VAL(str)[i]) {
			PERCENT_ENCODE_GEN_DELIMS_CASES(retval)
			PERCENT_ENCODE_SUB_DELIMS_CASES(retval)
			PERCENT_ENCODE_PERCENT_CASE(retval)
			PERCENT_ENCODE_OTHER_CHARACTERS_CASES(retval)
			default:
			smart_str_appendc(&retval, ZSTR_VAL(str)[i]);
			break;
		}
	}

	return smart_str_extract(&retval);
}

ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_rfc3986_percent_decode(const zend_string *str)
{
	smart_str retval = {0};

	smart_str_append(&retval, str);

	return smart_str_extract(&retval);
}

ZEND_ATTRIBUTE_NONNULL zend_string *php_uri_parser_rfc3986_percent_decode_reserved_characters(const zend_string *str)
{
	smart_str retval = {0};

	/* gen-delims    ":" / "/" / "?" / "#" / "[" / "]" / "@"
	 * sub-delims    = "!" / "$" / "&" / "'" / "(" / ")"
	 *			     / "*" / "+" / "," / ";" / "="
	 */

	for (int i = 0; i < ZSTR_LEN(str); i++) {
		switch (ZSTR_VAL(str)[i]) {
			case '%':
				if (i + 2 >= ZSTR_LEN(str)) {
					return NULL;
				}

				switch (ZSTR_VAL(str)[i + 1]) {
					case '2':
						switch (ZSTR_VAL(str)[i + 2]) {
							case 'F':
								smart_str_appendc(&retval, '/');
								i+= 2;
								break;
							case '3':
								smart_str_appendc(&retval, '#');
								i+= 2;
								break;
							case '1':
								smart_str_appendc(&retval, '!');
								i+= 2;
								break;
							case '4':
								smart_str_appendc(&retval, '$');
								i+= 2;
								break;
							case '6':
								smart_str_appendc(&retval, '&');
								i+= 2;
								break;
							case '7':
								smart_str_appendc(&retval, '\'');
								i+= 2;
								break;
							case '8':
								smart_str_appendc(&retval, '(');
								i+= 2;
								break;
							case '9':
								smart_str_appendc(&retval, ')');
								i+= 2;
								break;
							case 'A':
								smart_str_appendc(&retval, '*');
								i+= 2;
								break;
							case 'B':
								smart_str_appendc(&retval, '+');
								i+= 2;
								break;
							case 'C':
								smart_str_appendc(&retval, ',');
								i+= 2;
								break;
							default:
								smart_str_appendc(&retval, ZSTR_VAL(str)[i + 2]);
								i+= 2;
								break;
						}
						break;
					case '3':
						switch (ZSTR_VAL(str)[i + 2]) {
							case 'A':
									smart_str_appendc(&retval, ':');
									i+= 2;
									break;
							case 'F':
									smart_str_appendc(&retval, '?');
									i+= 2;
									break;
							case 'B':
									smart_str_appendc(&retval, ';');
								i+= 2;
									break;
							case 'D':
									smart_str_appendc(&retval, '=');
									i+= 2;
									break;
							default:
									smart_str_appendc(&retval, ZSTR_VAL(str)[i + 2]);
									i+= 2;
									break;
						}
						break;
					case '4':
						switch (ZSTR_VAL(str)[i + 2]) {
							case '0':
									smart_str_appendc(&retval, '@');
									i+= 2;
									break;
							default:
									smart_str_appendc(&retval, ZSTR_VAL(str)[i + 2]);
									i+= 2;
									break;
						}
						break;
					case '5':
						switch (ZSTR_VAL(str)[i + 2]) {
							case 'B':
									smart_str_appendc(&retval, '[');
									i+= 2;
									break;
							case 'D':
									smart_str_appendc(&retval, ']');
									i+= 2;
									break;
							default:
									smart_str_appendc(&retval, ZSTR_VAL(str)[i + 2]);
									i+= 2;
									break;
						}
						break;
					default:
						i+= 1;
						smart_str_appendc(&retval, ZSTR_VAL(str)[i + 1]);
						break;
				}
				break;
			default:
				smart_str_appendc(&retval, ZSTR_VAL(str)[i]);
				break;
		}
	}

	return smart_str_extract(&retval);
}

static inline size_t get_text_range_length(const UriTextRangeA *range)
{
	return range->afterLast - range->first;
}

static inline bool has_text_range(const UriTextRangeA *range)
{
	return range->first != NULL && range->afterLast != NULL;
}

ZEND_ATTRIBUTE_NONNULL static void copy_uri(UriUriA *new_uriparser_uri, const UriUriA *uriparser_uri)
{
	int result = uriCopyUriMmA(new_uriparser_uri, uriparser_uri, mm);
	ZEND_ASSERT(result == URI_SUCCESS);
}

ZEND_ATTRIBUTE_NONNULL static UriUriA *get_normalized_uri(php_uri_parser_rfc3986_uris *uriparser_uris) {
	if (!uriparser_uris->normalized_uri_initialized) {
		copy_uri(&uriparser_uris->normalized_uri, &uriparser_uris->uri);
		int result = uriNormalizeSyntaxExMmA(&uriparser_uris->normalized_uri, (unsigned int)-1, mm);
		ZEND_ASSERT(result == URI_SUCCESS);
		uriparser_uris->normalized_uri_initialized = true;
	}

	return &uriparser_uris->normalized_uri;
}

ZEND_ATTRIBUTE_NONNULL static UriUriA *get_uri_for_reading(php_uri_parser_rfc3986_uris *uriparser_uris, php_uri_component_read_mode read_mode)
{
	switch (read_mode) {
		case PHP_URI_COMPONENT_READ_MODE_RAW:
			return &uriparser_uris->uri;
		case PHP_URI_COMPONENT_READ_MODE_NORMALIZED_ASCII:
			ZEND_FALLTHROUGH;
		case PHP_URI_COMPONENT_READ_MODE_NORMALIZED_UNICODE:
			return get_normalized_uri(uriparser_uris);
		EMPTY_SWITCH_DEFAULT_CASE()
	}
}

ZEND_ATTRIBUTE_NONNULL static UriUriA *get_uri_for_writing(php_uri_parser_rfc3986_uris *uriparser_uris)
{
	return &uriparser_uris->uri;
}


ZEND_ATTRIBUTE_NONNULL void php_uri_parser_rfc3986_uri_type_read(void *uri, zval *retval)
{
	const UriUriA *uriparser_uri = get_uri_for_reading(uri, PHP_URI_COMPONENT_READ_MODE_RAW);

	if (has_text_range(&uriparser_uri->scheme)) {
		ZVAL_OBJ_COPY(retval, zend_enum_get_case_cstr(php_uri_ce_rfc3986_uri_type, "Uri"));
		return;
	}

	if (has_text_range(&uriparser_uri->hostText)) {
		ZVAL_OBJ_COPY(retval, zend_enum_get_case_cstr(php_uri_ce_rfc3986_uri_type, "NetworkPathReference"));
		return;
	}

	if (uriparser_uri->absolutePath) {
		ZVAL_OBJ_COPY(retval, zend_enum_get_case_cstr(php_uri_ce_rfc3986_uri_type, "AbsolutePathReference"));
	} else {
		ZVAL_OBJ_COPY(retval, zend_enum_get_case_cstr(php_uri_ce_rfc3986_uri_type, "RelativePathReference"));
	}
}


ZEND_ATTRIBUTE_NONNULL static zend_result php_uri_parser_rfc3986_scheme_read(void *uri, php_uri_component_read_mode read_mode, zval *retval)
{
	const UriUriA *uriparser_uri = get_uri_for_reading(uri, read_mode);

	if (has_text_range(&uriparser_uri->scheme)) {
		ZVAL_STRINGL(retval, uriparser_uri->scheme.first, get_text_range_length(&uriparser_uri->scheme));
	} else {
		ZVAL_NULL(retval);
	}

	return SUCCESS;
}

static zend_result php_uri_parser_rfc3986_scheme_write(void *uri, zval *value, zval *errors)
{
	UriUriA *uriparser_uri = get_uri_for_writing(uri);
	int result;

	if (Z_TYPE_P(value) == IS_NULL) {
		result = uriSetSchemeMmA(uriparser_uri, NULL, NULL, mm);
	} else {
		result = uriSetSchemeMmA(uriparser_uri, Z_STRVAL_P(value), Z_STRVAL_P(value) + Z_STRLEN_P(value), mm);
	}

	switch (result) {
		case URI_SUCCESS:
			return SUCCESS;
		case URI_ERROR_SYNTAX:
			zend_throw_exception(php_uri_ce_invalid_uri_exception, "The specified scheme is malformed", 0);
			return FAILURE;
		default:
			/* This should be unreachable in practice. */
			zend_throw_exception(php_uri_ce_error, "Failed to update the scheme", 0);
			return FAILURE;
	}
}

ZEND_ATTRIBUTE_NONNULL zend_result php_uri_parser_rfc3986_userinfo_read(php_uri_parser_rfc3986_uris *uri, php_uri_component_read_mode read_mode, zval *retval)
{
	const UriUriA *uriparser_uri = get_uri_for_reading(uri, read_mode);

	if (has_text_range(&uriparser_uri->userInfo)) {
		ZVAL_STRINGL(retval, uriparser_uri->userInfo.first, get_text_range_length(&uriparser_uri->userInfo));
	} else {
		ZVAL_NULL(retval);
	}

	return SUCCESS;
}

zend_result php_uri_parser_rfc3986_userinfo_write(php_uri_parser_rfc3986_uris *uri, zval *value, zval *errors)
{
	UriUriA *uriparser_uri = get_uri_for_writing(uri);
	int result;

	if (Z_TYPE_P(value) == IS_NULL) {
		result = uriSetUserInfoMmA(uriparser_uri, NULL, NULL, mm);
	} else {
		result = uriSetUserInfoMmA(uriparser_uri, Z_STRVAL_P(value), Z_STRVAL_P(value) + Z_STRLEN_P(value), mm);
	}

	switch (result) {
		case URI_SUCCESS:
			return SUCCESS;
		case URI_ERROR_SETUSERINFO_HOST_NOT_SET:
			zend_throw_exception(php_uri_ce_invalid_uri_exception, "Cannot set a userinfo without having a host", 0);
			return FAILURE;
		case URI_ERROR_SYNTAX:
			zend_throw_exception(php_uri_ce_invalid_uri_exception, "The specified userinfo is malformed", 0);
			return FAILURE;
		default:
			/* This should be unreachable in practice. */
			zend_throw_exception(php_uri_ce_error, "Failed to update the userinfo", 0);
			return FAILURE;
	}
}

ZEND_ATTRIBUTE_NONNULL static zend_result php_uri_parser_rfc3986_username_read(void *uri, php_uri_component_read_mode read_mode, zval *retval)
{
	const UriUriA *uriparser_uri = get_uri_for_reading(uri, read_mode);

	if (has_text_range(&uriparser_uri->userInfo)) {
		size_t length = get_text_range_length(&uriparser_uri->userInfo);
		const char *c = memchr(uriparser_uri->userInfo.first, ':', length);

		if (c == NULL && length > 0) {
			ZVAL_STRINGL(retval, uriparser_uri->userInfo.first, length);
		} else if (c != NULL && c - uriparser_uri->userInfo.first > 0) {
			ZVAL_STRINGL(retval, uriparser_uri->userInfo.first, c - uriparser_uri->userInfo.first);
		} else {
			ZVAL_EMPTY_STRING(retval);
		}
	} else {
		ZVAL_NULL(retval);
	}

	return SUCCESS;
}

ZEND_ATTRIBUTE_NONNULL static zend_result php_uri_parser_rfc3986_password_read(void *uri, php_uri_component_read_mode read_mode, zval *retval)
{
	const UriUriA *uriparser_uri = get_uri_for_reading(uri, read_mode);

	if (has_text_range(&uriparser_uri->userInfo)) {
		const char *c = memchr(uriparser_uri->userInfo.first, ':', get_text_range_length(&uriparser_uri->userInfo));

		if (c != NULL && uriparser_uri->userInfo.afterLast - c - 1 > 0) {
			ZVAL_STRINGL(retval, c + 1, uriparser_uri->userInfo.afterLast - c - 1);
		} else {
			ZVAL_EMPTY_STRING(retval);
		}
	} else {
		ZVAL_NULL(retval);
	}

	return SUCCESS;
}

ZEND_ATTRIBUTE_NONNULL static zend_result php_uri_parser_rfc3986_host_read(void *uri, php_uri_component_read_mode read_mode, zval *retval)
{
	const UriUriA *uriparser_uri = get_uri_for_reading(uri, read_mode);

	if (has_text_range(&uriparser_uri->hostText)) {
		if (uriparser_uri->hostData.ip6 != NULL || uriparser_uri->hostData.ipFuture.first != NULL) {
			/* the textual representation of the host is always accessible in the .hostText field no matter what the host is */
			smart_str host_str = {0};

			smart_str_appendc(&host_str, '[');
			smart_str_appendl(&host_str, uriparser_uri->hostText.first, get_text_range_length(&uriparser_uri->hostText));
			smart_str_appendc(&host_str, ']');

			ZVAL_NEW_STR(retval, smart_str_extract(&host_str));
		} else {
			ZVAL_STRINGL(retval, uriparser_uri->hostText.first, get_text_range_length(&uriparser_uri->hostText));
		}
	} else {
		ZVAL_NULL(retval);
	}

	return SUCCESS;
}

ZEND_ATTRIBUTE_NONNULL void php_uri_parser_rfc3986_host_type_read(void *uri, php_uri_component_read_mode read_mode, zval *retval)
{
	const UriUriA *uriparser_uri = get_uri_for_reading(uri, read_mode);

	if (!has_text_range(&uriparser_uri->hostText)) {
		ZVAL_NULL(retval);
		return;
	}

	if (uriparser_uri->hostData.ip4 != NULL) {
		ZVAL_OBJ_COPY(retval, zend_enum_get_case_cstr(php_uri_ce_rfc3986_uri_host_type, "IPv4"));
	} else if (uriparser_uri->hostData.ip6 != NULL) {
		ZVAL_OBJ_COPY(retval, zend_enum_get_case_cstr(php_uri_ce_rfc3986_uri_host_type, "IPv6"));
	} else if (has_text_range(&uriparser_uri->hostData.ipFuture)) {
		ZVAL_OBJ_COPY(retval, zend_enum_get_case_cstr(php_uri_ce_rfc3986_uri_host_type, "IPvFuture"));
	} else {
		ZVAL_OBJ_COPY(retval, zend_enum_get_case_cstr(php_uri_ce_rfc3986_uri_host_type, "RegisteredName"));
	}
}

static zend_result php_uri_parser_rfc3986_host_write(void *uri, zval *value, zval *errors)
{
	UriUriA *uriparser_uri = get_uri_for_writing(uri);
	int result;

	if (Z_TYPE_P(value) == IS_NULL) {
		result = uriSetHostAutoMmA(uriparser_uri, NULL, NULL, mm);
	} else {
		result = uriSetHostAutoMmA(uriparser_uri, Z_STRVAL_P(value), Z_STRVAL_P(value) + Z_STRLEN_P(value), mm);
	}

	switch (result) {
		case URI_SUCCESS:
			return SUCCESS;
		case URI_ERROR_SETHOST_PORT_SET:
			zend_throw_exception(php_uri_ce_invalid_uri_exception, "Cannot remove the host from a URI that has a port", 0);
			return FAILURE;
		case URI_ERROR_SETHOST_USERINFO_SET:
			zend_throw_exception(php_uri_ce_invalid_uri_exception, "Cannot remove the host from a URI that has a userinfo", 0);
			return FAILURE;
		case URI_ERROR_SYNTAX:
			zend_throw_exception(php_uri_ce_invalid_uri_exception, "The specified host is malformed", 0);
			return FAILURE;
		default:
			/* This should be unreachable in practice. */
			zend_throw_exception(php_uri_ce_error, "Failed to update the host", 0);
			return FAILURE;
	}
}

ZEND_ATTRIBUTE_NONNULL static zend_long port_str_to_zend_long_checked(const char *str, size_t len)
{
	if (len > MAX_LENGTH_OF_LONG) {
		return -1;
	}

	char buf[MAX_LENGTH_OF_LONG + 1];
	*(char*)zend_mempcpy(buf, str, len) = 0;

	zend_ulong result = ZEND_STRTOUL(buf, NULL, 10);

	if (result > ZEND_LONG_MAX) {
		return -1;
	}

	return (zend_long)result;
}

ZEND_ATTRIBUTE_NONNULL static zend_result php_uri_parser_rfc3986_port_read(void *uri, php_uri_component_read_mode read_mode, zval *retval)
{
	const UriUriA *uriparser_uri = get_uri_for_reading(uri, read_mode);

	if (has_text_range(&uriparser_uri->portText) && get_text_range_length(&uriparser_uri->portText) > 0) {
		ZVAL_LONG(retval, port_str_to_zend_long_checked(uriparser_uri->portText.first, get_text_range_length(&uriparser_uri->portText)));
	} else {
		ZVAL_NULL(retval);
	}

	return SUCCESS;
}

static zend_result php_uri_parser_rfc3986_port_write(void *uri, zval *value, zval *errors)
{
	UriUriA *uriparser_uri = get_uri_for_writing(uri);
	int result;

	if (Z_TYPE_P(value) == IS_NULL) {
		result = uriSetPortTextMmA(uriparser_uri, NULL, NULL, mm);
	} else {
		zend_string *tmp = zend_long_to_str(Z_LVAL_P(value));
		result = uriSetPortTextMmA(uriparser_uri, ZSTR_VAL(tmp), ZSTR_VAL(tmp) + ZSTR_LEN(tmp), mm);
		zend_string_release_ex(tmp, false);
	}

	switch (result) {
		case URI_SUCCESS:
			return SUCCESS;
		case URI_ERROR_SETPORT_HOST_NOT_SET:
			zend_throw_exception(php_uri_ce_invalid_uri_exception, "Cannot set a port without having a host", 0);
			return FAILURE;
		case URI_ERROR_SYNTAX:
			zend_throw_exception(php_uri_ce_invalid_uri_exception, "The specified port is malformed", 0);
			return FAILURE;
		default:
			/* This should be unreachable in practice. */
			zend_throw_exception(php_uri_ce_error, "Failed to update the port", 0);
			return FAILURE;
	}
}

ZEND_ATTRIBUTE_NONNULL static zend_result php_uri_parser_rfc3986_path_read(void *uri, php_uri_component_read_mode read_mode, zval *retval)
{
	const UriUriA *uriparser_uri = get_uri_for_reading(uri, read_mode);

	if (uriparser_uri->pathHead != NULL) {
		smart_str str = {0};

		if (uriparser_uri->absolutePath || uriHasHostA(uriparser_uri)) {
			smart_str_appendc(&str, '/');
		}

		for (const UriPathSegmentA *p = uriparser_uri->pathHead; p; p = p->next) {
			smart_str_appendl(&str, p->text.first, get_text_range_length(&p->text));
			if (p->next) {
				smart_str_appendc(&str, '/');
			}
		}

		ZVAL_NEW_STR(retval, smart_str_extract(&str));
	} else if (uriparser_uri->absolutePath) {
		ZVAL_CHAR(retval, '/');
	} else {
		ZVAL_EMPTY_STRING(retval);
	}

	return SUCCESS;
}

zend_result php_uri_parser_rfc3986_path_segments_read(void *uri, php_uri_component_read_mode read_mode, zval *retval)
{
	const UriUriA *uriparser_uri = get_uri_for_reading(uri, read_mode);

	if (uriparser_uri->pathHead != NULL) {
		array_init(retval);

		for (const UriPathSegmentA *p = uriparser_uri->pathHead; p; p = p->next) {
			zval tmp;
			ZVAL_STRINGL(&tmp,p->text.first, get_text_range_length(&p->text));
			zend_hash_next_index_insert_new(Z_ARRVAL_P(retval), &tmp);
		}
	} else if (uriparser_uri->absolutePath) {
		array_init(retval);
		zval tmp;
		ZVAL_EMPTY_STRING(&tmp);

		zend_hash_next_index_insert_new(Z_ARRVAL_P(retval), &tmp);
		zval_ptr_dtor(&tmp);
	} else {
		ZVAL_EMPTY_ARRAY(retval);
	}

	return SUCCESS;
}

static zend_result php_uri_parser_rfc3986_path_write(void *uri, zval *value, zval *errors)
{
	UriUriA *uriparser_uri = get_uri_for_writing(uri);
	int result;

	if (Z_STRLEN_P(value) == 0) {
		result = uriSetPathMmA(uriparser_uri, NULL, NULL, mm);
	} else {
		result = uriSetPathMmA(uriparser_uri, Z_STRVAL_P(value), Z_STRVAL_P(value) + Z_STRLEN_P(value), mm);
	}

	switch (result) {
		case URI_SUCCESS:
			return SUCCESS;
		case URI_ERROR_SYNTAX:
			zend_throw_exception(php_uri_ce_invalid_uri_exception, "The specified path is malformed", 0);
			return FAILURE;
		default:
			/* This should be unreachable in practice. */
			zend_throw_exception(php_uri_ce_error, "Failed to update the path", 0);
			return FAILURE;
	}
}

zend_result php_uri_parser_rfc3986_path_segments_write(php_uri_parser_rfc3986_uris *uri, zval *value, bool include_leading_slash_for_non_empty_relative_uri)
{
	UriUriA *uriparser_uri = get_uri_for_writing(uri);
	HashTable *segments = Z_ARR_P(value);
	int result;

	if (zend_array_count(segments) == 0) {
		result = uriSetPathMmA(uriparser_uri, NULL, NULL, mm);
	} else {
		smart_str path = {0};
		bool is_path_reference = !has_text_range(&uriparser_uri->scheme);
		bool is_first_segment = true;

		zval *tmp;
		zend_ulong index_num;
		zend_string *index_str = NULL;
		ZEND_HASH_FOREACH_KEY_VAL(segments, index_num, index_str, tmp) {
			if (index_str != NULL || index_num > 99999) { // TODO change
				zend_argument_type_error(1, "must only have numeric keys, string given");
				return FAILURE;
			}

			if (Z_TYPE_P(tmp) != IS_STRING) {
				zend_argument_type_error(1, "must be a list of strings, %s given", zend_zval_value_name(tmp));
				return FAILURE;
			}

			if (is_path_reference && is_first_segment && Z_STRLEN_P(tmp) > 0) {
				if (include_leading_slash_for_non_empty_relative_uri) {
					smart_str_appendc(&path, '/');
				}
			} else {
				smart_str_appendc(&path, '/');
			}

			smart_str_append(&path, Z_STR_P(tmp));

			is_first_segment = false;
		} ZEND_HASH_FOREACH_END();

		zend_string *str = smart_str_extract(&path);
		result = uriSetPathMmA(uriparser_uri, ZSTR_VAL(str), ZSTR_VAL(str) + ZSTR_LEN(str), mm);
		zend_string_release(str);
	}

	switch (result) {
		case URI_SUCCESS:
			return SUCCESS;
		default:
			/* This should be unreachable in practice. */
			zend_throw_exception(php_uri_ce_error, "Failed to update the path", 0);
			return FAILURE;
	}
}

ZEND_ATTRIBUTE_NONNULL static zend_result php_uri_parser_rfc3986_query_read(void *uri, php_uri_component_read_mode read_mode, zval *retval)
{
	const UriUriA *uriparser_uri = get_uri_for_reading(uri, read_mode);

	if (has_text_range(&uriparser_uri->query)) {
		ZVAL_STRINGL(retval, uriparser_uri->query.first, get_text_range_length(&uriparser_uri->query));
	} else {
		ZVAL_NULL(retval);
	}

	return SUCCESS;
}

ZEND_ATTRIBUTE_NONNULL zend_result php_uri_parser_rfc3986_query_params_read(void *uri, php_uri_component_read_mode read_mode, zval *retval)
{
	const UriUriA *uriparser_uri = get_uri_for_reading(uri, read_mode);

	if (has_text_range(&uriparser_uri->query)) {
		object_init_ex(retval, php_uri_ce_rfc3986_uri_query_params);
		php_uri_parser_rfc3986_uri_query_params_object *uri_query_params_object = Z_URI_QUERY_PARAMS_OBJECT_P(retval);

		if (php_uri_rfc3986_uri_query_params_from_str(uriparser_uri->query.first, get_text_range_length(&uriparser_uri->query),
			&uri_query_params_object->query_list, false
		) == FAILURE) {
			php_uri_rfc3986_uri_query_params_free(Z_URI_QUERY_PARAMS_OBJECT_P(retval));
			return FAILURE;
		}

		uri_query_params_object->is_initialized = true;
	} else {
		ZVAL_NULL(retval);
	}

	return SUCCESS;
}

static zend_result php_uri_parser_rfc3986_query_write(void *uri, zval *value, zval *errors)
{
	UriUriA *uriparser_uri = get_uri_for_writing(uri);
	int result;

	if (Z_TYPE_P(value) == IS_NULL) {
		result = uriSetQueryMmA(uriparser_uri, NULL, NULL, mm);
	} else {
		result = uriSetQueryMmA(uriparser_uri, Z_STRVAL_P(value), Z_STRVAL_P(value) + Z_STRLEN_P(value), mm);
	}

	switch (result) {
		case URI_SUCCESS:
			return SUCCESS;
		case URI_ERROR_SYNTAX:
			zend_throw_exception(php_uri_ce_invalid_uri_exception, "The specified query is malformed", 0);
			return FAILURE;
		default:
			/* This should be unreachable in practice. */
			zend_throw_exception(php_uri_ce_error, "Failed to update the query", 0);
			return FAILURE;
	}
}

ZEND_ATTRIBUTE_NONNULL zend_result php_uri_parser_rfc3986_query_params_write(void *uri, zval *value)
{
	UriUriA *uriparser_uri = get_uri_for_writing(uri);
	int result;

	if (Z_TYPE_P(value) == IS_NULL) {
		result = uriSetQueryMmA(uriparser_uri, NULL, NULL, mm);
	} else {
		ZEND_ASSERT(Z_TYPE_P(value) == IS_OBJECT && instanceof_function(Z_OBJCE_P(value), php_uri_ce_rfc3986_uri_query_params));
		php_uri_parser_rfc3986_uri_query_params_object *uri_query_params_object = Z_URI_QUERY_PARAMS_OBJECT_P(value);
		zend_string *query_string = php_uri_rfc3986_uri_query_params_to_string(uri_query_params_object->query_list);

		result = uriSetQueryMmA(uriparser_uri, ZSTR_VAL(query_string), ZSTR_VAL(query_string) + ZSTR_LEN(query_string), mm);

		zend_string_release(query_string);
	}

	switch (result) {
		case URI_SUCCESS:
			return SUCCESS;
		case URI_ERROR_SYNTAX:
			zend_throw_exception(php_uri_ce_invalid_uri_exception, "The specified query is malformed", 0);
			return FAILURE;
		default:
			/* This should be unreachable in practice. */
			zend_throw_exception(php_uri_ce_error, "Failed to update the query", 0);
			return FAILURE;
	}
}

ZEND_ATTRIBUTE_NONNULL static zend_result php_uri_parser_rfc3986_fragment_read(void *uri, php_uri_component_read_mode read_mode, zval *retval)
{
	const UriUriA *uriparser_uri = get_uri_for_reading(uri, read_mode);

	if (has_text_range(&uriparser_uri->fragment)) {
		ZVAL_STRINGL(retval, uriparser_uri->fragment.first, get_text_range_length(&uriparser_uri->fragment));
	} else {
		ZVAL_NULL(retval);
	}

	return SUCCESS;
}

static zend_result php_uri_parser_rfc3986_fragment_write(void *uri, zval *value, zval *errors)
{
	UriUriA *uriparser_uri = get_uri_for_writing(uri);
	int result;

	if (Z_TYPE_P(value) == IS_NULL) {
		result = uriSetFragmentMmA(uriparser_uri, NULL, NULL, mm);
	} else {
		result = uriSetFragmentMmA(uriparser_uri, Z_STRVAL_P(value), Z_STRVAL_P(value) + Z_STRLEN_P(value), mm);
	}

	switch (result) {
		case URI_SUCCESS:
			return SUCCESS;
		case URI_ERROR_SYNTAX:
			zend_throw_exception(php_uri_ce_invalid_uri_exception, "The specified fragment is malformed", 0);
			return FAILURE;
		default:
			/* This should be unreachable in practice. */
			zend_throw_exception(php_uri_ce_error, "Failed to update the fragment", 0);
			return FAILURE;
	}
}

static php_uri_parser_rfc3986_uris *uriparser_create_uris(void)
{
	php_uri_parser_rfc3986_uris *uriparser_uris = ecalloc(1, sizeof(*uriparser_uris));
	uriparser_uris->normalized_uri_initialized = false;

	return uriparser_uris;
}

php_uri_parser_rfc3986_uris *php_uri_parser_rfc3986_parse_ex(const char *uri_str, size_t uri_str_len, const php_uri_parser_rfc3986_uris *uriparser_base_urls, bool silent)
{
	UriUriA uri = {0};

	/* Parse the URI. */
	int result = uriParseSingleUriExMmA(&uri, uri_str, uri_str + uri_str_len, NULL, mm);
	if (result != URI_SUCCESS) {
		if (!silent) {
			switch (result) {
				case URI_ERROR_SYNTAX:
					zend_throw_exception(php_uri_ce_invalid_uri_exception, "The specified URI is malformed", 0);
					break;
				default:
					/* This should be unreachable in practice. */
					zend_throw_exception(php_uri_ce_error, "Failed to parse the specified URI", 0);
					break;
			}
		}

		goto fail;
	}

	if (uriparser_base_urls != NULL) {
		UriUriA tmp = {0};

		/* Combine the parsed URI with the base URI and store the result in 'tmp',
		 * since the target and source URLs must be distinct. */
		int result = uriAddBaseUriExMmA(&tmp, &uri, &uriparser_base_urls->uri, URI_RESOLVE_STRICTLY, mm);
		if (result != URI_SUCCESS) {
			if (!silent) {
				switch (result) {
					case URI_ERROR_ADDBASE_REL_BASE:
						zend_throw_exception(php_uri_ce_invalid_uri_exception, "The specified base URI must be absolute", 0);
						break;
					default:
						/* This should be unreachable in practice. */
						zend_throw_exception(php_uri_ce_error, "Failed to resolve the specified URI against the base URI", 0);
						break;
				}
			}

			goto fail;
		}

		/* Store the combined URI back into 'uri'. */
		uriFreeUriMembersMmA(&uri, mm);
		uri = tmp;
	}

	/* Make the resulting URI independent of the 'uri_str'. */
	uriMakeOwnerMmA(&uri, mm);

	if (has_text_range(&uri.portText) && get_text_range_length(&uri.portText) > 0) {
		if (port_str_to_zend_long_checked(uri.portText.first, get_text_range_length(&uri.portText)) == -1) {
			if (!silent) {
				zend_throw_exception(php_uri_ce_invalid_uri_exception, "The port is out of range", 0);
			}

			goto fail;
		}
	}

	php_uri_parser_rfc3986_uris *uriparser_uris = uriparser_create_uris();
	uriparser_uris->uri = uri;

	return uriparser_uris;

 fail:

	uriFreeUriMembersMmA(&uri, mm);

	return NULL;
}

void *php_uri_parser_rfc3986_parse(const char *uri_str, size_t uri_str_len, const void *base_url, zval *errors, bool silent)
{
	return php_uri_parser_rfc3986_parse_ex(uri_str, uri_str_len, base_url, silent);
}

ZEND_ATTRIBUTE_NONNULL static void *php_uri_parser_rfc3986_clone(void *uri)
{
	const php_uri_parser_rfc3986_uris *uriparser_uris = uri;

	php_uri_parser_rfc3986_uris *new_uriparser_uris = uriparser_create_uris();
	copy_uri(&new_uriparser_uris->uri, &uriparser_uris->uri);
	/* Do not copy the normalized URI: The expected action after cloning is
	 * modifying the cloned URI (which will invalidate the cached normalized
	 * URI). */

	return new_uriparser_uris;
}

ZEND_ATTRIBUTE_NONNULL static zend_string *php_uri_parser_rfc3986_to_string(void *uri, php_uri_recomposition_mode recomposition_mode, bool exclude_fragment)
{
	php_uri_parser_rfc3986_uris *uriparser_uris = uri;
	const UriUriA *uriparser_uri;

	if (recomposition_mode == PHP_URI_RECOMPOSITION_MODE_RAW_ASCII || recomposition_mode == PHP_URI_RECOMPOSITION_MODE_RAW_UNICODE) {
		uriparser_uri = &uriparser_uris->uri;
	} else {
		uriparser_uri = get_normalized_uri(uriparser_uris);
	}

	int charsRequired = 0;
	int result = uriToStringCharsRequiredA(uriparser_uri, &charsRequired);
	ZEND_ASSERT(result == URI_SUCCESS);

	charsRequired++;

	zend_string *uri_string = zend_string_alloc(charsRequired - 1, false);
	result = uriToStringA(ZSTR_VAL(uri_string), uriparser_uri, charsRequired, NULL);
	ZEND_ASSERT(result == URI_SUCCESS);

	if (exclude_fragment) {
		const char *pos = zend_memrchr(ZSTR_VAL(uri_string), '#', ZSTR_LEN(uri_string));
		if (pos != NULL) {
			uri_string = zend_string_truncate(uri_string, (pos - ZSTR_VAL(uri_string)), false);
		}
	}

	return uri_string;
}

static void php_uri_parser_rfc3986_destroy(void *uri)
{
	php_uri_parser_rfc3986_uris *uriparser_uris = uri;

	if (UNEXPECTED(uriparser_uris == NULL)) {
		return;
	}

	uriFreeUriMembersMmA(&uriparser_uris->uri, mm);
	uriFreeUriMembersMmA(&uriparser_uris->normalized_uri, mm);

	efree(uriparser_uris);
}

ZEND_ATTRIBUTE_NONNULL PHPAPI void php_uri_rfc3986_uri_query_params_free(php_uri_parser_rfc3986_uri_query_params_object *uri_query_params_object)
{
	if (uri_query_params_object->query_list != NULL) {
		uriFreeQueryListMmA(uri_query_params_object->query_list, mm);
		uri_query_params_object->query_list = NULL;
		uri_query_params_object->is_initialized = false;
	}
}

ZEND_ATTRIBUTE_NONNULL PHPAPI void php_uri_rfc3986_uri_query_params_object_handler_free(zend_object *object)
{
	php_uri_parser_rfc3986_uri_query_params_object *uri_query_params_object = php_uri_rfc3986_uri_query_params_object_from_obj(object);

	php_uri_rfc3986_uri_query_params_free(uri_query_params_object);

	zend_object_std_dtor(&uri_query_params_object->std);
}

ZEND_ATTRIBUTE_NONNULL PHPAPI zend_object *php_uri_rfc3986_uri_query_params_object_handler_clone(zend_object *object)
{
	php_uri_parser_rfc3986_uri_query_params_object *uri_query_params_object = php_uri_rfc3986_uri_query_params_object_from_obj(object);

	php_uri_parser_rfc3986_uri_query_params_object *new_uri_query_params_object = php_uri_rfc3986_uri_query_params_object_from_obj(
		object->ce->create_object(object->ce)
	);

	new_uri_query_params_object->query_list = uri_query_params_object->query_list; // TODO Fix

	zend_objects_clone_members(&new_uri_query_params_object->std, &uri_query_params_object->std);

	return &new_uri_query_params_object->std;
}

ZEND_ATTRIBUTE_NONNULL zend_result php_uri_rfc3986_uri_query_params_from_str(const char *query_str_first,
	size_t query_str_length, UriQueryListA **query_list, bool silent
) {
	int item_count;

	if (uriDissectQueryMallocExMmA(
		query_list, &item_count, query_str_first, query_str_first + query_str_length,
		URI_TRUE, URI_BR_DONT_TOUCH, mm
	) != URI_SUCCESS) {
		if (!silent) {
			zend_throw_exception(php_uri_ce_exception, "Failed to parse the specified query string", 0);
		}

		return FAILURE;
	}

	return SUCCESS;
}

static UriQueryListA *php_uri_rfc3986_uri_query_params_search_last_item(UriQueryListA *query_list)
{
	if (query_list == NULL) {
		return NULL;
	}

	UriQueryListA *p = query_list;

	while (p->next != NULL) {
		p = p->next;
	}

	return p;
}

ZEND_ATTRIBUTE_NONNULL_ARGS(2) UriQueryListA *php_uri_rfc3986_uri_query_params_search_first(UriQueryListA *query_list,
	const char *name, size_t name_len, const char *value, size_t value_len
) {
	for (UriQueryListA *p = query_list; p != NULL; p = p->next) {
		if (zend_binary_strcmp(name, name_len, p->key, strlen(p->key)) != 0) {
			continue;
		}

		if (EXPECTED(value == NULL)) {
			return p;
		}

		if (zend_binary_strcmp(value, value_len, p->value, strlen(p->value))) { // TODO check null values
			return p;
		}
	}

	return NULL;
}

ZEND_ATTRIBUTE_NONNULL_ARGS(1,2) zend_result php_uri_rfc3986_uri_query_params_append(UriQueryListA **query_list,
	const char *name, size_t name_len, const char *value, size_t value_len
) {
	int item_count;

	UriQueryListA **next_item;
	UriQueryListA *last_item = php_uri_rfc3986_uri_query_params_search_last_item(*query_list);
	if (UNEXPECTED(last_item == NULL)) {
		UriQueryListA *tmp = emalloc(sizeof(*tmp));
		tmp->key = NULL;
		tmp->value = NULL;
		tmp->next = NULL;

		*query_list = tmp;
		next_item = query_list;
	} else {
		next_item = &last_item->next;
	}

	bool result = uriAppendQueryItemA(next_item, &item_count,
		name, name + name_len, value, value ? value + value_len : 0,
		URI_TRUE, URI_BR_DONT_TOUCH, mm
	);

	return result == URI_TRUE ? SUCCESS : FAILURE;
}

zend_string *php_uri_rfc3986_uri_query_params_to_string(const UriQueryListA *query_list)
{
	if (query_list == NULL) {
		return zend_empty_string;
	}

	int chars_required = 0;
	if (uriComposeQueryCharsRequiredExA(query_list, &chars_required, true, URI_BR_DONT_TOUCH) != URI_SUCCESS) {
		return NULL;
	}

	zend_string *query_string = zend_string_alloc(chars_required, false);
	int chars_written;
	if (uriComposeQueryExA(ZSTR_VAL(query_string), query_list, chars_required + 1, &chars_written, URI_TRUE, URI_FALSE) != URI_SUCCESS) {
		zend_string_efree(query_string);
		return NULL;
	}

	if (chars_written == chars_required) {
		return query_string;
	}

	return zend_string_truncate(query_string, chars_written - 1, false);
}

ZEND_ATTRIBUTE_NONNULL HashTable *php_uri_rfc3986_uri_query_params_get_debug_properties(php_uri_parser_rfc3986_uri_query_params_object *object)
{
	const HashTable *std_properties = zend_std_get_properties(&object->std);
	HashTable *result = zend_array_dup(std_properties);

	for (UriQueryListA *param = object->query_list; param != NULL; param = param->next) {
		zval tmp;

		if (param->value == NULL) {
			ZVAL_NULL(&tmp);
		} else {
			ZVAL_STRING(&tmp, param->value);
		}

		zend_hash_str_add_new(result, param->key, strlen(param->key), &tmp);
	}

	return result;
}

PHPAPI const php_uri_parser php_uri_parser_rfc3986 = {
	.name = PHP_URI_PARSER_RFC3986,
	.parse = php_uri_parser_rfc3986_parse,
	.clone = php_uri_parser_rfc3986_clone,
	.to_string = php_uri_parser_rfc3986_to_string,
	.destroy = php_uri_parser_rfc3986_destroy,
	{
		.scheme = {.read = php_uri_parser_rfc3986_scheme_read, .write = php_uri_parser_rfc3986_scheme_write},
		.username = {.read = php_uri_parser_rfc3986_username_read, .write = NULL},
		.password = {.read = php_uri_parser_rfc3986_password_read, .write = NULL},
		.host = {.read = php_uri_parser_rfc3986_host_read, .write = php_uri_parser_rfc3986_host_write},
		.port = {.read = php_uri_parser_rfc3986_port_read, .write = php_uri_parser_rfc3986_port_write},
		.path = {.read = php_uri_parser_rfc3986_path_read, .write = php_uri_parser_rfc3986_path_write},
		.query = {.read = php_uri_parser_rfc3986_query_read, .write = php_uri_parser_rfc3986_query_write},
		.fragment = {.read = php_uri_parser_rfc3986_fragment_read, .write = php_uri_parser_rfc3986_fragment_write},
	}
};
