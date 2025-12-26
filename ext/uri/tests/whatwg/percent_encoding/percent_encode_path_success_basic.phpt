--TEST--
Test Uri\WhatWg\Url::percentEncode() - success - path
--FILE--
<?php

var_dump(Uri\WhatWg\Url::percentEncode("/path@(/!+#abc", Uri\WhatWg\UrlPercentEncodingMode::UserInfo));

?>
--EXPECT--
string(22) "%2Fpath%40(%2F!+%23abc"
