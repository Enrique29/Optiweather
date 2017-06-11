#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <DHT.h>
#include <DHT_U.h>
#include <EEPROM.h>
#include <String.h>
#include<ESP8266mDNS.h>
#define DHTTYPE DHT11
#define DHTPIN 1//GPIO1-Tx
DHT dht(DHTPIN, DHTTYPE);


#define OLED_RESET LED_BUILTIN
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

int h = 0;
int t = 0;
//byte hum = 0;
//byte tempe = 0;

byte letra = 0;
//byte valorByte = 0;

bool toggle = false;
bool medidor = false;
bool datosrec = false;
bool verificar = false;
boolean    Lflag       = true;


unsigned int localPort = 2390;      // local port to listen for UDP packets

IPAddress timeServerIP; // time.nist.gov NTP server address

const char* ntpServerName = "time.nist.gov"; //time.nist.gov

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

long horaActual = 0;
long minutoActual = 0;
long segundoActual = 0;

String strHora = "";
String strHoraActual = "";

byte lecturaTemp[127] = {0};
byte lecturaHum[127] = {0};

int tempOhum = 0;

const int ancho = 1 ;

int k = 0;

////////////////////////variables host //////////////////////////

char ssid[] = "";  //  your network SSID (name)
char pass[] = "";       // your network password
char gateway[] ="";
char ssidip[] ="";

boolean modoap = true;

String html1 ="<!DOCTYPE html> <html lang=\"es-Es\"> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=ISO-8859-1\"/> <head> <title>Optiweather</title> <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"> <meta content=\"width=device-width, initial-scale=1\" name=\"viewport\" /> <style> html,body {padding:0px;maring:0px;text-align: left; width:100%;height:100%;font-size:100%;font-family:arial;LINE-HEIGHT:30px;} input[type=\"text\"],select {width:100%;} input[type=\"submit\"] { padding:10px ;width:40%;font-size:120%} div {width:100%} form {padding:20px} </style> </head> <body> <img src=\"data:image/png;base64,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\" width=\"100%\">";
  
String getssid = "";
String getpass = "";
String getmethod  = "";
String getssidip = "";
String getidentikey = "";
String getgateway="";
String getconfigurado="";
String strIP="";
String ssid_leido="";
String pass_leido="";
String gateway_leido="";
String ssidip_leido="";

int cuenta=0;
int ssid_tamano=0;
int pass_tamano=0;

ESP8266WebServer server(80);
//////////////////////////TIMER///////////////

int segundos=0;
int minutos=0;
bool conectado= false;

/////////////////////////////////////////////////////// COMPROBAR LA RESOLUCIÓN DE LA PANTALLA OLED
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

/////////////////////  TEMPERATURA HUMEDAD    /////////////////////


void temperaturaHumedadNoHora() {
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("OPTIWEATHER");
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  dht.begin();
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(t)) {
    t = 100; 
  }
  if (isnan(h)) {
    h = 100;
  }
  desplazaYgraba(t, h);

  display.setTextSize(3);
  display.setCursor(20, 17);

  display.print(lecturaTemp[127]);
  display.setTextSize(1);
  display.print("c");
  display.print(" ");
  display.setTextSize(3);
  display.print(lecturaHum[127]);
  display.setTextSize(1);
  display.println("%");
  display.setCursor(7, 55);
  display.setTextSize(1);
  display.print(strHoraActual);
  display.display();
  delay(1 * 5 * 1000); /////////////////////// m, s, ms

}

void desplazaYgraba (int valort, int valorh) {
  for (k = 0; k < 127; k++) {
    lecturaTemp[k] = lecturaTemp[k + 1];
    lecturaHum[k] = lecturaHum[k + 1];
  }
  lecturaTemp[k] = valort;
  lecturaHum[k] = valorh;
}
String arregla(String a) {
  a.replace("%C3%A1", "á");
  a.replace("%C3%A9", "é");
  a.replace("%C3%A", "i");
  a.replace("%C3%B3", "ó");
  a.replace("%C3%BA", "ú");
  a.replace("%21", "!");
  a.replace("%23", "#");
  a.replace("%24", "$");
  a.replace("%25", "%");
  a.replace("%26", "&");
  a.replace("%27", "/");
  a.replace("%28", "(");
  a.replace("%29", ")");
  a.replace("%3D", "=");
  a.replace("%3F", "?");
  a.replace("%27", "'");
  a.replace("%C2%BF", "¿");
  a.replace("%C2%A1", "¡");
  a.replace("%C3%B1", "ñ");
  a.replace("%C3%91", "Ñ");
  a.replace("+", " ");
  a.replace("%2B", "+");
  a.replace("%22", "\"");
  return a;
}

void testopticalnetwork() {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(22, 16);
  display.clearDisplay();
  display.println("OPTICAL");
  display.setCursor(19, 28);
  display.println("networks");
  display.display();
  delay(200);
  display.clearDisplay();
}

void testMonitoreo() {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.clearDisplay();
  display.println("SSD:");
  display.setCursor(10, 24);
  display.println("IP:");
  display.setCursor(10, 44);
  display.println("ID:");
  display.display();
  delay(1000);
  display.clearDisplay();
}

/////////////////////  OPTICAL SCROLL    /////////////////////

void testscrollopticalnetwork(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  //
  display.setCursor(10, 16);
  display.clearDisplay();
  display.println("OPTICAL");
  display.setCursor(3, 28);
  display.println("networks");
  display.display();
  delay(1000);
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x06);
  delay(2000);
  display.stopscroll();
  display.display();

}

/////////////////////  OBTENER HORA    /////////////////////

String hora() {
  strHora = "";
  //get a random server from the pool
  udp.begin(localPort);
  WiFi.hostByName(ntpServerName, timeServerIP);

  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);

  int cb = udp.parsePacket();

    if (!cb) {
      return "-no packet yet-";
    }
    else {
        // We've received a packet, read the data from it
        udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
        //the timestamp starts at byte 40 of the received packet and is four bytes,
        // or two words, long. First, esxtract the two words:
        unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
        unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
        // combine the four bytes (two words) into a long integer
        // this is NTP time (seconds since Jan 1 1900):
        unsigned long secsSince1900 = (highWord + 5) << 16 | lowWord;
        // now convert NTP time into everyday time:
        // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
        const unsigned long seventyYears = 2208988800UL;
        // subtract seventy years:
        unsigned long epoch = secsSince1900 - seventyYears;
        // print Unix time:
        // print the hour, minute and second:
        horaActual = ((epoch  % 86400L) / 3600);
        minutoActual = ((epoch  % 3600) / 60);
        segundoActual = (epoch % 60);
      }
      if (horaActual < 10)strHora.concat("0");
      strHora.concat(horaActual);
      strHora.concat(":");
      if (minutoActual < 10)strHora.concat("0");
      strHora.concat(minutoActual);
      strHora.concat(":");
      if (segundoActual < 10)strHora.concat("0");
      strHora.concat(segundoActual);
      return strHora;
    }

  // send an NTP request to the time server at the given address
  unsigned long sendNTPpacket(IPAddress& address) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

///graba en EEPROM //////


void graba(int addr, String a) {
  int tamano = (a.length() + 1);
  char inchar[30];    //'30' Tamaño maximo del string
  a.toCharArray(inchar, tamano);
  EEPROM.write(addr, tamano);
  delay(180);
  for (int i = 0; i < tamano; i++) {
    addr++;
    EEPROM.write(addr, inchar[i]);
    delay(180);
  }
  EEPROM.commit();
}



/////////////////////  L E E R   EN LA  E E P R O M    /////////////////////

String lee(int addr) {
  String nuevoString = "";
  int valor;
  int tamano = EEPROM.read(addr) - 1;

  for (int i = 0; i < tamano; i++) {
    addr++;
    valor = EEPROM.read(addr);
    nuevoString += (char)valor;
  }
  return nuevoString;
}
////////////Restaura variables guardada en EEPROM//////////////////
void StartVar() {
  getssid = lee(1);
  getpass = lee(30);
  getmethod = lee(60);
  getssidip = lee(90);
  getgateway= lee(120);
  getidentikey = lee(150);
  getconfigurado=lee(180);

}




//// Funciones de hosteo ////////////////////////////////////////////////////////////
String scanWifi2() {
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  String a = "";
  for (int i = 0; i < n; ++i)
  {
    // Print SSID and RSSI for each network found

    String enctype = ((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? "" : "*");
    a += "<option value='" + enctype + String(WiFi.SSID(i)) + "'>" + String(WiFi.SSID(i)) + "</option>";
    delay(10);
  }
  return a;
}


/////////////////////


void root() {
  String html2 ="<div align=\"center\"><h2>Establecer red de Wifi</h2></div> <form action=\"/salvarDatos\" method=\"GET\" > Seleccione el SSID<br> <select name=\"ssid\" onchange=\"javascript:document.getElementById('divssipassword').style.display=((this.value.indexOf('*')!=-1)?'':'none')\"> <option value=\"\">--Seleccione valor</option>";
  String html3 = "</select> <div id=\"divssipassword\" style=\"display:none\"> Ingrese clave secreta<br> <input type=\"text\" name=\"ssidpassword\" > </div> Seleccione el metodo<br> <select name=\"method\" onchange=\"javascript:document.getElementById('divssidip').style.display=((this.value==1)?'':'none')\"> <option value=\"0\">DHCP</option> <option value=\"1\">Manual</option> </select> <div id=\"divssidip\" style=\"display:none\"> Ingrese la IP<br> <input type=\"text\" name=\"ssidip\" > Ingrese el gateway <input type=\"text\" name=\"gateway\"> ----- </div> Ingrese identity key<br> <input type=\"text\" name=\"identikey\" > <div align=\"center\" > <input type=\"submit\" value=\"Enviar\"> </div> </form> </body> </html>";
  server.send(200, "text/html",  html1 +  html2+ scanWifi2() + html3);

  
}

void validacion () {

if (getmethod=="0"){
  
  display.clearDisplay();
        display.setCursor(0, 20);
        display.println("se conectara por DHCP");
        display.display();
        conectarDHCP();
        
  
   
  }
  else  if (getmethod=="1")
  {
    display.clearDisplay();
        display.setCursor(0, 20);
        display.println("se conectara manualmente");
        display.display();
        conectarManual();
  }
  
}



//////////////////////////////////////////////////////////////////
void salvarDatos() {

  String html2 ="<div align=\"center\"><h2>Los datos suministados son:</h2></div> Nombre de la red:";
  String html3 ="<br> Clave secreta:";
  String html4 ="<br> Metodo:";
  String html5 ="<br> <div id=\"divssidip\"";
  String html6 ="> Ip estatico:";
  String html7 ="<br> Gateway:";
  String html8 ="<br> ----- </div> IdentiKey:";
  String html9 ="<br> <br> <button type=\"submit\" action=\"/validacion\" value=\"enviar\" align=\"center\"> Conectar </button> </body> </html>";
  
  String mostrar = "style='display:none'";  
  
  if(getmethod=="1"){ mostrar="";}
  int cuenta=0;
  display.clearDisplay();
  getssid = server.arg("ssid");
  delay(90);
  getpass = server.arg("ssidpassword");
  if (getpass != "") {
    getssid.remove(0, 1);
  }
  delay(90);
  getmethod = server.arg("method");
  delay(90);
  getssidip = server.arg("ssidip");
  delay(90);
  getgateway = server.arg("gateway");
  delay(90);
  getidentikey = server.arg("identikey");

  
  getssid = arregla(getssid);
  getpass = arregla(getpass);
  getmethod = arregla(getmethod);
  getssidip = arregla(getssidip);
  getgateway=arregla(getgateway);
  getidentikey = arregla(getidentikey);
  server.send(200, "text/html",  html1 + html2+ getssid +html3+ getpass + html4+ getmethod + html5 + mostrar + html6 +getssidip +html7 + getgateway + html8+ getidentikey + html9 );
  
////verificar lso datos obtenidos 
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("datos obtenidos");
        display.setCursor(0, 10);
        display.println(getssid);
        display.setCursor(0, 20);
        display.println(getpass);
        display.setCursor(0, 30);
        display.println(getmethod);
        display.setCursor(0, 40);
        display.println(getssidip);
        display.setCursor(0, 50);
        display.println(getgateway);
        display.setCursor(0, 60);
        display.println(getidentikey);
        display.display();
        delay(3000);
        display.clearDisplay();
//
  getssid.toCharArray(ssid, getssid.length() + 1);
  getpass.toCharArray(pass, getpass.length() + 1);
  getgateway.toCharArray(gateway,getgateway.length()+1);
  getssidip.toCharArray(ssidip,getssidip.length()+1);



}





//////////////////////////////////////////////////////////////////////////
void conectarDHCP(){
  WiFi.begin(ssid,pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
   
    cuenta++;
    if (cuenta > 10) {
      graba(150, "noconfigurado");
      server.send(200, "text/plain", "no se logró entrar");
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("no se logro conectar");
      display.display();
      delay(1000);
      return;
    }
  }
    if (WiFi.status() == WL_CONNECTED){
      server.send(200, "text/plain", "se logró entrar"); 
      strIP = "";
  
      graba(1, getssid);
      delay(10);
      graba(30, getpass);
      delay(10);
      graba(60, getmethod);
      delay(10);
      graba(90, getssidip);
      delay(10);   
      graba(120, getgateway);
      delay(10);
      graba(150, getidentikey);
      delay(10);
      graba(180, "configurado");
      delay(10);
      
      display.clearDisplay();
      display.println("conectado a: " + getssid);
      
   }
  
  }



/////////////////////////////////////
void conectarManual(){
 int p1,p2,p3,p4 =0;
 p1=getssid.indexOf(".");
 p2=getssid.indexOf(".",p1+1);
 p3=getssid.indexOf(".",p2+1);

 int ssid1,ssid2,ssid3,ssid4 =0;
 ssid1= getssid.substring(0,p1).toInt();
 ssid2= getssid.substring(p1+1,p2).toInt();
 ssid3= getssid.substring(p2+1,p3).toInt();
 ssid4= getssid.substring(p3).toInt();

 p1=getgateway.indexOf(".");
 p2=getgateway.indexOf(".",p1+1);
 p3=getgateway.indexOf(".",p2+1);

 int gw1,gw2,gw3,gw4 =0;
 gw1= getssid.substring(0,p1).toInt();
 gw2= getssid.substring(p1+1,p2).toInt();
 gw3= getssid.substring(p2+1,p3).toInt();
 gw4= getssid.substring(p3).toInt();
 
  
  IPAddress ip_static(ssid1,ssid2,ssid3,ssid4);    
  IPAddress ip_gateway(gw1,gw2,gw3,gw4); 
  IPAddress ip_subnet(255,255,255,0); 

  WiFi.config(ip_static, ip_gateway, ip_subnet); 
  
  WiFi.begin(ssid,pass);
  delay(100);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
   
    cuenta++;
    if (cuenta > 10) {
      graba(150, "noconfigurado");
      server.send(200, "text/plain", "no se logró entrar manualmente");
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("no se logro conectar");
      display.display();
      delay(1000);
      return;
    }
  }
    if (WiFi.status() == WL_CONNECTED){
      server.send(200, "text/plain", "se logró entrar manualmente"); 
      strIP = "";
  
      graba(1, getssid);
      delay(10);
      graba(30, getpass);
      delay(10);
      graba(60, getmethod);
      delay(10);
      graba(90, getssidip);
      delay(10);   
      graba(120, getgateway);
      delay(10);
      graba(150, getidentikey);
      delay(10);
      graba(180, "configurado");
      delay(10);
      
      display.clearDisplay();
      display.println("conectado a: " + getssid);
      
   }
  
  
}

////////////////////////////////////////





void intento_conexion() {
  // cuando se inicia el sistema intenta conectarse con la configuración anterior si es que logro hacerlo
  if (lee(150).equals("configurado")) {
    ////conectando con IP manual
    if(lee(60).equals("1")) // VERIFICA QUE METODO DE CONFIGURACION ES 1-> manual DHCP -> 0
    {
        ssid_leido = lee(1);      //leemos ssid y password
        pass_leido = lee(30);
        ssidip_leido = lee(90);
        gateway_leido= lee(120);

    ssid_tamano = ssid_leido.length() + 1;  //Calculamos la cantidad de caracteres que tiene el ssid y la clave
    pass_tamano = pass_leido.length() + 1;

    ssid_leido.toCharArray(ssid, ssid_tamano); //Transf. el String en un char array ya que es lo que nos pide WiFi.begin()
    pass_leido.toCharArray(pass, pass_tamano);

   int p1,p2,p3,p4 =0;
   p1=ssidip_leido.indexOf(".");
   p2=ssidip_leido.indexOf(".",p1+1);
   p3=ssidip_leido.indexOf(".",p2+1);
  
   int ssid1,ssid2,ssid3,ssid4 =0;
   ssid1= ssidip_leido.substring(0,p1).toInt();
   ssid2= ssidip_leido.substring(p1+1,p2).toInt();
   ssid3= ssidip_leido.substring(p2+1,p3).toInt();
   ssid4= ssidip_leido.substring(p3).toInt();
  
   p1=gateway_leido.indexOf(".");
   p2=gateway_leido.indexOf(".",p1+1);
   p3=gateway_leido.indexOf(".",p2+1);
  
   int gw1,gw2,gw3,gw4 =0;
   gw1= gateway_leido.substring(0,p1).toInt();
   gw2= gateway_leido.substring(p1+1,p2).toInt();
   gw3= gateway_leido.substring(p2+1,p3).toInt();
   gw4= gateway_leido.substring(p3).toInt();
   
    
    IPAddress ip_static(ssid1,ssid2,ssid3,ssid4);    
    IPAddress ip_gateway(gw1,gw2,gw3,gw4); 
    IPAddress ip_subnet(255,255,255,0); 
  
    WiFi.config(ip_static, ip_gateway, ip_subnet); 
  
    WiFi.begin(ssid, pass);      //Intentamos conectar
        
    }
    
    else{

      
    ssid_leido = lee(1);      //leemos ssid y password
    pass_leido = lee(30);

    

    ssid_tamano = ssid_leido.length() + 1;  //Calculamos la cantidad de caracteres que tiene el ssid y la clave
    pass_tamano = pass_leido.length() + 1;

    ssid_leido.toCharArray(ssid, ssid_tamano); //Transf. el String en un char array ya que es lo que nos pide WiFi.begin()
    pass_leido.toCharArray(pass, pass_tamano);

    int cuenta = 0;
    WiFi.begin(ssid, pass);      //Intentamos conectar
    }

    
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      cuenta++;
      if (cuenta > 10) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("Fallo al conectar automaticamente");
        return;
      }
    }
  }
  if (WiFi.status() == WL_CONNECTED) {
    
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(0, 0);
        display.print("Conexion exitosa");
  }
}



////



void setupWifiServer() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("Optiweather");
  //mdns.begin("optiweatherconfig",WiFi.localIP());
    
  IPAddress myIP = WiFi.softAPIP();

  server.on("/", root);
  server.on("/salvarDatos", salvarDatos);
  server.on ("/validacion", validacion);
  server.begin();
  //MDNS.addService("http","tcp",80)
  
}
////


void timer0_ISR () {
 segundos++;
  if(segundos%60==1 && !conectado){
    
    intento_conexion();
      
  }
  if (segundos%300==1 && conectado){

    // logica envío de datos 
  }
}


////

void setup() {
  Serial.begin(115200);//quitar
  Serial.println("encendiendo");
  EEPROM.begin(512); 
  Wire.begin(0, 2); //esp8266
  pinMode(1, INPUT); //esp8266
  display.begin();  // initialize with the I2C addr 0x3D (for the 128x64)
  display.display();
  display.clearDisplay();
  setupWifiServer();
  testscrollopticalnetwork();
  display.clearDisplay();
  /*
  noInterrupts();
  timer0_isr_init();
  timer0_attachInterrupt(timer0_ISR);
  timer0_write(ESP.getCycleCount() + 400000000L); // 80MHz == 1sec
  interrupts(); */

}



void loop() {
  noInterrupts();
      server.handleClient();
  interrupts(); 
     if (WiFi.status() == WL_CONNECTED) {
    strHoraActual = "  Hora : " + hora();
    display.clearDisplay();
    temperaturaHumedadNoHora();
    testopticalnetwork();
      }
      
    else{
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(0, 0);
        display.println("Por favor");
        display.setCursor(0, 20);
        display.println("entre a:");
        display.setCursor(0, 40);
        display.setTextSize(1);
        display.println("192.168.4.1");
        display.setCursor(0, 50);
        display.println("De la red Optiweather");
        display.display();
      }
    
}


