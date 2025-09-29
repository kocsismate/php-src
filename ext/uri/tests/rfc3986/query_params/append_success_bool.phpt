--TEST--
Test Uri\Rfc3986\UriQueryParams::append() - success - bool value
--FILE--
<?php

$params = Uri\Rfc3986\UriQueryParams::parseRfc3986("abc=123&foo=bar");
$params->append("baz", false);
$params->append("qux", true);

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
  string(1) "0"
  ["qux"]=>
  string(1) "1"
}
string(27) "abc=123&foo=bar&baz=0&qux=1"
