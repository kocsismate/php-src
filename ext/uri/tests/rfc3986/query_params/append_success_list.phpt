--TEST--
Test Uri\Rfc3986\UriQueryParams::append() - success - list
--FILE--
<?php

$params = Uri\Rfc3986\UriQueryParams::parseRfc3986("foo=0");
$params->append("foo", [1, 2, 3]);

var_dump($params);
var_dump($params->toRfc3986String());

?>
--EXPECTF--
object(Uri\Rfc3986\UriQueryParams)#%d (%d) {
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
