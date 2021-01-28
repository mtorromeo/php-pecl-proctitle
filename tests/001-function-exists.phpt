--TEST--
Check for setproctitle function presence
--SKIPIF--
<?php if (!function_exists("setproctitle")) print "skip"; ?>
--FILE--
<?php
echo "setproctitle function is available";
--EXPECT--
setproctitle function is available
