--TEST--
Test Uri\Rfc3986\UriQueryParams::append() - success - multi-level array value
--FILE--
<?php

$params = Uri\Rfc3986\UriQueryParams::parseRfc3986("");
$params->append("foo", [[0 => "bar"], [123]]);

var_dump($params);
var_dump($params->toRfc3986String());

?>
--EXPECTF--
object(Uri\Rfc3986\UriQueryParams)#%d (%d) {
  ["foo[][]"]=>
  string(3) "bar"
  ["foo[][]"]=>
  string(3) "123"
}
string(39) "foo%5B%5D%5B%5D=bar&foo%5B%5D%5B%5D=123"
