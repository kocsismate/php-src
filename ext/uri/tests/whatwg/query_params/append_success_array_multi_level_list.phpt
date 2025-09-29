--TEST--
Test Uri\WhatWg\UrlQueryParams::append() - success - multi-level array value
--FILE--
<?php

$params = Uri\WhatWg\UrlQueryParams::parse("");
$params->append("foo", [[0 => "bar"], [123]]);

var_dump($params);
var_dump($params->toString());

?>
--EXPECTF--
object(Uri\WhatWg\UrlQueryParams)#%d (%d) {
  ["foo[][]"]=>
  string(3) "bar"
  ["foo[][]"]=>
  string(3) "123"
}
string(39) "foo%5B%5D%5B%5D=bar&foo%5B%5D%5B%5D=123"
