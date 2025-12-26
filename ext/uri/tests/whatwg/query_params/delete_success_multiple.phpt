--TEST--
Test Uri\WhatWg\UrlQueryParams::delete() - success - basic
--FILE--
<?php

$params = Uri\WhatWg\UrlQueryParams::parse("abc=123&foo=bar&foo=baz");
$params->delete("foo");

var_dump($params);
var_dump($params->toString());

?>
--EXPECTF--
object(Uri\WhatWg\UrlQueryParams)#%d (%d) {
  ["abc"]=>
  string(3) "123"
}
string(7) "abc=123"
