<?php

include("hash.php");
define('RESTRICTED',1);


include("basic.html");

if (!isset($_COOKIE['auth_token']) || !isset($_COOKIE['uname']) || !isset($_COOKIE['PIN'])){

    echo "You need to have generated a user auth token before accessing this page.<br>";
    echo "Click <a href='/login.html'>here</a> to do so";
}



elseif (!validate($_COOKIE['uname'],$_COOKIE['PIN'],$_COOKIE['auth_token'])){
    echo "Your authentication token is invalid, please re-login to create a valid one.<br>";
    echo "Click <a href='/login.html'>here</a> to do so";
}

elseif (!isset($_GET['story']) || strpos($_GET['loc'], 'public') === false){


    if (isset($_GET['loc'])) {
        if (strpos($_GET['loc'], 'public') === false) {
            echo "<h3>Access Denied: Location parameter must contain public in its path!</h3>";
        }
    }
    #Only admin_802947291 has access with PIN 7843

    $uname = $_COOKIE['uname'];
    $pin = $_COOKIE['PIN'];
    $auth_token = $_COOKIE['auth_token'];

    echo "<h3><b>Your Credentials:</b><br></h3>";
    echo "<h4>User: $uname<br></h4>";
    echo "<h4>PIN: $pin<br></h4>";
    echo "<h4>Token: $auth_token<br></h4>";
    echo "Admin Credentials: " . ($auth_token == genToken("admin_802947291", 7843) ? "yes" : "no") . '<br>';
    echo "Click <a href='/login.html'>here</a> to log in with new credentials.<br>";




    include("access_form.html");
}

else {

    $loc = str_replace(chr(0), '', $_GET['loc']);
    if (strlen($loc) > 300){
        $loc = substr($loc, 0, 300);
    }

    $access_token = genToken("admin_802947291", 7843);
    //9cd6ef9734ae48d1bbdf0a65511e4700
    $hash_name = md5($_GET['story'] . $access_token);

    echo "Requested Story: " . $_GET['story'];
    echo "<br><br>";
    include("./stories/".$loc."/".$hash_name.".story");


}

echo "</body></html>";

