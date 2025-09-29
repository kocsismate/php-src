--TEST--
Test Uri\WhatWg\UrlQueryParams::parse() - success - space character
--FILE--
<?php

$params = Uri\WhatWg\UrlQueryParams::parse("a c=123&foo=b r");

var_dump($params);
var_dump($params->toString());

?>
--EXPECTF--
object(Uri\WhatWg\UrlQueryParams)#%d (%d) {
  ["a c"]=>
  string(3) "123"
  ["foo"]=>
  string(3) "b r"
}
string(15) "a+c=123&foo=b+r"
