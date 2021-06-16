<?php
if (!isset($_COOKIE["_id"])) {
	$h = hash('md5', strval(time()));
	setcookie("_id", $_COOKIE["_id"] = $h, time()+3600);
	echo time();
}
?>