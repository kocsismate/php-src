--TEST--
Test Uri\Rfc3986\Uri::percentEncode() - success - no reserved characters
--FILE--
<?php

var_dump(Uri\Rfc3986\Uri::percentEncode("foo 123 _ -", Uri\Rfc3986\UriPercentEncodingMode::AllReservedCharacters));

?>
--EXPECT--
string(11) "foo 123 _ -"
