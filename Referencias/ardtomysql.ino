/*
  Web client
  
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
  
 */
 
#include <SPI.h>
#include <Ethernet.h>
 
// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server(62, 83, 21, 38); 
 
// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
 
//inicializamos la varible
int potenciometro = 0;
 
void setup() {
   
  potenciometro=50;
   
  // start the serial library:
  Serial.begin(9600);
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");
 
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.print("GET /ingenieros/comunicaciones/arduino/mysql.php?valor=");
    client.print(potenciometro);
    client.println(" HTTP/1.0");
     
    //client.println("GET /ingenieros/comunicaciones/arduino/ethernet.php HTTP/1.0");
    client.println();
  } 

  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  
}
 
void loop()
{
  // if there are incoming bytes available 
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
 
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
 
    // do nothing forevermore:
    for(;;)
      ;
  }
}