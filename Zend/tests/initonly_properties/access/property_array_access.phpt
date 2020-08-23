--TEST--
Test that ArrayAccess works for initonly properties as expected
--FILE--
<?php

class Foo implements ArrayAccess
{
    initonly private string $property1;
    initonly private string $property2 = "";

    function offsetExists($offset)
    {
        return isset($this->$offset);
    }

    function offsetGet($offset)
    {
        return $this->$offset ?? null;
    }

    function offsetSet($offset, $value)
    {
        $this->$offset = $value;
    }

    function offsetUnset($offset){
        unset($this->$offset);
    }
}

$foo = new Foo();

$foo["property1"] = "foo";

try {
    $foo["property1"] = "foo";
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

try {
    $foo["property2"] = "foo";
} catch (Error $exception) {
    echo $exception->getMessage() . "\n";
}

$foo["property3"] = "foo";

?>
--EXPECT--
Cannot modify initonly property Foo::$property1 after initialization
Cannot modify initonly property Foo::$property2 after initialization
