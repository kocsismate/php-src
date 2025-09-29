--TEST--
Test Uri\WhatWg\Url::getQueryParams() - success - empty query string
--FILE--
<?php

$url = Uri\WhatWg\Url::parse("https://example.com/?");

var_dump($url->getQueryParams());

?>
--EXPECTF--
object(Uri\WhatWg\UrlQueryParams)#%d (%d) {
}
