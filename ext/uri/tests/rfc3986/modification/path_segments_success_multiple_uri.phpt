--TEST--
Test Uri\Rfc3986\Uri component modification - path segments - multiple path segments for URI
--EXTENSIONS--
uri
--FILE--
<?php

$uri1 = Uri\Rfc3986\Uri::parse("https://example.com/foo");
$uri2 = $uri1->withPathSegments(["foo", "bar", "baz"]);

var_dump($uri1->getRawPath());
var_dump($uri1->getRawPathSegments());
var_dump($uri2->getRawPath());
var_dump($uri2->getRawPathSegments());
var_dump($uri2->toRawString());
var_dump($uri2->getPath());
var_dump($uri2->getPathSegments());
var_dump($uri2->toString());

?>
--EXPECT--
string(4) "/foo"
array(1) {
  [0]=>
  string(3) "foo"
}
string(12) "/foo/bar/baz"
array(3) {
  [0]=>
  string(3) "foo"
  [1]=>
  string(3) "bar"
  [2]=>
  string(3) "baz"
}
string(31) "https://example.com/foo/bar/baz"
string(12) "/foo/bar/baz"
array(3) {
  [0]=>
  string(3) "foo"
  [1]=>
  string(3) "bar"
  [2]=>
  string(3) "baz"
}
string(31) "https://example.com/foo/bar/baz"
