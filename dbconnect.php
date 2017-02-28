<?php
$MyHostname = "MySQL Name/IP";    // this is usually "localhost" unless your database resides on a different server
$MyUsername = "MySql Username";  // enter your username for mysql
$MyPassword = "MySQL Password";  // enter your password for mysql
$dbName     = "Database Name";  // eter your database table name from mysql

$db = mysqli_connect("$MyHostname","$MyUsername","$MyPassword","$dbName");

// Check connection
if (mysqli_connect_errno())
  {
  echo "Failed to connect to MySQL: " . mysqli_connect_error();
  }
?>
