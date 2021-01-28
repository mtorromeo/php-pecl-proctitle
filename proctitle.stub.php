<?php

/** @generate-function-entries */

function setproctitle(string $title): void {}

#ifdef HAVE_PRCTL
function setthreadtitle(string $title): bool {}
#endif
