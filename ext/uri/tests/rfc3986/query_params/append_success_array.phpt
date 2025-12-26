--TEST--
Test Uri\Rfc3986\UriQueryParams::append() - success - array value
--FILE--
<?php

$params = Uri\Rfc3986\UriQueryParams::parseRfc3986("");
$params->append("baz", [null, false, "key2" => true, 123, "", ["bla"]]);

var_dump($params);
var_dump($params->toRfc3986String());

?>
--EXPECTF--
object(Uri\Rfc3986\UriQueryParams)#%d (%d) {
  ["baz[0]"]=>
  NULL
  ["baz[1]"]=>
  string(1) "0"
  ["baz[key2]"]=>
  string(1) "1"
  ["baz[2]"]=>
  string(3) "123"
  ["baz[3]"]=>
  string(0) ""
  ["baz[4][]"]=>
  string(3) "bla"
}
string(87) "baz%5B0%5D&baz%5B1%5D=0&baz%5Bkey2%5D=1&baz%5B2%5D=123&baz%5B3%5D=&baz%5B4%5D%5B%5D=bla"
