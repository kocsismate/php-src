--TEST--
Test Uri\WhatWg\UrlQueryParams::get_first() - success - multiple items
--FILE--
<?php

$params = Uri\WhatWg\UrlQueryParams::parse("foo=bar&foo=baz&foo=qux");

var_dump($params->getFirst("foo"));

?>
--EXPECT--
string(3) "bar"
