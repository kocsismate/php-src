--TEST--
Test Uri\Rfc3986\Uri component retrieval - path segments - URI with an empty trailing segment
--FILE--
<?php

$uri = Uri\Rfc3986\Uri::parse("https://example.com/foo/");

var_dump($uri->getRawPathSegments());
var_dump($uri->getPathSegments());

?>
--EXPECT--
array(2) {
  [0]=>
  string(3) "foo"
  [1]=>
  string(0) ""
}
array(2) {
  [0]=>
  string(3) "foo"
  [1]=>
  string(0) ""
}
