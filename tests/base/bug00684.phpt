--TEST--
Test for bug #684: xdebug_var_dump - IE does not support &amp;
--INI--
html_errors=1
xdebug.mode=display
xdebug.overload_var_dump=2
xdebug.file_link_format=
xdebug.filename_format=
--FILE--
<?php
xdebug_var_dump('Testing isn\'t fun');
?>
--EXPECTF--
<pre class='xdebug-var-dump' dir='ltr'>
<small>%sbug00684.php:2:</small><small>string</small> <font color='#cc0000'>'Testing isn&#39;t fun'</font> <i>(length=17)</i>
</pre>
