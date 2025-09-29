--TEST--
Test Uri\WhatWg\UrlQueryParams::has() - success - basic
--FILE--
<?php

$params = Uri\WhatWg\UrlQueryParams::parse("abc=123&foo=bar");

var_dump($params->has("abc"));

?>
--EXPECT--
bool(true)
