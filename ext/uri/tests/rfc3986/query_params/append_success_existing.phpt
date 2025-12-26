--TEST--
Test Uri\Rfc3986\UriQueryParams::append() - success - existing parameter
--FILE--
<?php

$params = Uri\Rfc3986\UriQueryParams::parseRfc3986("abc=123&foo=bar");
$params->append("foo", "bar");

var_dump($params);
var_dump($params->toRfc3986String());

?>
--EXPECTF--
object(Uri\Rfc3986\UriQueryParams)#%d (%d) {
  ["abc"]=>
  string(3) "123"
  ["foo"]=>
  string(3) "bar"
  ["foo"]=>
  string(3) "bar"
}
string(23) "abc=123&foo=bar&foo=bar"
