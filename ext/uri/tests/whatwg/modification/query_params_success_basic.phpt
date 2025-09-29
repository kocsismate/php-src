--TEST--
Test Uri\WhatWg\Url::withQueryParams() - success - basic
--FILE--
<?php

$uri1 = Uri\WhatWg\Url::parse("https://example.com/");
$queryParams = Uri\WhatWg\UrlQueryParams::parse("abc=123&foo=bar");
$uri2 = $uri1->withQueryParams($queryParams);

var_dump($uri1->getQuery());
var_dump($uri2->getQuery());
var_dump($uri2->getQueryParams());
var_dump($uri2->toAsciiString());

?>
--EXPECTF--
NULL
string(15) "abc=123&foo=bar"
object(Uri\WhatWg\UrlQueryParams)#%d (%d) {
  ["abc"]=>
  string(3) "123"
  ["foo"]=>
  string(3) "bar"
}
string(36) "https://example.com/?abc=123&foo=bar"
