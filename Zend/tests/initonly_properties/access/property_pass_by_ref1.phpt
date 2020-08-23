--TEST--
Test that initonly properties can't be passed by reference
--FILE--
<?php

class Foo
{
    initonly public array $property1;
    initonly public array $property2 = [1, 2, 3];
    initonly public static array $property3;
    initonly public static array $property4 = [1, 2, 3];
}

$foo = new Foo();

try {
    preg_match("/a/", "a", $foo->property1);
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    sort($foo->property2);
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    preg_match("/a/", "a", Foo::$property3);
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    sort(Foo::$property4);
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

?>
--EXPECT--
Cannot acquire reference to initonly property Foo::$property1
Cannot acquire reference to initonly property Foo::$property2
Cannot acquire reference to initonly property Foo::$property3
Cannot modify initonly static property Foo::$property4 after initialization
