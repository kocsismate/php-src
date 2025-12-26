--TEST--
Test Uri\Rfc3986\Uri::percentEncode() - success - reserved characters basic
--FILE--
<?php

var_dump(Uri\Rfc3986\Uri::percentEncode(": / ? # [ ] @ ! $ & ' ( ) * + , ; =", Uri\Rfc3986\UriPercentEncodingMode::AllReservedCharacters));

?>
--EXPECT--
string(71) "%3A %2F %3F %23 %5B %5D %40 %21 %24 %26 %27 %28 %29 %2A %2B %2C %3B %3D"
