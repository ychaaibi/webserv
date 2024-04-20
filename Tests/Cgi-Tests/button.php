<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Change Button Color</title>
    <style>
        /* Define styles for the button */
        button {
            padding: 10px;
            font-size: 16px;
            cursor: pointer;
        }
    </style>
</head>
<body>

<?php
// Handle button click logic
if (isset($_POST['color'])) {
    // Get the selected color from the form
    $selectedColor = $_POST['color'];
    echo '<style>button { background-color: ' . $selectedColor . '; }</style>';
}
?>

<!-- HTML form with a button -->
<form method="post" action="">
    <label for="colorPicker">Select a color:</label>
    <input type="color" id="colorPicker" name="color" value="#ff0000">
    <button type="submit">Change Color</button>
</form>

</body>
</html>
