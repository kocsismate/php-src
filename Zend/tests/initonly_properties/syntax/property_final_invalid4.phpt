--TEST--
Test invalid syntax of initonly classes
--FILE--
<?php

initonly class Foo
{
}

?>
--EXPECTF--
Parse error: syntax error, unexpected token "initonly", expecting end of file in %s on line %d
