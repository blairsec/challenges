<?php
include '../config.php';
$conn = mysqli_connect($servername, $username, $password, "sequel_fan_club");
$result = mysqli_query($conn, "SELECT * FROM users WHERE username='".$_POST["username"]."' AND password='".$_POST["password"]."';");
if (mysqli_num_rows($result) == 0) {
echo("<!doctype html>
<html>
	<head>
		<title>Sequel Fan Club</title>
	</head>
	<body style=\"background-image: url('images/background.jpeg');\">
		<h1 style=\"position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%); font-size: 5em; color: red; background-color: yellow; font-family: sans-serif;\">Sequel Fan Club</h1>
		<img src=\"images/porg.jpg\"></img>
		<img src=\"images/rey.png\"></img>
		<img src=\"images/kylo.jpg\"></img>
		<img src=\"images/finn.jpeg\"></img>
		<img src=\"images/luke.jpg\"></img>
		<form method=\"post\" style=\"text-align: center; width: 100%; position: absolute; bottom: -50%; transform: translateY(50%);\">");
if ($_POST["username"]) {
echo("			<h1 style='color: red; font-family: sans-serif'>Incorrect login</h1>");
}
echo("			<input style=\"font-size: 3em;\" type=\"text\" placeholder='Username' name=\"username\"><br><br>
			<input style=\"font-size: 3em;\" type=\"password\" placeholder='Password' name=\"password\"><br><br>
			<input style=\"font-size: 3em;\" type=\"submit\" value=\"Login\">
		</form>
	</body>
</html>");
} else {
echo("<!doctype html>
<html>
	<head>
		<title>Prequel Fan Club</title>
	</head>
	<body style=\"background-image: url('images/background2.jpeg');\">
		<h1 style=\"position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%); font-size: 4em; color: red; background-color: yellow; font-family: sans-serif;\">actf{sql_injection_more_like_prequel_injection}</h1>
		<img src=\"images/anakin.jpg\"></img>
		<img src=\"images/highground.jpg\"></img>
		<img src=\"images/sand.jpg\"></img>
		<img src=\"images/ironic.gif\"></img>
		<img style='width: 80%;' src=\"images/prequels.jpg\"></img>
	</body>
</html>");
}
