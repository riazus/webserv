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
    echo "<h2>Result: $result</h2>";
  }
?>
