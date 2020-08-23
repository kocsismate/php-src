--TEST--
Test that uninitialized initonly properties can be initialized after cloning
--FILE--
<?php

class Foo
{
    initonly public int $property1;
}

$foo = new Foo();
var_dump($foo);
$bar = clone $foo;
$bar->property1 = 1;
var_dump($bar);

?>
--EXPECTF--
object(Foo)#1 (0) {
  ["property1"]=>
  uninitialized(int)
}
object(Foo)#2 (1) {
  ["property1"]=>
  int(1)
}
