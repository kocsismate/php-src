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

#ifndef PHP_URI_QUERY_H
#define PHP_URI_QUERY_H

extern PHP_METHOD(Uri_Rfc3986_UriQueryParams, fromQueryString);
extern PHP_METHOD(Uri_Rfc3986_UriQueryParams, fromArray);
extern PHP_METHOD(Uri_Rfc3986_UriQueryParams, append);
extern PHP_METHOD(Uri_Rfc3986_UriQueryParams, delete);
extern PHP_METHOD(Uri_Rfc3986_UriQueryParams, has);
extern PHP_METHOD(Uri_Rfc3986_UriQueryParams, getFirst);
extern PHP_METHOD(Uri_Rfc3986_UriQueryParams, getLast);
extern PHP_METHOD(Uri_Rfc3986_UriQueryParams, getAll);
extern PHP_METHOD(Uri_Rfc3986_UriQueryParams, set);
extern PHP_METHOD(Uri_Rfc3986_UriQueryParams, sort);
extern PHP_METHOD(Uri_Rfc3986_UriQueryParams, toString);
extern PHP_METHOD(Uri_Rfc3986_UriQueryParams, __serialize);
extern PHP_METHOD(Uri_Rfc3986_UriQueryParams, __unserialize);
extern PHP_METHOD(Uri_Rfc3986_UriQueryParams, __debugInfo);

extern PHP_METHOD(Uri_WhatWg_UrlQueryParams, fromQueryString);
extern PHP_METHOD(Uri_WhatWg_UrlQueryParams, fromArray);
extern PHP_METHOD(Uri_WhatWg_UrlQueryParams, append);
extern PHP_METHOD(Uri_WhatWg_UrlQueryParams, delete);
extern PHP_METHOD(Uri_WhatWg_UrlQueryParams, has);
extern PHP_METHOD(Uri_WhatWg_UrlQueryParams, getFirst);
extern PHP_METHOD(Uri_WhatWg_UrlQueryParams, getLast);
extern PHP_METHOD(Uri_WhatWg_UrlQueryParams, getAll);
extern PHP_METHOD(Uri_WhatWg_UrlQueryParams, set);
extern PHP_METHOD(Uri_WhatWg_UrlQueryParams, sort);
extern PHP_METHOD(Uri_WhatWg_UrlQueryParams, toString);
extern PHP_METHOD(Uri_WhatWg_UrlQueryParams, __serialize);
extern PHP_METHOD(Uri_WhatWg_UrlQueryParams, __unserialize);
extern PHP_METHOD(Uri_WhatWg_UrlQueryParams, __debugInfo);

zend_object *php_uri_object_create_rfc3986_uri_query_params(zend_class_entry *ce);

#endif
