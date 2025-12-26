--TEST--
Test Uri\WhatWg\UrlQueryParams::delete() - success - basic
--FILE--
<?php

$params = Uri\WhatWg\UrlQueryParams::parse("abc=123&foo=bar");
$params->delete("abc");

var_dump($params);
var_dump($params->toString());

?>
--EXPECTF--
object(Uri\WhatWg\UrlQueryParams)#%d (%d) {
  ["foo"]=>
  string(3) "bar"
}
string(7) "foo=bar"
