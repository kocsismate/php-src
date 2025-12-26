--TEST--
Test Uri\WhatWg\UrlQueryParams::append() - success - list
--FILE--
<?php

$params = Uri\WhatWg\UrlQueryParams::parse("foo=0");
$params->append("foo", [1, 2, 3]);

var_dump($params);
var_dump($params->toString());

?>
--EXPECTF--
object(Uri\WhatWg\UrlQueryParams)#%d (%d) {
  ["foo"]=>
  string(1) "0"
  ["foo[]"]=>
  string(1) "1"
  ["foo[]"]=>
  string(1) "2"
  ["foo[]"]=>
  string(1) "3"
}
string(41) "foo=0&foo%5B%5D=1&foo%5B%5D=2&foo%5B%5D=3"
