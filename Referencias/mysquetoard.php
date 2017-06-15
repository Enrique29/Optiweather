<?php
    $conexion = mysql_connect("localhost", "TuUsuario", "TuContraseña");
    mysql_select_db("proyectos",$conexion);
                     
    mysql_query("INSERT INTO `tutorial-ethernet`(`valor`) VALUES ('" . $_GET['valor'] . "')", $conexion);
 
?>


<?php
    $conexion = mysql_connect("localhost", "******", "******");
    mysql_select_db("proyectos",$conexion);
                     
    $resultado = mysql_query("SELECT nombre FROM accesorfid WHERE id='" . $_GET['id']. "'", $conexion);
 
    $nombre = mysql_result($resultado, 0);
    echo "valor=" . $nombre . ";";
 
 
?>