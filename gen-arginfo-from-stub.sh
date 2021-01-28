#!/bin/sh -e
${PHP:-php} "${PHP_SRC:-/usr/src/php}/build/gen_stub.php" proctitle.stub.php
