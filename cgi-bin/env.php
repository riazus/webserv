<?php


echo '<html>';

# echo '<head>'
# echo '</head>'
echo '<body bgcolor="#c0c0c0"';
echo '<center>';

//Call getenv() function without argument
$env_array =getenv();
// echo "<h3>The list of environment variables with values are :</h3>";

//Print all environment variable names with values
foreach ($env_array as $key=>$value)
{
    echo "$key => $value \r\n\n";
}
echo '</center>';

echo '</body>';
echo '</html>';

?>
