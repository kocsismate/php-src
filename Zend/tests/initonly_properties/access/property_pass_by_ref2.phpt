--TEST--
Test that initonly properties can't be passed by reference
--FILE--
<?php

class Foo
{
    initonly public array $property1 = ["field" => "abc"];
    initonly public static array $property2 = ["field" => "abc"];
}

$foo = new Foo();

try {
    preg_match("/a/", "b", $foo->property1["field"]);
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    sort($foo->property1["field"]);
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    preg_match("/a/", "a", Foo::$property2["field"]);
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    sort(Foo::$property2["field"]);
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

?>
--EXPECT--
Cannot modify initonly property Foo::$property1 after initialization
Cannot modify initonly property Foo::$property1 after initialization
Cannot modify initonly static property Foo::$property2 after initialization
Cannot modify initonly static property Foo::$property2 after initialization
