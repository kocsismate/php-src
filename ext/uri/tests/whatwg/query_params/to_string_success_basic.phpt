--TEST--
Test Uri\WhatWg\UrlQueryParams::toString() - success - basic
--FILE--
<?php

$params = Uri\WhatWg\UrlQueryParams::parse("abc=123&foo=bar");

var_dump($params->toString());

?>
--EXPECT--
string(15) "abc=123&foo=bar"
