--TEST--
Test that non-private initonly static properties can't override non-initonly static properties
--FILE--
<?php

class Foo
{
    protected static int $property1;
}

class Bar extends Foo
{
    initonly protected static int $property1;
}

?>
--EXPECTF--
Fatal error: Cannot redeclare non-initonly property Foo::$property1 as initonly Bar::$property1 in %s on line %d
