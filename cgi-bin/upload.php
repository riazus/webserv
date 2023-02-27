<?php
echo '<head>';
echo '</head>';
echo '<body bgcolor="#c0c0c0" class="text"';
echo '<br>';
if($_SERVER['REQUEST_METHOD'] == 'POST')
{
	echo 'Request was a POST';
}
echo '<br>';
$uploaddir = __DIR__ . '/tmp/';
$uploadfile = $uploaddir . basename($_FILES['userfile']['name']);
$tmp_name = $_FILES['userfile']['tmp_name'];
echo '<pre>';
if (move_uploaded_file($tmp_name, $uploadfile)) {
	echo "FILE UPLOAD SUCCESFULY\n\n";
} else {
	echo "Oops, try again....\n";
}
echo 'Debug infos :';
echo '<br>';
echo '<div>';

print_r($_FILES);

echo '<a href="';
$pos = strpos($uploadfile, "/cgi-bin");
$readystr = substr($uploadfile, $pos);
echo $readystr;
echo '" download>Download file back</a>';
echo '</div>';


echo '</body>';
?>
