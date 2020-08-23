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

    public function &getProperty2(): int
    {
        return $this->property2;
    }
}

$foo = new Foo();

try {
    $x = &$foo->property1;
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $foo->getProperty2();
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

?>
--EXPECT--
Cannot acquire reference to initonly property Foo::$property1
Cannot acquire reference to initonly property Foo::$property2
