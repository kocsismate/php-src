--TEST--
Bug #46711 (lost memory when foreach is used for values passed to curl_setopt())
--SKIPIF--
<?php
if (!extension_loaded("curl")) {
	exit("skip curl extension not loaded");
}
?>
--FILE--
<?php
$ch = curl_init();

$opt = array(
    CURLOPT_AUTOREFERER  => true,
    CURLOPT_BINARYTRANSFER => true
);

curl_setopt($ch, CURLOPT_AUTOREFERER, true);

foreach ($opt as $option => $value) {
    curl_setopt($ch, $option, $value);
}

var_dump($opt); // with this bug, $opt[58] becomes NULL

?>
--EXPECT--
array(2) {
  [58]=>
  bool(true)
  [19914]=>
  bool(true)
}
