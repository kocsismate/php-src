--TEST--
Test that initonly properties can't be incremented/decremented
--FILE--
<?php

class Foo
{
    initonly public int $property1;
    initonly public int $property2;
}

$foo = new Foo();

try {
    ++$foo->property1;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $foo->property1++;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

$foo->property1 = 1;
$foo->property2 = 2;

try {
    ++$foo->property1;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    ++$foo->property2;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $foo->property1++;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $foo->property2++;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

var_dump($foo->property1);
var_dump($foo->property2);

try {
    --$foo->property1;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    --$foo->property2;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $foo->property1--;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $foo->property2--;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

var_dump($foo->property1);
var_dump($foo->property2);

?>
--EXPECT--
Typed property Foo::$property1 must not be accessed before initialization
Typed property Foo::$property1 must not be accessed before initialization
Cannot increment initonly property Foo::$property1
Cannot increment initonly property Foo::$property2
Cannot increment initonly property Foo::$property1
Cannot increment initonly property Foo::$property2
int(1)
int(2)
Cannot decrement initonly property Foo::$property1
Cannot decrement initonly property Foo::$property2
Cannot decrement initonly property Foo::$property1
Cannot decrement initonly property Foo::$property2
int(1)
int(2)
