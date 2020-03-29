/*
 Name:		MCInternetDateTimeBy_worldtimeapi_org.cpp
 Created:	3/29/2020 8:00:00 PM
 Author:	   Calfizzi Mario
*/

#include "MCInternetDateTimeBy_worldtimeapi_org.h"
#include <WiFiClient.h>

#define DEBUG 0
#define LOG(value) if (DEBUG==1){ Serial.print (__func__);  Serial.print ("::"); Serial.print (#value);  Serial.print ("= "); Serial.println(value);}


MCInternetDateTimeBy_worldtimeapi_org_Calss MCInternetDateTimeBy_worldtimeapi_org_Instance;


String MCInternetDateTimeBy_worldtimeapi_org_Calss::FindJsonKey(String Data, String SearchFor)
{
  String SearchString ="\"" + SearchFor + "\":";
  int index = Data.lastIndexOf(SearchString);
  if(index>=0)
  { 
    int lastIndex = Data.indexOf(',', index);
    if (lastIndex <0)
      lastIndex = Data.indexOf('}', index);
    String ReturnData = Data.substring(index + SearchString.length(), lastIndex);
    LOG( ReturnData )
    return ReturnData ;
  }
  else 
    return "";
}

time_t MCInternetDateTimeBy_worldtimeapi_org_Calss::GetTime(void )
{
  String host("worldtimeapi.org");
  String URI("/api/ip");
  time_t t = 0;
  WiFiClient client;
  uint32_t ms = millis();
  LOG("GetTime...");
  while (!!!client.connect(host.c_str(), 80) && (millis()- ms) < 2000) {
    yield();
  }
  if ((millis()- ms) >= 2000)
  {
    LOG("connection failed!");
    return t;
  }
  client.write(("GET " + URI + " HTTP/1.1\r\n"  +
                "Host: " + host + "\r\n" +
                "Accept: application/json\r\n"
                //"Accept: text/html\r\n"
                //"Accept: text/plain\r\n" +
                "Accept-Encoding: gizip, deflate\r\n" +
                "User-Agent: ESP32\r\n"
                "Connection: keep-alive\r\n\r\n").c_str());
  while(!!!client.available()) {yield();}
  while(client.available())
  {
    String Data = client.readStringUntil('\n');
    if (DEBUG == 1)
      Serial.println(Data);
    this->Timezone =  FindJsonKey(Data, "timezone");
    this->Timezone.replace("\"","");
    String IsDayLightSavingString =  FindJsonKey(Data, "dst");
    if(IsDayLightSavingString!="")
    { 
      IsDayLightSaving = IsDayLightSavingString=="true";
    }

    String TimeZoneSecondsOffsetString =  FindJsonKey(Data, "dst_offset");
    if(TimeZoneSecondsOffsetString!="")
    { 
      TimeZoneOffsetInSeconds = atoll(TimeZoneSecondsOffsetString.c_str());
      TimeZoneOffsetInSeconds += IsDayLightSaving? 3600:0;
    }

    String TimeString =  FindJsonKey(Data, "unixtime");
    if(TimeString!="")
    { 
      this->UTCDateTime = (time_t)atoll(TimeString.c_str());
      t = (time_t)atoll(TimeString.c_str()) + TimeZoneOffsetInSeconds;
      this->DateTime = t;
      LOG( ctime(&t));
    }
  }
  client.stop();
  return t;
}

void MCInternetDateTimeBy_worldtimeapi_org_Calss::ToSerial( void )
{
  Serial.print    ( "Date Time:           "                 );
  Serial.print    ( ctime(&this->DateTime                   ));
  Serial.print    ( "UTC Date Time:       "                 );
  Serial.print    ( ctime(&this->UTCDateTime                ));
  Serial.print    ( "Timezone:            "                 );
  Serial.println  ( this->Timezone                           );
  Serial.print    ( "Timezone Offset:     "                 );
  Serial.println  ( this->TimeZoneOffsetInSeconds            );
  Serial.print    ( "Is Day Light Saving: "                 );
  Serial.println  ( this->IsDayLightSaving? "True":"False"  );
}
