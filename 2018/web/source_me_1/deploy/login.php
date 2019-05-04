<?php

if (!isset($_GET["pass"]) || !isset($_GET["user"])){
    header("/");
}

elseif ($_GET["pass"] == "f7s0jkl" && $_GET["user"] == "admin"){
    echo "Welcome, admin. Here is your flag: actf{source_aint_secure}";
}
else {
    echo "<h3>Failed login.</h3>";
}
?>
