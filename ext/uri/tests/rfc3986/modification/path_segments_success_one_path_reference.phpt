--TEST--
Test Uri\Rfc3986\Uri component modification - path segments - one segment for path reference
--EXTENSIONS--
uri
--FILE--
<?php

$uri1 = Uri\Rfc3986\Uri::parse("foo");
$uri2 = $uri1->withPathSegments(["baz"], true);

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
string(3) "foo"
array(1) {
  [0]=>
  string(3) "foo"
}
string(4) "/baz"
array(1) {
  [0]=>
  string(3) "baz"
}
string(4) "/baz"
string(4) "/baz"
array(1) {
  [0]=>
  string(3) "baz"
}
string(4) "/baz"
