--TEST--
Test Uri\WhatWg\UrlQueryParams::delete() - success - nonexistent key
--FILE--
<?php

$params = Uri\WhatWg\UrlQueryParams::parse("abc=123&foo=bar");
$params->delete("baz");

var_dump($params);
var_dump($params->toString());

?>
--EXPECTF--
object(Uri\WhatWg\UrlQueryParams)#%d (%d) {
  ["abc"]=>
  string(3) "123"
  ["foo"]=>
  string(3) "bar"
}
string(15) "abc=123&foo=bar"
