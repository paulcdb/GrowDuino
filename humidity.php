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
    SELECT DateTime, ROUND(AVG(Humidity),2) AS Humidity
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
      $Humidity=$row["Humidity"];
      $DateTime=$row["DateTime"];
      //add to data areray
      $dataArray[$DateTime]=$Humidity;
  }

//configure graph
$graph->addData($dataArray);
$graph->setTitle("Humidity");
$graph->setGradient("lime", "green");
$graph->setBarOutlineColor("black");
$graph->setRange(30,70);
$graph->setBars(false);
$graph->setLine(true);
$graph->setDataPoints(true);
$graph->setDataValues(true);
$graph->setGoalLine('50');
$graph->createGraph();

$db->close();
?>
