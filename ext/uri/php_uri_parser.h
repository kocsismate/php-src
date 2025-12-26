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

#ifndef PHP_URI_PARSER_H
#define PHP_URI_PARSER_H

extern PHP_METHOD(Uri_Rfc3986_Uri, __construct);
extern PHP_METHOD(Uri_Rfc3986_Uri, parse);
extern PHP_METHOD(Uri_Rfc3986_Uri, withPassword);
extern PHP_METHOD(Uri_Rfc3986_Uri, getRawScheme);
extern PHP_METHOD(Uri_Rfc3986_Uri, getScheme);
extern PHP_METHOD(Uri_Rfc3986_Uri, getRawUserInfo);
extern PHP_METHOD(Uri_Rfc3986_Uri, getUserInfo);
extern PHP_METHOD(Uri_Rfc3986_Uri, withUserInfo);
extern PHP_METHOD(Uri_Rfc3986_Uri, getRawUsername);
extern PHP_METHOD(Uri_Rfc3986_Uri, getUsername);
extern PHP_METHOD(Uri_Rfc3986_Uri, withUsername);
extern PHP_METHOD(Uri_Rfc3986_Uri, getRawPassword);
extern PHP_METHOD(Uri_Rfc3986_Uri, getPassword);
extern PHP_METHOD(Uri_Rfc3986_Uri, getRawHost);
extern PHP_METHOD(Uri_Rfc3986_Uri, getHost);
extern PHP_METHOD(Uri_Rfc3986_Uri, getHostType);
extern PHP_METHOD(Uri_Rfc3986_Uri, withHost);
extern PHP_METHOD(Uri_Rfc3986_Uri, getPort);
extern PHP_METHOD(Uri_Rfc3986_Uri, withPort);
extern PHP_METHOD(Uri_Rfc3986_Uri, getRawPath);
extern PHP_METHOD(Uri_Rfc3986_Uri, getPath);
extern PHP_METHOD(Uri_Rfc3986_Uri, getRawPathSegments);
extern PHP_METHOD(Uri_Rfc3986_Uri, getPathSegments);
extern PHP_METHOD(Uri_Rfc3986_Uri, withPath);
extern PHP_METHOD(Uri_Rfc3986_Uri, getRawQuery);
extern PHP_METHOD(Uri_Rfc3986_Uri, getQuery);
extern PHP_METHOD(Uri_Rfc3986_Uri, withQuery);
extern PHP_METHOD(Uri_Rfc3986_Uri, getRawQueryParams);
extern PHP_METHOD(Uri_Rfc3986_Uri, getQueryParams);
extern PHP_METHOD(Uri_Rfc3986_Uri, withQueryParams);
extern PHP_METHOD(Uri_Rfc3986_Uri, getRawFragment);
extern PHP_METHOD(Uri_Rfc3986_Uri, getFragment);
extern PHP_METHOD(Uri_Rfc3986_Uri, withFragment);
extern PHP_METHOD(Uri_Rfc3986_Uri, equals);
extern PHP_METHOD(Uri_Rfc3986_Uri, toRawString);
extern PHP_METHOD(Uri_Rfc3986_Uri, toString);
extern PHP_METHOD(Uri_Rfc3986_Uri, resolve);
extern PHP_METHOD(Uri_Rfc3986_Uri, __serialize);
extern PHP_METHOD(Uri_Rfc3986_Uri, __unserialize);
extern PHP_METHOD(Uri_Rfc3986_Uri, __debugInfo);

extern PHP_METHOD(Uri_WhatWg_InvalidUrlException, withPassword);
extern PHP_METHOD(Uri_WhatWg_UrlValidationError, withPassword);

extern PHP_METHOD(Uri_WhatWg_Url, __construct);
extern PHP_METHOD(Uri_WhatWg_Url, parse);
extern PHP_METHOD(Uri_WhatWg_Url, getScheme);
extern PHP_METHOD(Uri_WhatWg_Url, withScheme);
extern PHP_METHOD(Uri_WhatWg_Url, withUsername);
extern PHP_METHOD(Uri_WhatWg_Url, withPassword);
extern PHP_METHOD(Uri_WhatWg_Url, getAsciiHost);
extern PHP_METHOD(Uri_WhatWg_Url, getUnicodeHost);
extern PHP_METHOD(Uri_WhatWg_Url, getHostType);
extern PHP_METHOD(Uri_WhatWg_Url, getPathSegments);
extern PHP_METHOD(Uri_WhatWg_Url, getQueryParams);
extern PHP_METHOD(Uri_WhatWg_Url, withQueryParams);
extern PHP_METHOD(Uri_WhatWg_Url, getFragment);
extern PHP_METHOD(Uri_WhatWg_Url, equals);
extern PHP_METHOD(Uri_WhatWg_Url, toAsciiString);
extern PHP_METHOD(Uri_WhatWg_Url, toUnicodeString);
extern PHP_METHOD(Uri_WhatWg_Url, resolve);
extern PHP_METHOD(Uri_WhatWg_Url, __serialize);
extern PHP_METHOD(Uri_WhatWg_Url, __unserialize);
extern PHP_METHOD(Uri_WhatWg_Url, __debugInfo);

#endif
