--TEST--
Test Uri\WhatWg\Url::getQueryParams() - success - non-existent query string
--FILE--
<?php

$uri = Uri\WhatWg\Url::parse("https://example.com/");

var_dump($uri->getQueryParams());

?>
--EXPECT--
NULL
