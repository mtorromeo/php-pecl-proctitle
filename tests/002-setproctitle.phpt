--TEST--
Check that setproctitle runs correctly
--FILE--
<?php
setproctitle('test');
echo "setproctitle function did not crash";
--EXPECT--
setproctitle function did not crash
