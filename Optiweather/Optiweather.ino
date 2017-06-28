        //Definiendo   
            #include <ESP8266WiFi.h>
            #include <ESP8266Ping.h>
            #include <SPI.h>
            #include <Wire.h>
            #include <Adafruit_SSD1306.h>
            #include <Adafruit_Sensor.h>
            #include <Adafruit_GFX.h>
            #include <ESP8266wifi.h>
            #include <Arduino.h>
            #include <ESP8266WiFiMulti.h>
            #include <ESP8266HTTPClient.h>
            #define USE_SERIAL Serial
            #include <ESP8266WebServer.h>
            #include <WiFiUdp.h>
            #include <SPI.h>
            #include <Wire.h>
            #include <DHT.h>
            #include <DHT_U.h>
            #include <EEPROM.h>
            #include <String.h>
            #include <FS.h>
            #define DHTTYPE DHT11
            #define DHTPIN 1//GPIO1-Tx
            DHT dht(DHTPIN, DHTTYPE);
            extern "C" {
              #include<user_interface.h>
            }


            #define OLED_RESET LED_BUILTIN
            Adafruit_SSD1306 display(OLED_RESET);

            #define NUMFLAKES 10
            #define XPOS 0
            #define YPOS 1
            #define DELTAY 2

            #define LOGO16_GLCD_HEIGHT 16
            #define LOGO16_GLCD_WIDTH  16

            ESP8266WiFiMulti WiFiMulti;
      

            int e=0; 
            int minuto= 100;//600; //un minuto en ticks
            bool conectar=false;

            String mensaje1="Ha pasado un minuto";
            String mensaje2="Se conectara";
            String mensaje3="utilizando los datos";
            String mensaje4= "guardados";

            byte letra = 0;

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

            const int ancho = 1 ;

            ////////////////////////variables host //////////////////////////

            char ssid[25] = "";  //  your network SSID (name)
            char pass[32] = "";       // your network password
            char gateway[20] ="";
            char ssidip[20] ="";

            String html1   ="<!DOCTYPE html> <html lang=\"es-Es\"> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=ISO-8859-1\"/> <head> <title>Optiweather</title> <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"> <meta content=\"width=device-width, initial-scale=1\" name=\"viewport\" /> <meta http-equiv='refresh' content='60' name='viewport' content='width=device-width, initial-scale=1'/> <link rel='stylesheet' type=\"text/css\" href=\"css/bootstrap.min.css\"> <style> html,body {padding:0px;maring:0px;text-align: left; width:100%;height:100%;font-size:100%;font-family:arial;LINE-HEIGHT:30px;background-color: #ffffff;} input[type=\"text\"],select {width:100%;} input[type=\"submit\"] { padding:10px ;width:40%;font-size:120%} div {width:100%} form {padding:20px} </style> </head> <body > <img src=\"data:image/png;base64,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\" width=\"100%\">";

            String getmethod="";

            String ssid_leido="";
            String pass_leido="";
            String gateway_leido="";
            String ssidip_leido="";
            String identitykey_leido ="";
            String metodo_leido ="";
            String mascara_leido="";
            String dns1_leido="";
            String dns2_leido="";
            String host_leido="";

            String response = "error";

            String statusconexion="";

            String a = "[";

            int cuenta=0;
            int ssid_tamano=0;
            int pass_tamano=0;
            int perdioconex=0;

            int h=0;
            int t=0;

            bool ik1=false;
            bool ik2 = false;

            ESP8266WebServer server(80);

            String json ="[ { status: \"1\", value: \"Ok\" } { statusText: \"1\", value: \"\" } ]";
         ////////////////////PING//////////////
            bool estadoping=false;
            int tiempoping=0;

          /////////////////////////////////////////////////////// COMPROBAR LA RESOLUCIÓN DE LA PANTALLA OLED
            #if (SSD1306_LCDHEIGHT != 64)
            #error("Height incorrect, please fix Adafruit_SSD1306.h!");
            #endif

            /////////////////////  UPTIME   /////////////////////

            int incomingByte =0;
            long currentmillis=0;
            

            // Imagenes para OLED

            #define partly_cloudy_day_width 50
            #define partly_cloudy_day_height 50


       const unsigned char termometro [] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x0C, 0x70, 0x00, 0x00, 0x08, 0x30, 0x00,
        0x00, 0x18, 0x1F, 0x00, 0x00, 0x18, 0x1F, 0x00, 0x00, 0x18, 0x10, 0x00, 0x00, 0x18, 0x10, 0x00,
        0x00, 0x19, 0xF0, 0x00, 0x00, 0x19, 0x10, 0x00, 0x00, 0x19, 0x0F, 0x00, 0x00, 0x19, 0x0F, 0x00,
        0x00, 0x19, 0x10, 0x00, 0x00, 0x19, 0x10, 0x00, 0x00, 0x19, 0x10, 0x00, 0x00, 0x19, 0x10, 0x00,
        0x00, 0x19, 0x10, 0x00, 0x00, 0x19, 0x08, 0x00, 0x00, 0x31, 0x04, 0x00, 0x00, 0x62, 0x02, 0x00,
        0x00, 0xC4, 0x02, 0x00, 0x00, 0x88, 0x01, 0x00, 0x00, 0x98, 0x01, 0x00, 0x00, 0x90, 0x01, 0x00,
        0x00, 0xE0, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0x40, 0x02, 0x00,
        0x00, 0x20, 0x04, 0x00, 0x00, 0x10, 0x18, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        const unsigned char gota [] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x07, 0xE0, 0x00,
        0x00, 0x07, 0xE0, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x1F, 0xF8, 0x00,
        0x00, 0x3F, 0xFC, 0x00, 0x00, 0x7F, 0xFE, 0x00, 0x00, 0x7F, 0xFE, 0x00, 0x00, 0xFF, 0xFF, 0x00,
        0x00, 0xFF, 0xFF, 0x00, 0x01, 0xFF, 0xFF, 0x80, 0x01, 0xFF, 0xFF, 0x80, 0x03, 0xFF, 0xFF, 0xC0,
        0x03, 0xFF, 0xFF, 0xC0, 0x03, 0x3F, 0xFF, 0xC0, 0x07, 0x3F, 0xFF, 0xE0, 0x07, 0x3F, 0xFF, 0xE0,
        0x06, 0x3F, 0xFF, 0xE0, 0x06, 0x3F, 0xFF, 0xE0, 0x07, 0x1F, 0xFF, 0xE0, 0x03, 0x0F, 0xFF, 0xC0,
        0x03, 0x07, 0xFF, 0xC0, 0x03, 0x83, 0xFF, 0xC0, 0x01, 0xC0, 0x7F, 0x80, 0x00, 0xF0, 0x3F, 0x00,
        0x00, 0xFE, 0xFF, 0x00, 0x00, 0x3F, 0xFC, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        struct dibujo1 {

            int x= 2 ;
            int y= 15;
            int w= 32;//44
            int h= 32;

        };

        struct dibujo2 {

            int x=65 ;  
            int y=15;
            int w=32;
            int h=32;
        };

        
                /// fin de variables globales
        void temperaturaHumedadNoHora() {
            dibujo1 termo;
            dibujo2 got;
            display.setTextSize(1);
            dibujoRssiMini();
            dht.begin();
            delay(3000);
            h = dht.readHumidity();
            t = dht.readTemperature();
            delay(1000);
            // Reading temperature or humidity takes about 250 milliseconds!
            // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
            if (isnan(t)|| t> 100) {
              t = 0;
            }
            if (isnan(h)|| h> 100) {
              h = 0;
            }
            display.setTextSize(2);
            display.setCursor(33, 22);
            display.print(t);
            //    display.print(27);
            display.setTextSize(1);
            display.print("c");
            display.print(" ");
            display.setTextSize(2);
            display.setCursor(96, 22);
            display.print(h);
            //    display.print(60);
            display.setTextSize(1);
            display.println("%");
            display.setCursor(0, 55);
            display.print(hora());
            display.drawFastHLine(0,  52,  128,WHITE);
            display.setCursor(0, 1);
            uptime();
            display.drawBitmap(termo.x, termo.y,  termometro, termo.w, termo.h, WHITE);
            display.drawBitmap(got.x, got.y,  gota, got.w, got.h, WHITE);
            
            display.display();
            delay(1 * 8* 1000);   
        }

        void dibujoRssiMini(){
            //display.setCursor(105, 0);
            int rssi =WiFi.RSSI();
            display.drawFastVLine( 114,  1,  8,WHITE);
            display.drawFastHLine(110,  1,  9,WHITE);
            display.drawPixel(111,2,WHITE);
            display.drawPixel(112,3,WHITE);
            display.drawPixel(113,4,WHITE);
            display.drawPixel(115,4,WHITE);
            display.drawPixel(116,3,WHITE);
            display.drawPixel(117,2,WHITE);


            if(rssi>-80){
            display.drawFastVLine( 116,  7,  2,WHITE);
                

            }
            if(rssi>-70){
            display.drawFastVLine( 118,  5,  4,WHITE);
                
            }
            if(rssi>-60){
            display.drawFastVLine( 120,  3,  6,WHITE);
                
            }
            if(rssi>-50){
            display.drawFastVLine( 122,  1,  8,WHITE);
                
            }

        }

        void dibujoRssi(){
            int rssi =WiFi.RSSI();
                display.drawFastVLine( 16,  25,  20,WHITE);
                display.drawFastVLine( 17,  26,  20,WHITE);
                display.drawFastHLine(8,  25,  18,WHITE);
                display.drawFastHLine(8,  26,  18,WHITE);
                int c=26;
                for(int i=8; i<16;i++){

                    display.drawPixel(i,c,WHITE);
                    display.drawPixel(i+1,c,WHITE);
                  
                    c++;
                }
                c=26;
                for(int i=25; i>17;i--){

                    display.drawPixel(i,c,WHITE);
                    display.drawPixel(i-1,c,WHITE);
                    
                    c++;
                }
                    if(rssi>-80){
                    display.drawFastVLine( 23,40,  5,  WHITE);
                    display.drawFastVLine( 24,40,  5,  WHITE);   
                    
                    }
                    if(rssi>-70){
                    display.drawFastVLine( 27,35,  10,  WHITE);
                    display.drawFastVLine( 28,35,  10,  WHITE); 
                      
                    }
                    if(rssi>-60){
                    display.drawFastVLine( 31,30,  15,  WHITE);
                    display.drawFastVLine( 32,30,  15,  WHITE);  
                      
                    }
                    if(rssi>-50){
                    display.drawFastVLine( 35,25,  20,  WHITE);
                    display.drawFastVLine( 36,25,  20,  WHITE); 
                      
                    }


        }

        String arregla(String n) {
            n.replace("%C3%A1", "á");
            n.replace("%C3%A9", "é");
            n.replace("%C3%A", "i");
            n.replace("%C3%B3", "ó");
            n.replace("%C3%BA", "ú");
            n.replace("%21", "!");
            n.replace("%23", "#");
            n.replace("%24", "$");
            n.replace("%25", "%");
            n.replace("%26", "&");
            n.replace("%27", "/");
            n.replace("%28", "(");
            n.replace("%29", ")");
            n.replace("%3D", "=");
            n.replace("%3F", "?");
            n.replace("%27", "'");
            n.replace("%C2%BF", "¿");
            n.replace("%C2%A1", "¡");
            n.replace("%C3%B1", "ñ");
            n.replace("%C3%91", "Ñ");
            n.replace("+", " ");
            n.replace("%2B", "+");
            n.replace("%22", "\"");
            return n;
        }

        void testopticalnetwork() {
             
            display.setTextSize(2);
            display.setTextColor(WHITE);
            display.setCursor(43,10);
            display.clearDisplay();
            display.println("OPTI");
            display.setCursor(25,25);
            display.println("WEATHER");
            display.setTextSize(1);
            display.setCursor(20,40);
            display.println("Optical Networks");
            display.setCursor(37,50);
            display.println("2017-V1.0");
            display.display();
            display.clearDisplay();

        }

        void pantallaConexion (){   
                display.clearDisplay();
                dibujoRssi();
                
                
                
                bool estadoping = Ping.ping("www.google.com");
                int tiempoping = Ping.averageTime();//respuesta en milisegundos
                display.setTextSize(1);
                display.setCursor(60,17);
                display.println("Google Ping");

                if(estadoping){
                display.setTextSize(2);
                display.setCursor(65,30);
                display.println(String(tiempoping));
                display.setCursor(105,35);
                display.setTextSize(1);
                display.println("ms");
                
                }

                else 
                {display.println("no ping");}
                display.setTextSize(1);
                display.setCursor(0,0);
                String ssidcorto = String(WiFi.SSID());
                if(ssidcorto.length()>16){
                    ssidcorto= ssidcorto.substring(0,16);
                }
                display.println("SSID:" + ssidcorto);
                display.setCursor(0,55);
                display.println("IP:" + String(WiFi.localIP()));
                subir();
                display.drawFastHLine(0,  52,  128,WHITE);
                display.display();  
        }
        /////////////////////  OPTICAL SCROLL    /////////////////////
        void testscrollopticalnetwork() {
         

            display.display();
            


        }

          /////////////////////  OBTENER HORA    /////////////////////

        String hora() {
            //
                /*strHora = "";
                //get a random server from the pool
                udp.begin(localPort);
                WiFi.hostByName(ntpServerName, timeServerIP);

                sendNTPpacket(timeServerIP); // send an NTP packet to a time server
                // wait to see if a reply is available
                delay(1000);

                int cb = udp.parsePacket();

                  if (!cb) {
                    return " ";
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
                udp.endPacket(); */
            //
            //sacando hora del servidor

                 if((WiFiMulti.run() == WL_CONNECTED) ) {  
                HTTPClient http;
                USE_SERIAL.print("[HTTP] begin...\n");
                http.begin("http://"+ host_leido+"/hora.php"); //HTTP
                int httpCode = http.GET();

                if(httpCode > 0) {
                    if(httpCode == HTTP_CODE_OK) {
                        response= http.getString();
                            http.end();
                          return response;  
                    } else {
                        http.end();
                        return "";
                    }
                } 
                
            }
        }
            //////// UPTIME ////

        void uptime() {
        
               currentmillis=millis(); // get the  current milliseconds from arduino
               // report milliseconds
               Serial.print("Total milliseconds running: "); 
               Serial.println(currentmillis);
                long days=0;
                 long hours=0;
                 long mins=0;
                 long secs=0;
                 secs = currentmillis/1000; //convect milliseconds to seconds
                 mins=secs/60; //convert seconds to minutes
                 hours=mins/60; //convert minutes to hours
                 days=hours/24; //convert hours to days
                 secs=secs-(mins*60); //subtract the coverted seconds to minutes in order to display 59 secs max 
                 mins=mins-(hours*60); //subtract the coverted minutes to hours in order to display 59 minutes max
                 hours=hours-(days*24); //subtract the coverted hours to days in order to display 23 hours max
                 
                display.println("UP:"+String(days)+"d:"+String(hours)+"h:"+String(mins)+"m:"+String(secs)+"s");
                display.display();
          
        }
          ///graba en EEPROM //////
        void graba(int addr, String c) {
            int tamano = (c.length() + 1);
            char inchar[30];    //'30' Tamaño maximo del string
            c.toCharArray(inchar, tamano);
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
          //// Funciones de hosteo ////////////////////////////////////////////////////////////
        void root() {
  
            String html ="<html> <head> <title>Redirigir al navegador a otra URL</title> <META HTTP-EQUIV=\"REFRESH\" CONTENT=\"1;URL=http://192.168.4.1/index.html\"> </head> <body>  </body> </html>";
            server.send(200, "text/html",  html);

            display.clearDisplay();
            display.setTextSize(1);
            display.setCursor(0, 0);
            display.println("Se esta recepcionando");
            display.setCursor(0, 10);
            display.println("los datos de configuracion");
            display.display(); 
            delay(2000);
        }
          ///////////////////////////////////////////////////
        void validar (){           
            server.send(200, "text/html", json);    
        }

        //////////////////////////////////////////////////////

        void salvarDatos() {

            display.clearDisplay();
            String getssid = server.arg("in_ssid");
            delay(90);

            String getpass = server.arg("in_password");
            if (getpass != "") {
              getssid.remove(0, 1);
            }
            delay(90);
         getmethod = server.arg("in_metodo");
            delay(90);
            String met="DHCP";
                  if(getmethod=="1"){met="Manual";}
            String getssidip = server.arg("in_ip");
            delay(90);
            String getgateway = server.arg("in_gateway");
            delay(90);
            String getidentitykey = server.arg("in_identitykey");
            delay(90);
            String getmascara = server.arg("in_mask");
                if(getmethod==0){getmascara="";}
            delay(90);
            String getdns1 = server.arg("in_dns1");
            delay(90);
            String getdns2 = server.arg("in_dns2");
            delay(90);
            String gethost = server.arg("in_urlreport");
            delay(90);

            
            getssid = arregla(getssid);
            getpass = arregla(getpass);
            getmethod = arregla(getmethod);
            getssidip = arregla(getssidip);
            getgateway=arregla(getgateway);
            getidentitykey = arregla(getidentitykey);

    
            ///////////verificar lso datos obtenidos ///////////
                  display.clearDisplay();
                  display.setTextSize(1);
                  display.setCursor(10, 25);
                  display.println("Se han obtenido los ");
                  display.setCursor(10, 35);
                  display.println("siguientes datos");
                  display.display();
                  delay(1500);
                  display.clearDisplay();

                  display.clearDisplay();
                  display.setTextSize(1);
                  display.setCursor(0, 0);
                  display.println("SSID:"+getssid);
                  display.setCursor(0, 10);
                  display.println("clave:"+getpass);
                  display.setCursor(0, 20);
                  display.println("metodo:"+met);
                  //display.println("metodo:"+getmethod);
                  display.setCursor(0, 30);
                  display.println("IP est:"+getssidip);
                  display.setCursor(0, 40);
                  display.println("gateway:"+getgateway);
                  display.setCursor(0, 50);
                  display.println("mascara:"+getmascara);
                  display.display();
                  delay(4000);
                  display.clearDisplay();
                  display.setCursor(0, 20);
                  display.println("DNS1:"+getdns1);
                  display.setCursor(0, 30);
                  display.println("DNS2:"+getdns2);
                  display.setCursor(0, 40);
                  display.println("Host:"+gethost);
                  display.setCursor(0, 50);
                  display.println("identitykey:"+getidentitykey);
                  display.display();
                  delay(1500);
                  display.clearDisplay();


            graba(1,getssid);
            delay(10);
            graba(30,getpass);
            delay(10);
            graba(60,getmethod);
            delay(10);
            graba(90,getssidip);
            delay(10);
            graba(120,getgateway);
            delay(10);
            graba(150,getidentitykey);
            delay(10);
            graba(180,getmascara);
            delay(10);
            graba(210,getdns1);
            delay(10);
            graba(240,getdns2);
            delay(10);
            graba(270,gethost);
            delay(10);

            String json ="[ { status: \"1\", value: \"Ok\" } { statusText: \"1\", value: \"\" } ]";
            
            server.send(200, "text/html", json);
            e=minuto+1;
         
        }
        void escanearssids() {

        }


        void getssids() {
            // WiFi.scanNetworks will return the number of networks found
               int n = WiFi.scanNetworks();
            
            for (int i = 0; i < n; ++i)
            {
              // Print SSID and RSSI for each network found

              String enctype = ((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? "" : "*");
              a += "{ id: '" + enctype + String(WiFi.SSID(i)) + "', value: '" + String(WiFi.SSID(i)) + "'},";
              delay(10);
            }
            a+="]";
            server.send(200, "text/json",  a);
            
        }
          ////////////Restaura variables guardada en EEPROM//////////////////
        void startVar() { 

            ssid_leido = lee(1);      
            pass_leido = lee(30);
            metodo_leido = lee(60);
            ssidip_leido = lee(90);
            gateway_leido= lee(120);
            identitykey_leido = lee(150);
            mascara_leido=lee(180);
            dns1_leido=lee(210);
            dns2_leido=lee(240);
            host_leido=lee(270);


        }

        boolean conectarWifi(){
            startVar();
            int cuenta = 0;
            
            //ssid_leido="EMR";
            ssid_leido.toCharArray(ssid, ssid_leido.length() + 1);
            pass_leido.toCharArray(pass, pass_leido.length() + 1);

            display.clearDisplay();
            display.setTextSize(1);
            display.setCursor(0, 20);  

            if(metodo_leido=="1") // VERIFICA QUE METODO DE CONFIGURACION ES 1-> manual DHCP -> 0
              {

               int p1,p2,p3,p4 =0;
               p1=ssidip_leido.indexOf(".");
               p2=ssidip_leido.indexOf(".",p1+1);
               p3=ssidip_leido.indexOf(".",p2+1);
              
               int ssid1,ssid2,ssid3,ssid4 =0;
               ssid1= ssidip_leido.substring(0,p1).toInt();
               ssid2= ssidip_leido.substring(p1+1,p2).toInt();
               ssid3= ssidip_leido.substring(p2+1,p3).toInt();
               ssid4= ssidip_leido.substring(p3+1).toInt();
         ////////////////////////////////////////////////////////      
               p1=gateway_leido.indexOf(".");
               p2=gateway_leido.indexOf(".",p1+1);
               p3=gateway_leido.indexOf(".",p2+1);
              
               int gw1,gw2,gw3,gw4 =0;
               gw1= gateway_leido.substring(0,p1).toInt();
               gw2= gateway_leido.substring(p1+1,p2).toInt();
               gw3= gateway_leido.substring(p2+1,p3).toInt();
               gw4= gateway_leido.substring(p3+1).toInt();
         /////////////////////////////////////////////////////////
               p1=mascara_leido.indexOf(".");
               p2=mascara_leido.indexOf(".",p1+1);
               p3=mascara_leido.indexOf(".",p2+1);
              
               int m1,m2,m3,m4 =0;
               m1= mascara_leido.substring(0,p1).toInt();
               m2= mascara_leido.substring(p1+1,p2).toInt();
               m3= mascara_leido.substring(p2+1,p3).toInt();
               m4= mascara_leido.substring(p3+1).toInt();
         ////////////////////////////////////////////////////////
               p1=dns1_leido.indexOf(".");
               p2=dns1_leido.indexOf(".",p1+1);
               p3=dns1_leido.indexOf(".",p2+1);
              
               int dns11,dns12,dns13,dns14 =0;
               dns11= dns1_leido.substring(0,p1).toInt();
               dns12= dns1_leido.substring(p1+1,p2).toInt();
               dns13= dns1_leido.substring(p2+1,p3).toInt();
               dns14= dns1_leido.substring(p3+1).toInt();
         ////////////////////////////////////////////////////////
               p1=dns2_leido.indexOf(".");
               p2=dns2_leido.indexOf(".",p1+1);
               p3=dns2_leido.indexOf(".",p2+1);
              
               int dns21,dns22,dns23,dns24 =0;
               dns21= dns2_leido.substring(0,p1).toInt();
               dns22= dns2_leido.substring(p1+1,p2).toInt();
               dns23= dns2_leido.substring(p2+1,p3).toInt();
               dns24= dns2_leido.substring(p3+1).toInt();
         ////////////////////////////////////////////////////////

               
              IPAddress ip_static(ssid1,ssid2,ssid3,ssid4);    
              IPAddress ip_gateway(gw1,gw2,gw3,gw4); 
              IPAddress ip_subnet(m1,m2,m3,m4);
              IPAddress ip_dns1 ( dns11,dns12,dns13,dns14);
              IPAddress ip_dns2 ( dns21,dns22,dns23,dns24);
              display.println("usando metodo Manual"  );
              //WiFi.config(ip_static, ip_gateway, ip_subnet); 
               WiFi.config(ip_static, ip_gateway, ip_subnet,ip_dns1,ip_dns2); 
                  //Intentamos conectar    
              }else{
               if(metodo_leido=="0"){
                  display.println("usando metodo DHCP"  );
                   //Intentamos conectar
                }
              }

                display.setCursor(0, 30);
                display.println("SSID:"+ ssid_leido );
                display.setCursor(0, 40);
                display.println("Password:"+ pass_leido );
                display.display();

              WiFi.begin(ssid, pass); 

              delay(2000);

              cuenta=0;

              while (WiFi.status() != WL_CONNECTED) {
                delay(200);
                cuenta++;
                display.setCursor(0, 30);
                display.clearDisplay();
                display.println("intentando, veces:" + String(cuenta));
                display.display();
                if (cuenta > 30) { 
                  display.setTextSize(1);
                  display.setCursor(0, 40);
                  display.println("Fallo al conectar");
                  display.display();
                  delay(200);
                  display.clearDisplay();
                  e=0;
                  conectar=false;
                  return false;        
                }        
         }
            
            if (WiFi.status() == WL_CONNECTED) {

                  conectar=true;
                  display.clearDisplay();
                  display.setTextSize(1);
                  display.setCursor(0, 0);
                  display.println("conectado a: " + ssid_leido);
                  display.display();
   
                  
              }    

              return true;
          }


        bool verificaridentitykey() {

            if((WiFiMulti.run() == WL_CONNECTED) ) {  
                HTTPClient http;
                USE_SERIAL.print("[HTTP] begin...\n");
                http.begin("http://"+ host_leido+"/valididentitykey.php?identitykey="+identitykey_leido); //HTTP
                int httpCode = http.GET();

                if(httpCode > 0) {
                    if(httpCode == HTTP_CODE_OK) {
                        response= http.getString();
                        http.end();
                          return response;  
                    } else {
                        http.end();
                        return false;
                    }
                } 
                
            }
        }   

        boolean verificardatos(){

            if (conectarWifi()==false) {
                server.send(200, "text/html",  "{status:'100'.statustext:'no fue posible conectarse a la WIFI'}");
                display.clearDisplay();
                  display.setTextSize(1);
                  display.setCursor(0, 0);
                  display.println("Error conexión");
                  display.display();
                  delay(2000);
                  display.clearDisplay();
                return false;

            }

            if (Ping.ping("www.google.com")==false) {
                server.send(200, "text/html",  "{status:'101'.statustext:'no hay conexion a internet'}");
                display.clearDisplay();
                  display.setTextSize(1);
                  display.setCursor(0, 0);
                  display.println("Error internet");
                  display.setCursor(0, 20);
                  display.display();
                  delay(2000);
                  display.clearDisplay();
                return false;
            }

            if (verificaridentitykey()==false) {
                server.send(200, "text/html",  "{status:'102'.statustext:'servidor de validación no responde'}");
                display.clearDisplay();
                  display.setTextSize(1);
                  display.setCursor(0, 0);
                  display.println("Error servidor");
                  display.setCursor(0, 20);
                  display.display();
                  delay(2000);
                  display.clearDisplay();
                return false;
            }
            if (response=="0"){
                statusconexion="IdentityKey erronea";
                server.send(200, "text/html",  "{status:'103'.statustext:'IdentityKey erronea'}");
               display.clearDisplay();
                  display.setTextSize(1);
                  display.setCursor(0, 0);
                  display.println("Error identitykey");
                  display.setCursor(0, 20);
                  display.display();
                  delay(2000);
                  display.clearDisplay();
                return false;
            }
            if(response=="error") {   
                server.send(200, "text/html",  "{status:'104'.statustext:'error en el servidor web'}");
                display.clearDisplay();
                  display.setTextSize(1);
                  display.setCursor(0, 0);
                  display.println("Error servidorweb");
                  display.setCursor(0, 20);
                  display.display();
                  delay(2000);
                  display.clearDisplay();
                return false;
            }
            server.send(200, "text/html",  "{status:'ok'.statustext:''}");
                  /*display.clearDisplay();
                  display.setTextSize(1);
                  display.setCursor(0, 0);
                  display.println("Todo ok");
                  display.setCursor(0, 20);
                  display.display();
                  delay(2000);*/
                  display.clearDisplay();
                  for(int i=0; i<150; i++){
             server.handleClient();}
             return true;
             
             
        }


        void subir(){
            display.setTextSize(1);
            display.setCursor(120, 0);

            if((WiFiMulti.run() == WL_CONNECTED)) {
            
                HTTPClient http;

                USE_SERIAL.print("[HTTP] begin...\n");

                http.begin("http://"+ host_leido+"/setvalues.php?deviceid="+String(identitykey_leido)+"&valtemp="+String(t)+"&valhume="+String(h)); //HTTP

                // start connection and send HTTP header
                int httpCode = http.GET();

                // httpCode will be negative on error
                if(httpCode > 0) {
                    // HTTP header has been send and Server response header has been handled
                    // file found at server
                    if(httpCode == HTTP_CODE_OK) {
                        String payload = http.getString();
                        USE_SERIAL.println(payload);
                        display.println("o");
                    }
                } 
                else {
                    display.println("x");
                }   

                http.end();
            }

        display.display();
        }

        void setupWifiServer() {
            WiFi.mode(WIFI_OFF); 
            delay(500);
            WiFi.mode(WIFI_AP_STA);
            WiFi.softAP("Optiweather");     
            IPAddress myIP = WiFi.softAPIP();
            server.on("/", root);
            server.on("/salvarDatos", salvarDatos);
            server.on("/validar", validar);
            server.on("/getssids", getssids);
             server.on("/verificardatos", verificardatos);
            server.serveStatic("/", SPIFFS, "/");         
            server.begin();
            delay(500);
            void escanearssids();
            conectar=false;

        }

        void setup() {
            USE_SERIAL.begin(115200);
            EEPROM.begin(512); 
            SPIFFS.begin();
            Wire.begin(0, 2); //0 , 2 en esp y nodemcu. En wemos 5,4 
            pinMode(1, INPUT); 
            display.begin();  // initialize with the I2C addr 0x3D (for the 128x64)
            display.setTextColor(WHITE);
            display.clearDisplay();
            testscrollopticalnetwork();
            display.clearDisplay();
            pinMode(5, OUTPUT);
            pinMode(4, OUTPUT);
            digitalWrite(5, HIGH);
            digitalWrite(4, LOW);
            
        }
        
        void loop() {
           
            if(!ik2){
              setupWifiServer();
              while(e<minuto){
                
                  display.clearDisplay();
                  display.setTextSize(1);
                  display.setCursor(0, 0);
                  display.println("Bienvenido      " +String(e/10)+" s");
                  display.setCursor(0, 20);
                  display.println("por favor ingrese a:");
                  display.setCursor(0, 30);
                  display.setTextSize(1); 
                  display.println("192.168.4.1");
                  display.setCursor(0, 40);
                  display.println("Para configurar el");
                  display.setCursor(0, 50);
                  display.println("optiweather  " );
                  display.display();
                  server.handleClient();
                  if(wifi_softap_get_station_num()==NULL){
                  e++;}

              }
                if(wifi_softap_get_station_num()==NULL){
                   display.clearDisplay();
                  display.setTextSize(1);
                  display.setCursor(0, 20);
                  display.println(mensaje1);
                  display.setCursor(0, 30);
                  display.println(mensaje2);
                  display.setCursor(0, 40);
                  display.setTextSize(1); 
                  display.println(mensaje3);
                  display.setCursor(0, 50);
                  display.println(mensaje4);
                  display.display();
                  delay(1000);
                  display.clearDisplay();
                }
                display.clearDisplay();
                display.setTextSize(1);
                display.setCursor(0, 0);
                display.println("verificando datos...");
                ik2=verificardatos();
              /*conectarWifi();*/
              delay(2000);
              
            }
                   
                if(ik2){
                WiFi.mode(WIFI_AP_STA);
                perdioconex=0;
                temperaturaHumedadNoHora(); // Pantalla de Datos de T y H
                pantallaConexion();//pantalla de datos de conexión a internet
                delay(4000);
                testopticalnetwork();//pantalla de Optiweather V1.0
                //si se desconecta de pronto voverá al inicio
                 while (WiFi.status() != WL_CONNECTED) {

                    display.clearDisplay();
                    display.setTextSize(1);
                    display.setCursor(0,30);
                    display.println("Se ha perdido la conexión");  
                    display.setCursor(0,40);
                    display.println("esperando...");
                    perdioconex++;
                    if(perdioconex >= 600){
                        conectar=false;
                        e=0;
                    }

                 }
                }
                else{
                    WiFi.mode(WIFI_OFF); 
                    delay(5000);
                    conectar=false;
                    ik1=false;
                    ik2=false;
                    e=0;
                }
        }

        
        