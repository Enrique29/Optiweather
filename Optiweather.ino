    //Definiendo   
        #include <ESP8266WiFi.h>
        #include <ESP8266Ping.h>
        #include <Adafruit_SSD1306.h>
        #include <Adafruit_Sensor.h>
        #include <ESP8266wifi.h>
        #include <Arduino.h>
        #include <ESP8266WiFiMulti.h>
        #include <ESP8266HTTPClient.h>
        #define USE_SERIAL Serial
        #include <ESP8266WebServer.h>
        #include <WiFiUdp.h>
        #include <SPI.h>
        #include <Wire.h>
        #include <Adafruit_GFX.h>
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
        bool conectadoporweb=false;
        String mensaje1="Ha pasado un minuto";
        String mensaje2="Se conectara";
        String mensaje3="utilizando los datos";
        String mensaje4= "guardados";

        int k=0;

        byte letra = 0;

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

        const int ancho = 1 ;

        ////////////////////////variables host //////////////////////////

        char ssid[25] = "";  //  your network SSID (name)
        char pass[32] = "";       // your network password
        char gateway[20] ="";
        char ssidip[20] ="";

        String html1   ="<!DOCTYPE html> <html lang=\"es-Es\"> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=ISO-8859-1\"/> <head> <title>Optiweather</title> <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"> <meta content=\"width=device-width, initial-scale=1\" name=\"viewport\" /> <meta http-equiv='refresh' content='60' name='viewport' content='width=device-width, initial-scale=1'/> <link rel='stylesheet' type=\"text/css\" href=\"css/bootstrap.min.css\"> <style> html,body {padding:0px;maring:0px;text-align: left; width:100%;height:100%;font-size:100%;font-family:arial;LINE-HEIGHT:30px;background-color: #ffffff;} input[type=\"text\"],select {width:100%;} input[type=\"submit\"] { padding:10px ;width:40%;font-size:120%} div {width:100%} form {padding:20px} </style> </head> <body > <img src=\"data:image/png;base64,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\" width=\"100%\">";


        String getssid = "";
        String getpass = "";
        String getmethod  = "";
        String getssidip = "";
        String getidentikey = "";
        String getgateway="";
        String getmascara="";
        String getdns1="";
        String getdns2="";

        String ssid_leido="";
        String pass_leido="";
        String gateway_leido="";
        String ssidip_leido="";
        String identikey_leido ="";
        String metodo_leido ="";
        String mascara_leido="";
        String dns1_leido="";
        String dns2_leido="";

        String identikeyBD="";

        int cuenta=0;
        int ssid_tamano=0;
        int pass_tamano=0;
        int perdioconex=0;

        int h=0;
        int t=0;

        ESP8266WebServer server(80);
     ////////////////////PING//////////////
        bool estadoping=false;
        int tiempoping=0;

      /////////////////////////////////////////////////////// COMPROBAR LA RESOLUCIÓN DE LA PANTALLA OLED
        #if (SSD1306_LCDHEIGHT != 64)
        #error("Height incorrect, please fix Adafruit_SSD1306.h!");
        #endif

        /////////////////////  TEMPERATURA HUMEDAD    /////////////////////

        ///conexión con DB////
        char serv[] = "10.20.1.112";

    /// fin de variables globales
    void temperaturaHumedadNoHora() {
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("OPTIWEATHER");
        // Reading temperature or humidity takes about 250 milliseconds!
        // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
        dht.begin();
        delay(2000);
         h = dht.readHumidity();
         t = dht.readTemperature();

        if (isnan(t)|| t> 100) {
          t = 0;
        }
        if (isnan(h)|| h> 100) {
          h = 0;
        }

        display.setTextSize(3);
        display.setCursor(20, 17);

        display.print(t);
        display.setTextSize(1);
        display.print("c");
        display.print(" ");
        display.setTextSize(3);
        display.print(h);
        display.setTextSize(1);
        display.println("%");
        display.setCursor(7, 55);
        display.setTextSize(1);
        display.print(strHoraActual);
        subir();
        display.display();
        delay(1 * 5* 1000);   
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
        display.setTextSize(1);
        display.setCursor(0,45);
        display.println("red:" + String(WiFi.SSID()));
        display.setCursor(0,55);
        display.println("Ip:" + String(WiFi.localIP()));
        display.display();
        delay(100);
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


      //// Funciones de hosteo ////////////////////////////////////////////////////////////


    void root() {
        String html2 ="<div class=\"form-horizontal\"> <fieldset> <div class=\"well bs-component\"> <div align=\"center\"><h2>Establecer red de Wifi</h2></div> <form action=\"/salvarDatos\" method=\"GET\" > <label for=\"inputEmail\" class=\"col-lg-2 control-label\">Seleccione el SSID </label> <br> <div class=\"col-lg-10\"> <select name=\"ssid\" onchange=\"javascript:document.getElementById('divssipassword').style.display=((this.value.indexOf('*')!=-1)?'':'none')\" class=\"form-control\"> <option value=\"\">--Seleccione valor</option>";
        String html3 ="</select> </div> <div id=\"divssipassword\" style=\"display:none\"> <label class=\"col-lg-2 control-label\">Ingrese clave secreta</label> <br> <div class=\"col-lg-10\"> <input type=\"text\" class=\"form-control\" name=\"ssidpassword\" > </div> </div> <label class=\"col-lg-2 control-label\">Seleccione el metodo </label> <br> <div class=\"col-lg-10\"> <select name=\"method\" onchange=\"javascript:document.getElementById('divssidip').style.display=((this.value==1)?'':'none')\" class=\"form-control\"> <option value=\"0\">DHCP</option> <option value=\"1\">Manual</option> </select> </div> <div id=\"divssidip\" style=\"display:none\" class=\"col-lg-10\" > <label class=\"col-lg-2 control-label\">IP </label> <br> <input type=\"text\" class=\"form-control\" name=\"ssidip\" size=\"30\" > <label class=\"col-lg-2 control-label\">Gateway </label> <br> <input type=\"text\" class=\"form-control\" name=\"gateway\" > <label class=\"col-lg-2 control-label\">Mascara</label> <br> <input type=\"text\" class=\"form-control\" name=\"mascara\" value=\"255.255.255.0\"> <label class=\"col-lg-2 control-label\">DNS1</label> <br> <input type=\"text\" class=\"form-control\" name=\"dns1\"> <label class=\"col-lg-2 control-label\">DNS2</label> <br> <input type=\"text\" class=\"form-control\" name=\"dns2\"> ----- </div> <label class=\"col-lg-2 control-label\">Ingrese identity key</label> <br> <div class=\"col-lg-10\"> <input type=\"text\" class=\"form-control\" name=\"identikey\" > </div> <div align=\"center\" > <br> <input type=\"submit\" value=\"Enviar\" class=\"btn btn-warning\"> </div> </form> </div> </fieldset> </div> </body> </html>";

        server.send(200, "text/html",  html1 +  html2+ scanWifi2() + html3);
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

    void validacion () {
        
       ///////////////////////////////////FLAG
        display.clearDisplay();
        display.setCursor(0, 20);
        display.println("Guardando datos, por favor espere");
        display.display(); 
        
        if (getmethod=="0"){
        

              display.setCursor(0, 50);
              display.println("se conectara por DHCP");
              display.display();   
        
        }
        else  if (getmethod=="1")
        {
          
              display.setCursor(0, 40);
              display.println("se conectara");
              display.setCursor(0, 50);
              display.println("manualmente");
              display.display();
              
            
        
       
        }
        server.send(200, "text/plain", "Conectandose, ver pantalla Oled");
        e=minuto+1;
        delay(1500);
        display.clearDisplay();
        conectadoporweb=true;
    }

      //////////////////////////////////////////////////////

    void salvarDatos() {

        String html2s ="<div class=\"form-horizontal\"> <fieldset> <div class=\"well bs-component\"> <form action=\"/validacion\"> <div align=\"center\"><h2>Los datos suministados son:</h2></div> Nombre de la red:";
        String html3s ="<br> Clave secreta:";
        String html4s ="<br> Metodo:";
        String html5s ="<br> Ip estatico:";
        String html6s ="<br> Gateway:";
        String html7s ="<br> Mascara:";
        String html8s ="<br> DNS1:";
        String html9s ="<br> DNS2:";
        String html10s ="<br> ----- <br> IdentiKey:";
        String html11s ="<br> <br> <div align=\"center\"> <input type=\"submit\" value=\"enviar\" class=\"btn btn-warning\" > </input> </div> </form> </div> </fieldset> </div> </body> </html>";

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
        String met="DHCP";
              if(getmethod=="1"){met="Manual";}
        getssidip = server.arg("ssidip");
        delay(90);
        getgateway = server.arg("gateway");
        delay(90);
        getidentikey = server.arg("identikey");
        delay(90);
        getmascara = server.arg("mascara");
        if(getmethod==0){getmascara="";}
        delay(90);
        getdns1 = server.arg("dns1");
        delay(90);
        getdns2 = server.arg("dns2");

        
        getssid = arregla(getssid);
        getpass = arregla(getpass);
        getmethod = arregla(getmethod);
        getssidip = arregla(getssidip);
        getgateway=arregla(getgateway);
        getidentikey = arregla(getidentikey);
        server.send(200, "text/html",  html1 + html2s+ getssid +html3s+ getpass + html4s+ met + html5s +getssidip +html6s + getgateway + html7s+ getmascara +html8s+getdns1 +html9s + getdns2 +html10s+getidentikey + html11s );
        
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
              display.setCursor(0, 30);
              display.println("IP est:"+getssidip);
              display.setCursor(0, 40);
              display.println("gateway:"+getgateway);
              display.setCursor(0, 50);
              display.println("mascara:"+getmascara);
              display.display();
              delay(2000);
              display.clearDisplay();
              display.setCursor(0, 30);
              display.println("DNS1:"+getdns1);
              display.setCursor(0, 40);
              display.println("DNS2:"+getdns2);
              display.setCursor(0, 50);
              display.println("identikey:"+getidentikey);
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
        graba(150,getidentikey);
        delay(10);
        graba(180,getmascara);
        delay(10);
        graba(210,getdns1);
        delay(10);
        graba(240,getdns2);
        delay(10);



    }

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


      ////////////Restaura variables guardada en EEPROM//////////////////


    void startVar() { 

        ssid_leido = lee(1);      
        pass_leido = lee(30);
        metodo_leido = lee(60);
        ssidip_leido = lee(90);
        gateway_leido= lee(120);
        identikey_leido = lee(150);
        mascara_leido=lee(180);
        dns1_leido=lee(210);
        dns2_leido=lee(240);


    }


    void conectarWifi(){
        startVar();
        
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("Leyendo datos");
        display.display();
        

        ssid_leido.toCharArray(ssid, ssid_leido.length() + 1);
        pass_leido.toCharArray(pass, pass_leido.length() + 1);
              
                    
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

          display.clearDisplay();

          display.setTextSize(1);
          display.setCursor(0, 30);
          display.println("usando metodo Manual"  );
          display.setCursor(0, 40);
          display.println("SSID:"+ ssid_leido );
          display.setCursor(0, 50);
          display.println("Password"+ pass_leido );
          display.display();
          delay(1500);
          display.clearDisplay();
          
          //WiFi.config(ip_static, ip_gateway, ip_subnet); 
           WiFi.config(ip_static, ip_gateway, ip_subnet,ip_dns1,ip_dns2); 
              //Intentamos conectar    
          }
          
          else if(metodo_leido=="0"){

          int cuenta = 0;
           display.setTextSize(1);
            display.setCursor(0, 30);
            display.println("usando metodo DHCP"  );
            display.setCursor(0, 40);
            display.println("SSID:"+ ssid_leido );
            display.setCursor(0, 50);
            display.println("Password:"+ pass_leido );
            display.display();

               //Intentamos conectar

          }
       

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
              delay(2000);
              display.clearDisplay();
              e=0;
              conectar=false;
              return;        
            }        
     }
        
        if (WiFi.status() == WL_CONNECTED) {

              conectar=true;
              display.clearDisplay();
              display.setTextSize(2);
              display.setCursor(0, 0);
              display.println("conectado a: " + ssid_leido);
              display.display();
              delay(1000);
              display.clearDisplay();
              
          }    
      }

    void setupWifiServer() {
        WiFi.mode(WIFI_AP);
        WiFi.softAP("Optiweather");
          
        IPAddress myIP = WiFi.softAPIP();

        server.on("/", root);
        server.on("/salvarDatos", salvarDatos);
        server.on ("/validacion", validacion);
        server.serveStatic("/css", SPIFFS, "/css", "max-age=86400");
        server.begin();
        conectar=false;
        
    }

    bool httpRequest() {
   
    }   



    void subir(){
        display.setTextSize(1);
        display.setCursor(120, 0);

        if((WiFiMulti.run() == WL_CONNECTED)) {
        
        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");

        http.begin("http://10.20.1.112:8080/setvalues.php?deviceid=123456789&valtemp="+String(t)+"&valhume="+String(h)); //HTTP

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
        } else {
            display.println("x");
        }

        http.end();
    }

    display.display();
    }


    void setup() {
        USE_SERIAL.begin(115200);
        EEPROM.begin(512); 
        SPIFFS.begin();
        Wire.begin(0, 2); //0 , 2 en esp y nodemcu. En wemos 5,4 
        pinMode(1, INPUT); 
        display.begin();  // initialize with the I2C addr 0x3D (for the 128x64)
        display.clearDisplay();
        testscrollopticalnetwork();
        display.clearDisplay();
        pinMode(5, OUTPUT);
        pinMode(4, OUTPUT);
        digitalWrite(5, HIGH);
        digitalWrite(4, LOW);


    }
    
    void loop() {
       
        if(!conectar){
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

          WiFi.mode(WIFI_STA);
          delay(1000);
          conectarWifi();
          delay(2000);
        }

        else{

            perdioconex=0;
            strHoraActual = "  Hora : " + hora();
            display.clearDisplay();
            bool estadoping = Ping.ping("www.google.com");
            int tiempoping = Ping.averageTime();//respuesta en milisegundos
            display.setTextSize(1);
            display.setCursor(80,0);
            
            if(estadoping){
            display.println(String(tiempoping));}

            
            temperaturaHumedadNoHora(); // Aqui se suben los datos
            
            delay(100);
            testopticalnetwork();



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
                }

           }

        }
    }









