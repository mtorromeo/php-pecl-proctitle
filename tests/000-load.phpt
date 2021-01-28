--TEST--
Check for php-proctitle presence
--SKIPIF--
<?php if (!extension_loaded("proctitle")) print "skip"; ?>
--FILE--
<?php
echo "proctitle extension is available";
--EXPECT--
proctitle extension is available
