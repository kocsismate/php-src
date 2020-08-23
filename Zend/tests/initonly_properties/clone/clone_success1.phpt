--TEST--
Test that initonly properties can be cloned
--FILE--
<?php

class Foo
{
    initonly public int $property1;
    initonly public int $property2;
    initonly public string $property3 = "";
}

$foo = new Foo();
$foo->property2 = 2;
var_dump($foo);
$bar = clone $foo;
var_dump($bar);

?>
--EXPECTF--
object(Foo)#1 (2) {
  ["property1"]=>
  uninitialized(int)
  ["property2"]=>
  int(2)
  ["property3"]=>
  string(0) ""
}
object(Foo)#2 (2) {
  ["property1"]=>
  uninitialized(int)
  ["property2"]=>
  int(2)
  ["property3"]=>
  string(0) ""
}
