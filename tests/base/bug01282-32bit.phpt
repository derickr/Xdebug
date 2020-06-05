--TEST--
Test for bug #1282: var_dump() of integers > 32 bit is broken on Windows
--SKIPIF--
<?php
require __DIR__ . '/../utils.inc';
check_reqs('32bit');
?>
--INI--
xdebug.mode=display
xdebug.overload_var_dump=1
--FILE--
<?php
ini_set( 'html_errors', 0 );
var_dump(PHP_INT_MAX);

ini_set( 'html_errors', 1 );
var_dump(PHP_INT_MAX);
?>
--EXPECTF--
int(2147483647)
<pre class='xdebug-var-dump' dir='ltr'><small>int</small> <font color='#4e9a06'>2147483647</font>
</pre>
