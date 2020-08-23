--TEST--
Test that initonly properties of scalar types can't be accessed by reference
--FILE--
<?php

class Foo
{
    initonly public bool $property1 = false;
    initonly public int $property2 = 2;
    initonly public string $property3 = "Bar";

    initonly public static bool $property4 = false;
    initonly public static int $property5 = 5;
    initonly public static string $property6 = "Baz";
}

$foo = new Foo();

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
    $var = &$foo->property3;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $var = &Foo::$property4;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $var = &Foo::$property5;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $var = &Foo::$property6;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

?>
--EXPECT--
Cannot acquire reference to initonly property Foo::$property1
Cannot acquire reference to initonly property Foo::$property2
Cannot acquire reference to initonly property Foo::$property3
Cannot modify initonly static property Foo::$property4 after initialization
Cannot modify initonly static property Foo::$property5 after initialization
Cannot modify initonly static property Foo::$property6 after initialization
