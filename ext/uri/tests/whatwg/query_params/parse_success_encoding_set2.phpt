--TEST--
Test Uri\WhatWg\UrlQueryParams::parse() - success - characters in the application/x-www-form-urlencoded percent-encode set
--FILE--
<?php

$params = Uri\WhatWg\UrlQueryParams::parse("abc!=123&foo'=bar)");

var_dump($params);
var_dump($params->toString());

?>
--EXPECTF--
object(Uri\WhatWg\UrlQueryParams)#%d (%d) {
  ["abc!"]=>
  string(3) "123"
  ["foo'"]=>
  string(4) "bar)"
}
string(24) "abc%21=123&foo%27=bar%29"
