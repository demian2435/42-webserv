<?php
if (isset($_POST["name"])) {
    // Set the cookie
    setcookie("name", $_COOKIE['name'] = $_POST["name"], time()+3600);  /* expire in 1 hour */
}
?>

<html>
<head>
<style>
h1 {text-align: center;}
form {text-align: center;}
.container {
  display: grid;
  place-items: center;
  height: 100%;
}
</style>
</head>
<body>
<div class="container">
	<div>
  <?php
if(!isset($_COOKIE["name"])) {
  echo "<h1>Ciao, straniero. Qual e' il tuo nome?</h1>";
  echo '<form action="cookie3.php" method="get">
	Nome: <input type="text" name="name">
	<input type="submit">
	</form>';
} else {
  echo "<h1>Ciao " . $_COOKIE["name"] . ", bentornato.</h1>";
}
echo "\n ";
echo $_POST['ciao'];
?>
</div>
</div>

</body>
</html>