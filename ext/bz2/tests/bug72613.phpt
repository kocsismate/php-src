--TEST--
Bug #72613 (Inadequate error handling in bzread())
--SKIPIF--
<?php if (!extension_loaded("bz2")) print "skip"; ?>
--FILE--
<?php
$fp = bzopen(__DIR__ . '/72613.bz2', 'r');
if ($fp === false) {
    exit("ERROR: bzopen()");
}
$data = "";
while (!feof($fp)) {
    $res = bzread($fp);
    if ($res === false) {
        exit("ERROR: bzread()");
    }
    $data .= $res;
}
bzclose($fp);
?>
DONE

?>

?>
--EXPECT--
ERROR: bzread()
