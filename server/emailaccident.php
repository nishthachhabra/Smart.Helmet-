<?php

$email ="vaibhav.grover18@gmail.com";
$content ="Help Me!! I met with an accident. Location: https://maps.app.goo.gl/foEltd8xcihHF2ME2";

$sender ="vaibhav.grover18@gmail.com";

$subject ="Help Urgently!!";

$headers = "From: $name "."<".$sender.">\r\n";
//add boundary string and mime type specification
//$headers .= 'MIME-Version: 1.0' . "\r\n";
//$headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";

//send the email
mail($email, $subject, $content, $headers );

?>