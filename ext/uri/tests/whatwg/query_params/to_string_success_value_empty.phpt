--TEST--
Test Uri\WhatWg\UrlQueryParams::toString() - success - empty values
--FILE--
<?php

$params = Uri\WhatWg\UrlQueryParams::parse("foo=&bar");

var_dump($params->toString());

?>
--EXPECT--
string(9) "foo=&bar="
