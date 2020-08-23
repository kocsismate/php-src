--TEST--
Test that initonly properties of scalar types can be accessed for reading
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

var_dump($foo->property1);
var_dump($foo->property2);
var_dump($foo->property3);

var_dump(Foo::$property4);
var_dump(Foo::$property5);
var_dump(Foo::$property6);

?>
--EXPECT--
bool(false)
int(2)
string(3) "Bar"
bool(false)
int(5)
string(3) "Baz"
