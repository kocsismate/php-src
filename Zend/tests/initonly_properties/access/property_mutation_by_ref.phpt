--TEST--
Test that initonly properties can't be mutated by reference
--FILE--
<?php

class Foo
{
    initonly public int $property1;
    initonly public int $property2 = 2;

    public function __construct()
    {
    }
}

$foo = new Foo();

$x = 1;
try {
    $foo->property1 = &$x;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

$foo->property1 = 1;

try {
    $foo->property1 = &$x;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $foo->property2 = &$foo->property1;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

?>
--EXPECT--
Cannot assign to initonly property Foo::$property1 by reference
Cannot assign to initonly property Foo::$property1 by reference
Cannot acquire reference to initonly property Foo::$property1
