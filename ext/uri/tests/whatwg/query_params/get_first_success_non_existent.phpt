--TEST--
Test Uri\WhatWg\UrlQueryParams::get_first() - success - item not exists
--FILE--
<?php

$params = Uri\WhatWg\UrlQueryParams::parse("foo=bar&foo=baz&foo=qux");

var_dump($params->getFirst("baz"));

?>
--EXPECT--
NULL
