/*
 Name:		InternetDateTime.ino
 Created:	3/29/2020 8:04:04 PM
 Author:	Calfizzi Mario
*/
#if defined(ESP32)
  #include <wifi.h>
  #include <ESPmDNS.h>
  #include <WiFiMulti.h> 
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
  #include <ESP8266WiFiMulti.h> 
  //ESP8266WiFiMulti wifiMulti; 
#endif
#include <DNSServer.h>
#include <ipaddress.h>
#include <MCInternetDateTimeBy_worldtimeapi_org.h>

const byte  DNS_PORT = 53;
DNSServer   dnsServer;
char        HOSTNAME_s[]      = "INTERNET_TIME";
char        SSID_s[]          = "MY_SSID";
char        SSID_PASSWORD_s[] = "MY_SSID_PASSWORD";
IPAddress   DNSa(8,8,8,8);
IPAddress   DNSb(8,8,4,4);
#define     HOSTNAME         HOSTNAME_s
#define     SSID             SSID_s
#define     SSID_PASSWORD    SSID_PASSWORD_s
#define     DNS1 DNSa
#define     DNS2 DNSb
#
void connectAsAP()
{
  Serial.println("Access Point Intitialization...");
  IPAddress SelfIP(192,168,1,1);
  IPAddress SelfGateway(192,168,1,1);
  IPAddress SelfMask(255,255,255,0);
  WiFi.mode(WIFI_AP_STA);
  Serial.println("Mode setted");
  WiFi.persistent(true);
  Serial.println("persistent setted");
#ifdef ESP32
  WiFi.setHostname     ( HOSTNAME );
  Serial.println("Hostname setted");
#else
  WiFi.hostname     ( HOSTNAME );
#endif
  WiFi.softAPConfig(SelfIP, SelfGateway, SelfMask);   // subnet FF FF FF 00
  Serial.println("softAPConfig setted");
  boolean result  = WiFi.softAP       ( HOSTNAME);
  Serial.println("softAP setted");
  
  if(result == true)
  {
    Serial.println("Ready");
  }
  else
  {
    Serial.println("Failed!");
  }  
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP()); 

  if (!MDNS.begin    ( HOSTNAME )) 
  {
    Serial.println("Error setting up MDNS responder ( " + String(HOSTNAME) +  " )!");
  }else
    Serial.println("mDNS responder started ( " + String(HOSTNAME) +  " )");

}

void reconnectWiFi()
{
  static bool firstTime = true;
  static wl_status_t LastState = wl_status_t::WL_DISCONNECTED;
  static uint32_t Timeout = 30000; // sec
  static uint32_t ms = millis(); // sec
  //static MCTimer ReconnectionTry(30000); // 30 sec
  //if (!ReconnectionTry.IsStarted()) ReconnectionTry.Start();
  //Serial.println("Trying Connect...");
  delay(100);
  if (LastState == wl_status_t::WL_DISCONNECTED && WiFi.status() == WL_CONNECTED)
  {
    Serial.println('\n');
    Serial.println(F("###############################"));
    Serial.println("Connection established!");  
    Serial.println(F("###############################"));
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP()); 
    Serial.print("MAC address:\t");
    Serial.println(WiFi.macAddress()); 
#if defined(ESP32)
    WiFi.setHostname(HOSTNAME);
#elif defined(ESP8266)
    WiFi.hostname ( HOSTNAME);
#endif
    if (!MDNS.begin    ( HOSTNAME )) 
    {
      Serial.println("Error setting up MDNS responder ( " + String(HOSTNAME) +  " )!");
      //while (1) {
      //  delay(1000);
      //}
    }else
      Serial.println("mDNS responder started ( " + String(HOSTNAME) +  " )");

    Serial.println(F("###############################"));
    LastState = wl_status_t::WL_CONNECTED;
  }

  if (WiFi.status() != WL_CONNECTED )
  {
    LastState = wl_status_t::WL_DISCONNECTED;
    Serial.print(".");
    if (strlen(SSID)>0 && strlen(SSID_PASSWORD)>0 && (millis()- ms>Timeout || firstTime))
    {
      firstTime = false;
      Serial.print("Connecting to ");
      Serial.print(SSID); Serial.println(" ...");
#if defined(ESP32)
      WiFi.config(  IPAddress( 0, 0, 0, 0), 
                    IPAddress( 0, 0, 0, 0), 
                    IPAddress( 255, 255, 255, 0), 
                    DNS1,
                    DNS2);
#endif
      Serial.println("Begin");
      WiFi.begin(SSID, SSID_PASSWORD);
      Serial.println("setAutoReconnect");
      WiFi.setAutoReconnect(true);
      ms = millis();
    }
    yield();
  }
}


// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  delay (1000);
  connectAsAP();
  Serial.println("Connected AP");
  reconnectWiFi();
}

// the loop function runs over and over again until power down or reset
void loop() {
  static uint32_t InternetTimeMillis = 0;
  static uint32_t InternetTimeTimout = 60000LL; // 1 min
  reconnectWiFi();
  dnsServer.processNextRequest ();
  if (WiFi.isConnected() &&  
     (InternetTimeMillis == 0 || millis()-InternetTimeMillis>InternetTimeTimout || 
     (InternetTimeMillis>millis() && ((uint32_t)-1) -InternetTimeMillis + millis()>InternetTimeTimout)))
  {
    time_t t = MCInternetDateTime.GetTime();
    if (t != 0)
    {
      MCInternetDateTime.ToSerial();
    }
    InternetTimeMillis = millis();
  }
  yield();  
}

