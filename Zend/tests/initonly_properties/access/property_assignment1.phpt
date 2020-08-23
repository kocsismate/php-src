--TEST--
Test that initonly properties can't be mutated by assignment operators
--FILE--
<?php

class Foo
{
    initonly public int $property1 = 1;
    initonly public string $property2 = "";
}

$foo = new Foo();

try {
    $foo->property1 = $foo->property1 + 1;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $foo->property1 = $foo->property1 - 1;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $foo->property1 = $foo->property1 * 1;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $foo->property1 = $foo->property1 / 1;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $foo->property1 = $foo->property1 ** 1;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $foo->property1 = $foo->property1 & 1;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $foo->property1 = $foo->property1 | 1;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $foo->property1 = $foo->property1 ^ 1;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $foo->property1 = $foo->property1 << 1;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $foo->property1 = $foo->property1 >> 1;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $foo->property2 = $foo->property2 . "foo";
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $foo->property2 = $foo->property2 ?? "foo";
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

var_dump($foo->property1);
var_dump($foo->property2);

?>
--EXPECT--
Cannot modify initonly property Foo::$property1 after initialization
Cannot modify initonly property Foo::$property1 after initialization
Cannot modify initonly property Foo::$property1 after initialization
Cannot modify initonly property Foo::$property1 after initialization
Cannot modify initonly property Foo::$property1 after initialization
Cannot modify initonly property Foo::$property1 after initialization
Cannot modify initonly property Foo::$property1 after initialization
Cannot modify initonly property Foo::$property1 after initialization
Cannot modify initonly property Foo::$property1 after initialization
Cannot modify initonly property Foo::$property1 after initialization
Cannot modify initonly property Foo::$property2 after initialization
Cannot modify initonly property Foo::$property2 after initialization
int(1)
string(0) ""
