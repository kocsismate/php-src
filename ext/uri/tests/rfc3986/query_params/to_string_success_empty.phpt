--TEST--
Test Uri\Rfc3986\UriQueryParams::toRfc3986String() - success - empty query string
--FILE--
<?php

$params = Uri\Rfc3986\UriQueryParams::parseRfc3986("");

var_dump($params->toRfc3986String());

?>
--EXPECT--
string(0) ""
