--TEST--
Test Uri\WhatWg\Url::getQueryParams() - success - empty query string
--FILE--
<?php

$uri = Uri\WhatWg\Url::parse("https://example.com/?");

var_dump($uri->getQueryParams());

?>
--EXPECTF--
object(Uri\WhatWg\UrlQueryParams)#%d (%d) {
}
