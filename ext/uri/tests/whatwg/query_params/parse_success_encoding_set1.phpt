--TEST--
Test Uri\WhatWg\UrlQueryParams::parse() - success - characters in the query string percent-encode set
--FILE--
<?php

$params = Uri\WhatWg\UrlQueryParams::parse("ab:c=#123&<foo>=b@r");

var_dump($params);
var_dump($params->toString());

?>
--EXPECTF--
object(Uri\WhatWg\UrlQueryParams)#%d (%d) {
  ["ab:c"]=>
  string(4) "#123"
  ["<foo>"]=>
  string(3) "b@r"
}
string(29) "ab%3Ac=%23123&%3Cfoo%3E=b%40r"
