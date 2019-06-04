--TEST--
Test for bug #609: Xdebug and SOAP error handler conflicts
--SKIPIF--
<?php
require 'tests/utils.inc';
check_reqs('ext SOAP');
?>
--INI--
xdebug.default_enable=1
--FILE--
<?php
try {
	  $sc = new SoapClient("some-wrong.wsdl", array('exceptions' => true));
} catch (Exception $e) {
	  echo 'Error Caught :-)';
}
?>
--EXPECTF--
Error Caught :-)
