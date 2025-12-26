--TEST--
Test Uri\Rfc3986\Uri component retrieval - path segments - URI with zero segments
--FILE--
<?php

$uri = Uri\Rfc3986\Uri::parse("");

var_dump($uri->getRawPathSegments());
var_dump($uri->getPathSegments());

?>
--EXPECT--
array(0) {
}
array(0) {
}
