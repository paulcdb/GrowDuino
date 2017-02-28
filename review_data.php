<?php
    // Start MySQL Connection
    include('dbconnect.php');
?>

<html>
<head>
    <title>Arduino Temperature Log</title>
    <style type="text/css">
        .table_titles, .table_cells_odd, .table_cells_even {
                padding-right: 20px;
                padding-left: 20px;
                color: #000;
        }
        .table_titles {
            color: #FFF;
            background-color: #666;
        }
        .table_cells_odd {
            background-color: #CCC;
        }
        .table_cells_even {
            background-color: #FAFAFA;
        }
        table {
            border: 2px solid #333;
        }
        body { font-family: "Trebuchet MS", Arial; }
    </style>

<script type="text/javascript"> 
function openwindow(url){
	  NewWindow=window.open(url,'newWin','width=1500,height=750,left=20,top=20,toolbar=No,location=No,scrollbars=no,status=No,resizable=no,fullscreen=No');  NewWindow.focus(); void(0);  }
</script>
</head>

    <body align="center">
        <h1>Grow Controller - Statistics</h1>
<p><font size="2">Updated every 10 Minutes</font></p>
    <table align="center" width="80%" border="0" cellspacing="0" cellpadding="4">
      <tr>
            <td class="table_titles">Date and Time</td>
            <td class="table_titles">Controller</td>
            <td class="table_titles">PH Level</td>
            <td class="table_titles">Room Temp</td>
            <td class="table_titles">Humidity</td>
            <td class="table_titles">Water Temp</td>
            <td class="table_titles">Water Level</td>
          </tr>
<?php
    // Retrieve all records and display them
$sql = <<<SQL
    SELECT *
    FROM `Sensors`
    ORDER BY id DESC
    LIMIT 5
SQL;

    // Used for row color toggle
    $oddrow = true;

if(!$result = $db->query($sql)){
    die('There was an error running the query [' . $db->error . ']');
}

    // process every record
    while($row = $result->fetch_assoc())
    {
        if ($oddrow)
        {
            $css_class=' class="table_cells_odd"';
        }
        else
        {
            $css_class=' class="table_cells_even"';
        }

        $oddrow = !$oddrow;

        echo '<tr>';
        echo '   <td'.$css_class.'>'.$row["DateTime"].'</td>';
        echo '   <td'.$css_class.'>'.$row["Controller"].'</td>';
	if ( $row["PHLevel"] > 6.99 ) {
        echo '   <td'.$css_class.'><font color="RED">'.$row["PHLevel"].'</font></td>';
	} else if ( $row["PHLevel"] < 6 ) {
        echo '   <td'.$css_class.'><font color="RED">'.$row["PHLevel"].'</font></td>';
	} else {
        echo '   <td'.$css_class.'>'.$row["PHLevel"].'</td>';
	}
        echo '   <td'.$css_class.'>'.$row["RoomTemp"].'</td>';
        echo '   <td'.$css_class.'>'.$row["Humidity"].'</td>';
        echo '   <td'.$css_class.'>'.$row["WaterTemp"].'</td>';
	if ( $row["WaterLevel"] == 1 ) {
        echo '   <td'.$css_class.'><font color="GREEN">FULL</font></td>';
	} else if  ( $row["WaterLevel"] == 0 ) {
        echo '   <td'.$css_class.'><font color="RED">LOW</font></td>';
	}
        echo '</tr>';
    }
?>
    </table>
<p><font size="2">
<?php echo 'Last ' . $result->num_rows . ' results.</font><br>'; ?>

<font size="1"><?php  echo 'Server Time: ' ?>
<?php  echo date("h:i:sa") ?></font>
</p>
<hr>
<h1>LED Status - Soon&trade;</h1>
<table align="center" width="80%"border="0" cellspacing="0" cellpadding="4">
    <tr align="center">
    <td class="table_titles">Seed LEDs</td>
    <td class="table_titles">Seed Pump</td>
    <td class="table_titles">Tower LEDs</td>
    <td class="table_titles">Tower Pump</td>
    </tr>
    <tr align="center">
    <td class="table_cells_odd"> - </td>
    <td class="table_cells_odd"> - </td>
    <td class="table_cells_odd"> - </td>
    <td class="table_cells_odd"> - </td>
    </tr>
    <tr align="center">
    <td class="table_cells_even"> ON / OFF </td>
    <td class="table_cells_even"> ON / OFF</td>
    <td class="table_cells_even"> ON / OFF</td>
    <td class="table_cells_even"> ON / OFF </td>
    </tr>
    </table>
<p><a href="control.php?running=0">Pause</a>/<a href="control.php?running=1">Resume</a></p>
<p><font size="2">Status: Paused / Running</font></p>
<hr>
<h1>Graphs</h1>
<table align="center" width="80%"border="0" cellspacing="0" cellpadding="4">
    <tr align="center">
    <td class="table_titles" colspan="2">Last 24 Hours</td>
  </tr>
    <tr align="center">
    <td class="table_cells_odd"><a href="ph.php?x=1500&y=750" onclick="openwindow(this.href); return false;"><img src="ph.php" /></a></td>
    <td class="table_cells_odd"><a href="roomtemp.php?x=1500&y=750" onclick="openwindow(this.href); return false;"><img src="roomtemp.php" /></a></td>
  </tr>
  <tr align="center">
    <td class="table_cells_odd"><a href="humidity.php?x=1500&y=750" onclick="openwindow(this.href); return false;"><img src="humidity.php" /></a></td>
    <td class="table_cells_odd"><a href="watertemp.php?x=1500&y=750" onclick="openwindow(this.href); return false;"><img src="watertemp.php" /></a></td>
    </tr>
    </table>
    <p><a href="review_data.php?graphs=daily">Daily</a> | <a href="review_data.php?graphs=weekly">Weekly</a> | <a href="review_data.php?graphs=monthly">Monthly</a> | <a href="review_data.php?graphs=yearly">Yearly</a>
    </body>
</html>
<?php
$db->close();
?>
