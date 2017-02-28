<?php
if (empty($_GET)) {
    $width = 600;
    $height = 250;
} else {
    $width = $_GET['x'];
    $height = $_GET['y'];
}

    // Start MySQL Connection
    include('dbconnect.php');
    include("phpgraphlib.php");
    $graph=new PHPGraphLib($width,$height);
    $dataArray=array();

    // Retrieve all records and display them
$sql = <<<SQL
    SELECT DateTime, ROUND(AVG(RoomTemp),2) AS RoomTemp
    FROM `Sensors`
    WHERE DateTime > DATE_SUB(NOW(), INTERVAL 24 HOUR)
    GROUP BY HOUR(DateTime)
    ORDER BY DateTime ASC
SQL;

    // Used for row color toggle
    $oddrow = true;

if(!$result = $db->query($sql)){
    die('There was an error running the query [' . $db->error . ']');
}

    // process every record
    while($row = $result->fetch_assoc())
    {
      $RoomTemp=$row["RoomTemp"];
      $DateTime=$row["DateTime"];
      //add to data areray
      $dataArray[$DateTime]=$RoomTemp;
  }

//configure graph
$graph->addData($dataArray);
$graph->setTitle("Room Temp");
$graph->setGradient("lime", "green");
$graph->setBarOutlineColor("black");
$graph->setRange(0,30);
$graph->setBars(false);
$graph->setLine(true);
$graph->setDataPoints(true);
$graph->setDataValues(true);
$graph->setDataFormat("degrees");
$graph->setGoalLine('15');
$graph->createGraph();

$db->close();
?>
