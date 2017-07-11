#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <DHT.h>
#include <DHT_U.h>
#include <EEPROM.h>

#define DHTTYPE DHT11 
#define DHTPIN 1 //GPIO1-Tx
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
int f = 0;
byte hum = 0;
byte tempe = 0;

byte letra = 0;
byte valorByte = 0;

bool toggle = false;
bool medidor = false;

char ssid[] = "EMR";  //  your network SSID (name)
char pass[] = "12345678";       // your network password

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

int cuenta =0;

byte lecturaTemp[127]={0};
byte lecturaHum[127]={0};

int tempOhum = 0;

const int ancho = 1 ;

int k = 0;


#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


void barChartTemp(){
      display.clearDisplay();
      display.setCursor(0,0);
      display.setTextSize(1);
      display.print("Grafico-temperatura");
      for (int j=0 ; j<128 ; j++)
        {  int X0 = 127-(ancho * j) ;
           int X1 = ancho ;
           byte H = (lecturaTemp[127-j]*50)/50;
           display.fillRect( X0,64-H ,X1 , 64, 1 );//49
        }
      display.drawLine(0, 63, 127, 63, 1);
      display.display();
      delay(1*3*1000); /////////////////////// m, s, ms

   }

void barChartHum(){
      display.clearDisplay();
      display.setCursor(0,0);
      display.setTextSize(1);
      display.print("Grafico-humedad");
      for (int j=0 ; j<128 ; j++)
        {  int X0 = 127-(ancho * j) ;
           int X1 = ancho ;
           byte H = (lecturaHum[127-j]*40)/100;
           display.fillRect( X0,64-H,X1 , 64, 1 );
        }
      display.drawLine(0, 63, 127, 63, 1);
      display.display();
      delay(1*3*1000); /////////////////////// m, s, ms

   }
/////////////////////  TEMPERATURA HUMEDAD    /////////////////////

void temperaturaHumedadNoHora(){
      display.setTextSize(1);
      display.setCursor(0,0);
      display.println("OPTIWEATHER");
      // Reading temperature or humidity takes about 250 milliseconds!
      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
      dht.begin();
      delay(2000);
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      if (isnan(t)){t=100;}
      if (isnan(h)){h=100;}
      desplazaYgraba(t,h);
      
      display.setTextSize(3);
      display.setCursor(20,17);

      display.print(lecturaTemp[127]);
      display.setTextSize(1);
      display.print("c");
      display.print(" ");
      display.setTextSize(3);
      display.print(lecturaHum[127]);
      display.setTextSize(1);
      display.println("%");
      display.setCursor(7,55);
      display.setTextSize(1);
      display.print(strHoraActual);
      display.display();
      delay(1*4*1000); /////////////////////// m, s, ms

}

void desplazaYgraba (int valort,int valorh){
  for (k= 0; k<127; k++){
    lecturaTemp[k] = lecturaTemp[k+1];
    lecturaHum[k] = lecturaHum[k+1];
  }
  lecturaTemp[k] = valort;
  lecturaHum[k] = valorh;
}

void testopticalnetwork() {
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(22,16);
      display.clearDisplay();
      display.println("OPTICAL");
      display.setCursor(19,28);
      display.println("networks");
      display.display();
      delay(500);
      display.clearDisplay();
}

void testMonitoreo() {
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(10,0);
      display.clearDisplay();
      display.println("SSD:");
      display.setCursor(10,24);
      display.println("IP:");
      display.setCursor(10,44);
      display.println("ID:");
      display.display();
      delay(1000);
      display.clearDisplay();
}

/////////////////////  OPTICAL SCROLL    /////////////////////

void testscrollopticalnetwork(void) {
  display.setTextSize(2 );
  display.setTextColor(WHITE);
  //
  display.setCursor(10,16);
  display.clearDisplay();
  display.println("OPTICAL");
  display.setCursor(3,28);
  display.println("networks");
  display.display();
  //
  /*display.setCursor(10,0);
  display.clearDisplay();
  display.println("OPTICAL");
  display.println(" network");
  display.display();*/
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
}

/////////////////////  OBTENER HORA    /////////////////////

String hora(){
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
  if (horaActual<10)strHora.concat("0");
  strHora.concat(horaActual);
  strHora.concat(":");
  if (minutoActual<10)strHora.concat("0");
  strHora.concat(minutoActual);
  strHora.concat(":");
  if (segundoActual<10)strHora.concat("0");
  strHora.concat(segundoActual);
  return strHora;
}

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress& address){
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


void setup(){
  Wire.begin(0  ,2); //esp8266
  pinMode(1,INPUT);//esp8266
  display.begin();  // initialize with the I2C addr 0x3D (for the 128x64)
  display.display();
  display.clearDisplay();
  dht.begin();
  //testscrollopticalnetwork();
  display.clearDisplay();
}

void loop(){
       display.setTextSize(2 );
      display.setTextColor(WHITE);
        if (WiFi.status() != WL_CONNECTED)WiFi.begin(ssid, pass); 
        strHoraActual = "No WIFI - No Clock";
        if (WiFi.status() == WL_CONNECTED){
           strHoraActual = "  Hora : " + hora();
        }  
        
        temperaturaHumedadNoHora();           
        testopticalnetwork();
}

