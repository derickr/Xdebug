--TEST--
Test for bug #931: Crash with exception in shut-down stage
--SKIPIF--
<?php
require 'tests/utils.inc';
check_reqs('ext session');
?>
--FILE--
<?php
session_start();
$_SESSION['test'] = function() { };
echo "DONE";
?>
--EXPECTF--
DONE
Fatal error: Uncaught%sSerialization of 'Closure' is not allowed%sin [no active file]:0
Stack trace:
#0 {main}
  thrown in [no active file] on line 0
