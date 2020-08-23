--TEST--
Test invalid syntax of initonly constants
--FILE--
<?php

class Foo
{
    initonly const CONST1 = 1;
}

?>
--EXPECTF--
Fatal error: Cannot use 'initonly' as constant modifier in %s on line %d
