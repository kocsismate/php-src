--TEST--
Test that initonly properties can't be accessed by reference
--FILE--
<?php

class Foo
{
    initonly public int $property1;
    initonly public int $property2 = 2;

    public function __construct()
    {
        $this->property1 = 1;
    }
}

$foo = new Foo();

try {
    foreach ($foo as &$property) {}
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

?>
--EXPECT--
Cannot acquire reference to initonly property Foo::$property1
