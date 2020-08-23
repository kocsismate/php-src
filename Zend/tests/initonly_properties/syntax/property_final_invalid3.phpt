--TEST--
Test invalid syntax of initonly methods
--FILE--
<?php

class Foo
{
    initonly function bar() {}
}

?>
--EXPECTF--
Fatal error: Cannot declare function Foo::$bar initonly, the initonly modifier is allowed only for properties in %s on line %d
