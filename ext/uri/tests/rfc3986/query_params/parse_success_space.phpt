--TEST--
Test Uri\Rfc3986\UriQueryParams::parseRfc3986() - success - space character
--FILE--
<?php

$params = Uri\Rfc3986\UriQueryParams::parseRfc3986("a c=123&foo=b r");

var_dump($params);
var_dump($params->toRfc3986String());

?>
--EXPECTF--
object(Uri\Rfc3986\UriQueryParams)#%d (%d) {
  ["a c"]=>
  string(3) "123"
  ["foo"]=>
  string(3) "b r"
}
string(15) "a+c=123&foo=b+r"
