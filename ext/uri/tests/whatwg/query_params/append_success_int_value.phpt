--TEST--
Test Uri\WhatWg\UrlQueryParams::append() - success - integer value

--FILE--
<?php

$params = Uri\WhatWg\UrlQueryParams::parse("abc=123&foo=bar");
$params->append("baz", 0);

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
  string(1) "0"
}
string(21) "abc=123&foo=bar&baz=0"
