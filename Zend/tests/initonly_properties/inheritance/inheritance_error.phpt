--TEST--
Test that non-private initonly properties can't override non-initonly properties
--FILE--
<?php

class Foo
{
    protected int $property1;
}

class Bar extends Foo
{
    initonly protected int $property1;
}

?>
--EXPECTF--
Fatal error: Cannot redeclare non-initonly property Foo::$property1 as initonly Bar::$property1 in %s on line %d
