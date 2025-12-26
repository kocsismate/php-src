--TEST--
Test Uri\Rfc3986\Uri::getQueryParams() - success - non-existent query string
--FILE--
<?php

$uri = Uri\Rfc3986\Uri::parse("https://example.com/");

var_dump($uri->getRawQueryParams());
var_dump($uri->getQueryParams());

?>
--EXPECT--
NULL
NULL
