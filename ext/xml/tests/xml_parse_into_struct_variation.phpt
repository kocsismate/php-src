--TEST--
Test xml_parse_into_struct() function : variation
--EXTENSIONS--
xml
--FILE--
<?php
echo "*** Testing xml_parse_into_struct() : variation ***\n";

$simple = "<main><para><note>simple note</note></para><para><note>simple note</note></para></main>";
$p = xml_parser_create();
xml_parse_into_struct($p, $simple, $vals, $index);
xml_parser_free($p);
echo "Index array\n";
print_r($index);
echo "\nVals array\n";
print_r($vals);


echo "Done";
?>
--EXPECT--
*** Testing xml_parse_into_struct() : variation ***
Index array
Array
(
    [MAIN] => Array
        (
            [0] => 0
            [1] => 7
        )

    [PARA] => Array
        (
            [0] => 1
            [1] => 3
            [2] => 4
            [3] => 6
        )

    [NOTE] => Array
        (
            [0] => 2
            [1] => 5
        )

)

Vals array
Array
(
    [0] => Array
        (
            [tag] => MAIN
            [type] => open
            [level] => 1
        )

    [1] => Array
        (
            [tag] => PARA
            [type] => open
            [level] => 2
        )

    [2] => Array
        (
            [tag] => NOTE
            [type] => complete
            [level] => 3
            [value] => simple note
        )

    [3] => Array
        (
            [tag] => PARA
            [type] => close
            [level] => 2
        )

    [4] => Array
        (
            [tag] => PARA
            [type] => open
            [level] => 2
        )

    [5] => Array
        (
            [tag] => NOTE
            [type] => complete
            [level] => 3
            [value] => simple note
        )

    [6] => Array
        (
            [tag] => PARA
            [type] => close
            [level] => 2
        )

    [7] => Array
        (
            [tag] => MAIN
            [type] => close
            [level] => 1
        )

)
Done
