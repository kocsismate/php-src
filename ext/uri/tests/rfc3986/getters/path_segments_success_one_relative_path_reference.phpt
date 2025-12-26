--TEST--
Test Uri\Rfc3986\Uri component retrieval - path segments - relative path reference with one non-empty segment
--FILE--
<?php

$uri = Uri\Rfc3986\Uri::parse("foo");

var_dump($uri->getRawPathSegments());
var_dump($uri->getPathSegments());

?>
--EXPECT--
array(1) {
  [0]=>
  string(3) "foo"
}
array(1) {
  [0]=>
  string(3) "foo"
}
