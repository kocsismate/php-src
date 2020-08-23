--TEST--
Test invalid syntax of initonly properties
--FILE--
<?php

class Foo
{
    initonly initonly $property1;
}

?>
--EXPECTF--
Fatal error: Multiple initonly modifiers are not allowed in %s on line %d
