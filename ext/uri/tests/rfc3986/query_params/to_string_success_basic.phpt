--TEST--
Test Uri\Rfc3986\UriQueryParams::toRfc3986String() - success - basic
--FILE--
<?php

$params = Uri\Rfc3986\UriQueryParams::parseRfc3986("abc=123&foo=bar");

var_dump($params->toRfc3986String());

?>
--EXPECT--
string(15) "abc=123&foo=bar"
