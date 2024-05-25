<?php


include("hash.php");

$auth_token = genToken($_POST['username'], $_POST['password']);

setcookie("uname", $_POST['username'], time() + (86400 * 30), "/");
setcookie("PIN", $_POST['password'], time() + (86400 * 30), "/");

setcookie("auth_token", $auth_token);

include("basic.html");

echo "You are now signed in! Click <a href='/'>here</a> to return to the home page.";


echo "<br>Admin Credentials: " . ($auth_token == genToken("admin_802947291", 7843) ? "yes" : "no") . '<br>';

?>
