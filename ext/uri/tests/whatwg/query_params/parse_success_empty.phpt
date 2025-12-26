--TEST--
Test Uri\WhatWg\UrlQueryParams::parse() - success - empty string
--FILE--
<?php

$params = Uri\WhatWg\UrlQueryParams::parse("");

var_dump($params);
var_dump($params->toString());

?>
--EXPECTF--
object(Uri\WhatWg\UrlQueryParams)#%d (%d) {
}
string(0) ""
