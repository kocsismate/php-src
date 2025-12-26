--TEST--
Test Uri\Rfc3986\Uri component retrieval - path segments - URI without authority with one empty segment
--FILE--
<?php

$uri = Uri\Rfc3986\Uri::parse("https:/");

var_dump($uri->getRawPathSegments());
var_dump($uri->getPathSegments());

?>
--EXPECT--
array(1) {
  [0]=>
  string(0) ""
}
array(1) {
  [0]=>
  string(0) ""
}
