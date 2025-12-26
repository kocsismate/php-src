--TEST--
Test Uri\Rfc3986\UriQueryParams::toRfc3986String() - success - empty values
--FILE--
<?php

$params = Uri\Rfc3986\UriQueryParams::parseRfc3986("foo=&bar");

var_dump($params->toRfc3986String());

?>
--EXPECT--
string(8) "foo=&bar"
