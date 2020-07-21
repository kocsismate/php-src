--TEST--
Test that there is no arginfo/zpp mismatch
--FILE--
<?php

function test($function) {
    if ($function === 'readline' /* expects input */
     || $function === 'zend_create_unterminated_string' /* violates engine invariant */
     || $function === 'zend_test_array_return' /* intentionally violates return type */
     || (is_string($function) && str_starts_with($function, 'ob_')) /* messes with output */
    ) {
        return;
    }
    if (($function[0] instanceof DOMText || $function[0] instanceof DOMComment)
            && $function[1] === 'before') {
        /* TODO: Bug #79968 */
        return;
    }
    if ($function[0] instanceof SoapServer) {
        /* TODO: Uses fatal errors */
        return;
    }
    if ($function[0] instanceof ZipArchive) {
        /* TODO: There's a PR for this. */
        return;
    }

    ob_start();
    if (is_string($function)) {
        echo "Testing $function\n";
    } else {
        echo "Testing " . get_class($function[0]) . "::$function[1]\n";
    }
    try {
        @$function();
    } catch (Throwable) {
    }
    try {
        @$function(null);
    } catch (Throwable) {
    }
    try {
        @$function(null, null);
    } catch (Throwable) {
    }
    try {
        @$function(null, null, null, null, null, null, null, null);
    } catch (Throwable) {
    }
    ob_end_clean();
}

foreach (get_defined_functions()["internal"] as $function) {
    test($function);
}

foreach (get_declared_classes() as $class) {
    try {
        $rc = new ReflectionClass($class);
        $obj = $rc->newInstanceWithoutConstructor();
    } catch (Throwable) {
        continue;
    }

    foreach (get_class_methods($class) as $method) {
        test([$obj, $method]);
    }
}

// var_dump() and debug_zval_dump() print all arguments
?>
===DONE===
--EXPECT--
Segmentation fault (core dumped)

Termsig=11
