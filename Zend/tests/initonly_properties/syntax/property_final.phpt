--TEST--
Test valid syntax of initonly properties
--FILE--
<?php

class Foo
{
    initonly private int $property1;
    initonly protected int $property2 = 0;
    initonly protected static int $property3;
    initonly protected static int $property4 = 0;
}

?>
--EXPECT--
