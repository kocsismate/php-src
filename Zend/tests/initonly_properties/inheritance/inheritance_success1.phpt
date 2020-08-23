--TEST--
Test that non-initonly properties can override initonly properties
--FILE--
<?php

class Foo
{
    initonly private static int $property1;
    initonly private int $property2;

    initonly protected static int $property3;
    initonly protected int $property4;
}

class Bar extends Foo
{
    initonly private static int $property1;
    initonly private int $property2;

    protected static int $property3;
    protected int $property4;
}

?>
--EXPECT--
