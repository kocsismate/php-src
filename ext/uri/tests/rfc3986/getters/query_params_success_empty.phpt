--TEST--
Test Uri\Rfc3986\Uri::getQueryParams() - success - empty query string
--FILE--
<?php

$uri = Uri\Rfc3986\Uri::parse("https://example.com/?");

var_dump($uri->getRawQueryParams());
var_dump($uri->getQueryParams());

?>
--EXPECTF--
object(Uri\Rfc3986\UriQueryParams)#%d (%d) {
}
object(Uri\Rfc3986\UriQueryParams)#%d (%d) {
}
