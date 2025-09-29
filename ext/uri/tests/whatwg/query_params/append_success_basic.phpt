--TEST--
Test Uri\WhatWg\UrlQueryParams::append() - success - basic
--FILE--
<?php

$params = Uri\WhatWg\UrlQueryParams::parse("abc=123&foo=bar");
$params->append("baz", "qux");

var_dump($params);
var_dump($params->toString());

?>
--EXPECTF--
object(Uri\WhatWg\UrlQueryParams)#%d (%d) {
  ["abc"]=>
  string(3) "123"
  ["foo"]=>
  string(3) "bar"
  ["baz"]=>
  string(3) "qux"
}
string(23) "abc=123&foo=bar&baz=qux"
