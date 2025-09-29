--TEST--
Test Uri\Rfc3986\UriQueryParams::parseRfc3986() - success - empty string
--FILE--
<?php

$params = Uri\Rfc3986\UriQueryParams::parseRfc3986("");

var_dump($params);

?>
--EXPECTF--
object(Uri\Rfc3986\UriQueryParams)#%d (%d) {
}
