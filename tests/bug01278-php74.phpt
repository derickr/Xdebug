--TEST--
Test for bug #1278: Xdebug with PHP 7 does not handle prefill-from-oparray for XDEBUG_CC_UNUSED (>= PHP 7.4)
--SKIPIF--
<?php
require 'tests/utils.inc';
check_reqs('PHP >= 7.4');
?>
--INI--
xdebug.default_enable=1
xdebug.auto_trace=0
xdebug.auto_profile=0
xdebug.profiler_enable=0
xdebug.overload_var_dump=0
--FILE--
<?php
$file = 'bug01278.inc';
$pathname = stream_resolve_include_path($file);
xdebug_start_code_coverage(XDEBUG_CC_UNUSED | XDEBUG_CC_DEAD_CODE);
require $pathname;
$coverage = xdebug_get_code_coverage();
xdebug_stop_code_coverage();
print_r($coverage[$pathname]);
?>
--EXPECTF--
Array
(
    [3] => -1
    [4] => -1
    [5] => -1
    [7] => -1
    [9] => 1
)
