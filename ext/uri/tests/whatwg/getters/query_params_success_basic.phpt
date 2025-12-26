--TEST--
Test Uri\WhatWg\Url::getQueryParams() - success - basic
--FILE--
<?php

$url = Uri\WhatWg\Url::parse("https://example.com/?%61bc=123&foo=b%61r"); // abc=123&foo=bar

var_dump($url->getQueryParams());

?>
--EXPECTF--
object(Uri\WhatWg\UrlQueryParams)#%d (%d) {
  ["abc"]=>
  string(3) "123"
  ["foo"]=>
  string(3) "bar"
}
