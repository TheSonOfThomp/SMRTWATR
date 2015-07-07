<script type="text/javascript">
var pw = '';
var PASSWORD = 'Syde361';
var authCount = 0;
function authenticate(){
	do {
		authCount++;
		pw = prompt('Please Enter the SMRT password', '');
		if (pw ==  PASSWORD){
			document.getElementById("form").submit();
		}
		else if (authCount <= 10) {
			pw = '';
			alert ('Incorrect password. Try Again');
		}
	} while ((pw != PASSWORD) && (authCount <= 10));
}

</script>

<!DOCTYPE html>
<html>
<head>
	<title></title>
</head>
<body onload="authenticate()" style="background: url('smrtwatr.png') no-repeat center center">
<form action="main.php" id="form"></form>
</body>
</html>