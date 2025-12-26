--TEST--
Test Uri\WhatWg\Url::getQueryParams() - success - non-existent query string
--FILE--
<?php

$url = Uri\WhatWg\Url::parse("https://example.com/");

var_dump($url->getQueryParams());

?>
--EXPECT--
NULL
