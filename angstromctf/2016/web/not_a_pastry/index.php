<?php
class Session {
    public $start_time;
    public $admin;
    
    public function __construct() {
        $this->start_time = time();
        $this->admin = false;
    }
}

$loggedin = false;
$admin = false;

if (isset($_COOKIE["not_a_pastry"])) {
    $cookie = base64_decode($_COOKIE["not_a_pastry"]);
    
    if ($cookie) {
        $session = unserialize($cookie);
        
        if (isset($session->start_time) and isset($session->admin)) {
            $loggedin = true;
            
            if ($session->admin) {
                $admin = true;
            }
        }
    }
}

if (!$loggedin) {
    $session = new Session();
    setcookie("not_a_pastry", base64_encode(serialize($session)));
}
?>

<html>
    <head>
        <title>Not A Pastry</title>
    </head>
    <body>
<?php
if ($admin) {
    echo "<h1>Congratulations!</h1>  Your flag is: good_ole_fashioned_homemade_cookies";
} else {
    echo "<h1>You're not admin!  Sorry :(</h1>";
    echo "<p>Your session started at: " . date(DATE_ISO8601, $session->start_time) . "</p>";
    echo '<iframe id="ytplayer" type="text/html" width="640" height="390" src="http://www.youtube.com/embed/dQw4w9WgXcQ?autoplay=1&origin=http://example.com"
frameborder="0"/><br/>';
}
?>
</html>