--TEST--
Cannot take property of a string
--FILE--
<?php

"foo"->bar;

?>
--EXPECTF--
Warning: Access to property 'bar' on string in %s on line %d
