<?php
	// Authorisation details.
	$username = "vaibhav.grover18@gmail.com";
	$hash = "b1b27fa7f7247aa0832898282c0909cc8f9fd8ad15a811323cf9f9fb7b88478a";

	// Config variables. Consult http://api.textlocal.in/docs for more info.
	$test = "0";

	// Data for text message. This is the text message data.
	$sender = "TXTLCL"; // This is who the message appears to be from.
	$numbers = "919999407638"; // A single number or a comma-seperated list of numbers
	$message = "Sorry to inform you!! I am drunk. Location: https://maps.app.goo.gl/foEltd8xcihHF2ME2";
	// 612 chars or less
	// A single number or a comma-seperated list of numbers
	$message = urlencode($message);
	$data = "username=".$username."&hash=".$hash."&message=".$message."&sender=".$sender."&numbers=".$numbers."&test=".$test;
	$ch = curl_init('http://api.textlocal.in/send/?');
	curl_setopt($ch, CURLOPT_POST, true);
	curl_setopt($ch, CURLOPT_POSTFIELDS, $data);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	$result = curl_exec($ch); // This is the result from the API
	curl_close($ch);
?>