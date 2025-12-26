--TEST--
Test Uri\WhatWg\UrlQueryParams::append() - success - multi-level array value
--FILE--
<?php

$params = Uri\WhatWg\UrlQueryParams::parse("");
$params->append("foo", ["bar" => [0 => "baz", 2 => 123, 3 => [], 4 => [1, 2, 3]]]);

var_dump($params);
var_dump($params->toString());

?>
--EXPECTF--
object(Uri\WhatWg\UrlQueryParams)#%d (%d) {
  ["foo[bar][0]"]=>
  string(3) "baz"
  ["foo[bar][2]"]=>
  string(3) "123"
  ["foo[bar][3]"]=>
  string(0) ""
  ["foo[bar][4][]"]=>
  string(1) "1"
  ["foo[bar][4][]"]=>
  string(1) "2"
  ["foo[bar][4][]"]=>
  string(1) "3"
}
string(152) "foo%5Bbar%5D%5B0%5D=baz&foo%5Bbar%5D%5B2%5D=123&foo%5Bbar%5D%5B3%5D=&foo%5Bbar%5D%5B4%5D%5B%5D=1&foo%5Bbar%5D%5B4%5D%5B%5D=2&foo%5Bbar%5D%5B4%5D%5B%5D=3"
