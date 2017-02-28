<?php
    // Start MySQL Connection
    include('dbconnect.php');

$Controller = $_GET['controller'];
$pHLevel = $_GET['ph'];
$RoomTemp = $_GET['roomtemp'];
$WaterTemp = $_GET['watertemp'];
$Humidity = $_GET['humidity'];
$WaterLevel = $_GET['waterlevel'];

$sql = "INSERT INTO Sensors (Controller, PHLevel, RoomTemp, WaterTemp, Humidity, WaterLevel) VALUES ('$Controller', '$pHLevel', '$RoomTemp', '$WaterTemp', '$Humidity', '$WaterLevel')";

if ($db->query($sql) === TRUE) {
    echo "New record created successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

$conn->close();
?>

