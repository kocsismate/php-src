--TEST--
Test Uri\WhatWg\UrlQueryParams::append() - success - existing parameter
--FILE--
<?php

$params = Uri\WhatWg\UrlQueryParams::parse("abc=123&foo=bar");
$params->append("foo", "bar");

var_dump($params);
var_dump($params->toString());

?>
--EXPECTF--
object(Uri\WhatWg\UrlQueryParams)#%d (%d) {
  ["abc"]=>
  string(3) "123"
  ["foo"]=>
  string(3) "bar"
  ["foo"]=>
  string(3) "bar"
}
string(23) "abc=123&foo=bar&foo=bar"
