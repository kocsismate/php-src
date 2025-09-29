--TEST--
Test Uri\Rfc3986\UriQueryParams::parseRfc3986() - success - reserved characters are not validated
--FILE--
<?php

$params = Uri\Rfc3986\UriQueryParams::parseRfc3986("ab:c=#123&<foo>=b@r");

var_dump($params);
var_dump($params->toRfc3986String());

?>
--EXPECTF--
object(Uri\Rfc3986\UriQueryParams)#%d (%d) {
  ["ab:c"]=>
  string(4) "#123"
  ["<foo>"]=>
  string(3) "b@r"
}
string(29) "ab%3Ac=%23123&%3Cfoo%3E=b%40r"
