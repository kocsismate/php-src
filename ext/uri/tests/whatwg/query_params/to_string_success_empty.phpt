--TEST--
Test Uri\WhatWg\UrlQueryParams::toString() - success - empty query string
--FILE--
<?php

$params = Uri\WhatWg\UrlQueryParams::parse("");

var_dump($params->toString());

?>
--EXPECT--
string(0) ""
