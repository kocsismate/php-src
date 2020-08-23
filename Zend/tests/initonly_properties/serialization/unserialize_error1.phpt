--TEST--
Test that initonly properties can only be assigned to once during unserialization
--FILE--
<?php

class MyClass
{
    initonly public int $finalProp;
}

var_dump(unserialize('O:7:"MyClass":2:{s:9:"finalProp";i:2;s:9:"finalProp";i:3;}'));

?>
--EXPECTF--
Cannot declare function Foo::$bar initonly, the initonly modifier is allowed only for properties in %s on line %d
