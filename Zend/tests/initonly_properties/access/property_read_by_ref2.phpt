--TEST--
Test that initonly properties of composite type can't be accessed by reference
--FILE--
<?php

class Foo
{
    initonly public array $property1 = [];
    initonly public stdClass $property2;

    initonly public static array $property3 = [];
    initonly public static stdClass $property4;
}

$foo = new Foo();
$foo->property2 = new stdClass();
Foo::$property4 = new stdClass();

$var = null;

try {
    $var = &$foo->property1;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $var = &$foo->property2;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $var = &Foo::$property3;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $var = &Foo::$property4;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

?>
--EXPECT--
Cannot acquire reference to initonly property Foo::$property1
Cannot acquire reference to initonly property Foo::$property2
Cannot modify initonly static property Foo::$property3 after initialization
Cannot modify initonly static property Foo::$property4 after initialization
