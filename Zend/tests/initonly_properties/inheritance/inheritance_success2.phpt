--TEST--
Test that private initonly properties can override non-initonly properties
--FILE--
<?php

class Foo
{
    private static int $property1;
    private int $property2;
}

class Bar extends Foo
{
    initonly private static int $property1;
    initonly private int $property2;
}

?>
--EXPECT--
