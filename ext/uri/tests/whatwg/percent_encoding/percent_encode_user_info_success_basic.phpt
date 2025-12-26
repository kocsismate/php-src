--TEST--
Test Uri\WhatWg\Url::percentEncode() - success - user info
--FILE--
<?php

var_dump(Uri\WhatWg\Url::percentEncode("us%rname:p@ssword", Uri\WhatWg\UrlPercentEncodingMode::UserInfo));

?>
--EXPECT--
string(21) "us%rname%3Ap%40ssword"
