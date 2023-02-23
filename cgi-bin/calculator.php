<?php
  // Check if the form has been submitted
  if (isset($_POST['submit'])) {
    // Get the values from the form
    $num1 = $_POST['num1'];
    $num2 = $_POST['num2'];
    $operator = $_POST['operator'];

    // Perform the arithmetic operation based on the selected operator
    switch ($operator) {
      case 'add':
        $result = $num1 + $num2;
        break;
      case 'subtract':
        $result = $num1 - $num2;
        break;
      case 'multiply':
        $result = $num1 * $num2;
        break;
      case 'divide':
        $result = $num1 / $num2;
        break;
      default:
        $result = 'Invalid operator';
    }

    // Display the result
    echo "<!DOCTYPE html>";
    echo "<html>";
    echo "<body bgcolor=";
    echo '"';
    echo "#c0c0c0";
    echo '"';
    echo ">";
    echo "<center>";
    echo "<br><br><br><br><br>";
    echo "<h2>Result: $result</h2>";
    echo "</center>";
    echo "</body>";
    echo "</html>";
  }
?>
