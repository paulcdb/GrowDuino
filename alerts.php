<?php
    // Start MySQL Connection
    include('dbconnect.php');

    // Retrieve all records and display them
$sql = <<<SQL
    SELECT *
    FROM `Sensors`
    ORDER BY id DESC
    LIMIT 1
SQL;

if(!$result = $db->query($sql)){
    die('There was an error running the query [' . $db->error . ']');
}

    // process every record
    while($row = $result->fetch_assoc())
    {
    if ( $row["WaterLevel"] == 0 ) {
	curl_setopt_array($ch = curl_init(), array(
  	CURLOPT_URL => "https://api.pushover.net/1/messages.json",
  	CURLOPT_POSTFIELDS => array(
    	"token" => "YOUR TOKEN ID",
    	"user" => "YOUR USER ID",
    	"message" => "Alert - Water Level Low!",
	),
	CURLOPT_SAFE_UPLOAD => true,
	));
	curl_exec($ch);
	curl_close($ch);
}
else if ( $row["WaterLevel"] == 1 ) {
	echo 'Water Level - Full';
	}
}
?>
