--TEST--
Test Uri\Rfc3986\Uri component retrieval - path segments - URI with multiple segments
--FILE--
<?php

$uri = Uri\Rfc3986\Uri::parse("https://example.com/foo/bar/baz");

var_dump($uri->getRawPathSegments());
var_dump($uri->getPathSegments());

?>
--EXPECT--
array(3) {
  [0]=>
  string(3) "foo"
  [1]=>
  string(3) "bar"
  [2]=>
  string(3) "baz"
}
array(3) {
  [0]=>
  string(3) "foo"
  [1]=>
  string(3) "bar"
  [2]=>
  string(3) "baz"
}
