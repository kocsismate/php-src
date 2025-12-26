--TEST--
Test Uri\Rfc3986\UriQueryParams::append() - success - empty array value
--FILE--
<?php

$params = Uri\Rfc3986\UriQueryParams::parseRfc3986("abc=123&foo=bar");
$params->append("baz", []);

var_dump($params);
var_dump($params->toRfc3986String());

?>
--EXPECTF--
object(Uri\Rfc3986\UriQueryParams)#%d (%d) {
  ["abc"]=>
  string(3) "123"
  ["foo"]=>
  string(3) "bar"
  ["baz"]=>
  NULL
}
string(19) "abc=123&foo=bar&baz"
