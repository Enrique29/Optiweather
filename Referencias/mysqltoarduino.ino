#include <SPI.h>
#include <Ethernet.h>
 
//Asignamos una dirección MAC
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
 
//Asignar dirección IP
IPAddress ip(192,168,1,173);
 
//Inicializar instancia de la libreria ethernet
EthernetClient client;
 
//Dirección IP del servidor con la página PHP
char server[] = "192.168.1.251"; 
 
//Variables que neceita para funcionar
String codigo;        //Aquí se almacena la respuesta del servidor
String nombre;        //Aquí se almacena el nombre que recuperamos de MySQL
boolean fin = false;  
boolean pregunta = true;
 
 
void setup() {
  // Iniciar puerto serie
  Serial.begin(9600);
  // Dar un respiro a Arduino
  delay(1000);
  //Iniciar la coneción de red
  Ethernet.begin(mac, ip);
  // Imprimir la dirección IP
  Serial.print("IP: ");
  Serial.println(Ethernet.localIP());
}
 
void loop() {
    //Comprobamos si tenemos datos en el puerto serie
    if (pregunta == true) Serial.print("Escribe el identificador de la persona: ");
    pregunta = false;
    if (Serial.available()>0){
        //leemos el identificador
        int identificador=Serial.read()-48;
        //llamamos a la función quenos permitira comunicarnos con el servidor
        httpRequest(identificador);
        pregunta = true;
  }
       
 
}
 


 
// Con esta función hacemos la conecion con el servidor
int httpRequest(int identificador) {
 
  // Comprobar si hay conexión
  if (client.connect(server, 80)) {
    Serial.println("nConectado");
    // Enviar la petición HTTP
    //Dirección del archivo php dentro del servidor
    client.print("GET /ingenieros/comunicaciones/valormysql.php?id=");
    //Mandamos la variable junto a la línea de GET
    client.print(identificador);
    client.println(" HTTP/1.0");
    //IP del servidor
    client.println("Host: 192.168.1.251");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
  }
  else {
    // Si no conseguimos conectarnos
    Serial.println("Conexión fallida");
    Serial.println("Desconectando");
    client.stop();
  }
 
  delay(500);
  //Comprobamos si tenemos respuesta del servidor y la 
  //almacenamos en el string ----> codigo.
  while (client.available()) {
    char c = client.read();
    codigo += c;
    //Habilitamos la comprobación del código recibido
    fin = true;
 
  }
  //Si está habilitada la comprobación del código entramos en el IF
  if (fin)  {
   // Serial.println(codigo);
   //Analizamos la longitud del código recibido
    int longitud = codigo.length();
    //Buscamos en que posición del string se encuentra nuestra variable
    int posicion = codigo.indexOf("valor=");
    //Borramos lo que haya almacenado en el string nombre
    nombre = "";
 
    //Analizamos el código obtenido y almacenamos el nombre en el string nombre
    for (int i = posicion + 6; i < longitud; i ++){
      if (codigo[i] == ';') i = longitud;
      else nombre += codigo[i];
    }
    //Deshabilitamos el análisis del código
    fin = false;
    //Imprimir el nombre obtenido
    Serial.println("Valor de la variable nombre: " + nombre);
    //Cerrar conexión
    Serial.println("Desconectarn");
    client.stop();
  }
  //Borrar código y salir de la función//Dirección IP del servidor
  codigo="";
  return 1;
}