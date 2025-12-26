--TEST--
Test Uri\Rfc3986\Uri::percentEncode() - success - user info
--FILE--
<?php

var_dump(Uri\Rfc3986\Uri::percentEncode("us%rname:p@ssword", Uri\Rfc3986\UriPercentEncodingMode::UserInfo));

?>
--EXPECT--
string(21) "us%25rname:p%40ssword"
