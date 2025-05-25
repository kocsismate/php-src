--TEST--
Test IDNA support
--EXTENSIONS--
uri
--FILE--
<?php

$uri = Uri\Rfc3986Uri::create("🐘");
var_dump($uri);

$uri = Uri\WhatWgUri::create("🐘", null);
var_dump($uri);
var_dump($uri[0]->errorCode === \Uri\WhatWgError::ERROR_TYPE_MISSING_SCHEME_NON_RELATIVE_URL);

?>
--EXPECTF--
NULL
array(1) {
  [0]=>
  object(Uri\WhatWgError)#%d (%d) {
    ["uri"]=>
    string(4) "🐘"
    ["position"]=>
    string(4) "🐘"
    ["errorCode"]=>
    int(21)
  }
}
bool(true)
