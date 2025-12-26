--TEST--
Test Uri\Rfc3986\Uri::getQueryParams() - success - basic
--FILE--
<?php

$uri = Uri\Rfc3986\Uri::parse("https://example.com/?%61bc=123&foo=b%61r"); // abc=123&foo=bar

var_dump($uri->getRawQueryParams());
var_dump($uri->getQueryParams());

?>
--EXPECTF--
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
