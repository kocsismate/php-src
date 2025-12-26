--TEST--
Test Uri\Rfc3986\Uri::withQueryParams() - success - basic
--FILE--
<?php

$uri1 = Uri\Rfc3986\Uri::parse("https://example.com/");
$queryParams = Uri\Rfc3986\UriQueryParams::parseRfc3986("abc=123&foo=bar");
$uri2 = $uri1->withQueryParams($queryParams);

var_dump($uri1->getRawQuery());
var_dump($uri2->getRawQuery());
var_dump($uri2->getQuery());
var_dump($uri2->getRawQueryParams());
var_dump($uri2->getQueryParams());
var_dump($uri2->toString());

?>
--EXPECTF--
NULL
string(15) "abc=123&foo=bar"
string(15) "abc=123&foo=bar"
object(Uri\Rfc3986\UriQueryParams)#%d (%d) {
  ["abc"]=>
  string(3) "123"
  ["foo"]=>
  string(3) "bar"
}
object(Uri\Rfc3986\UriQueryParams)#%d (%d) {
  ["abc"]=>
  string(3) "123"
  ["foo"]=>
  string(3) "bar"
}
string(36) "https://example.com/?abc=123&foo=bar"
