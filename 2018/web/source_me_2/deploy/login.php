<?php

if (!isset($_GET["pass"]) || !isset($_GET["user"])){
    header("/");
}

elseif ($_GET["pass"] == "password1234" && $_GET["user"] == "admin"){
    echo "Welcome, admin. Here is your flag: actf{md5_hash_browns_and_pasta_sauce}";
}
else {
    echo "<h3>Failed login.</h3>";
}
?>

