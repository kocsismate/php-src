--TEST--
Test Uri\Rfc3986\Uri component modification - path segments - zero segments for URI
--EXTENSIONS--
uri
--FILE--
<?php

$uri1 = Uri\Rfc3986\Uri::parse("https://example.com/foo");
$uri2 = $uri1->withPathSegments([]);

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
string(0) ""
array(0) {
}
string(19) "https://example.com"
string(0) ""
array(0) {
}
string(19) "https://example.com"
