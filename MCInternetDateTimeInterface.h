/*
 Name:		MCInternetDateTimeInterface.h
 Created:	3/29/2020 8:04:04 PM
 Author:	Calfizzi Mario
*/
#pragma once
#if !defined(MCInternetDateTimeInterface_h)
#define MCInternetDateTimeInterface_h

#include <Arduino.h>
#include <string.h>

class MCInternetDateTimeInterface
{
private: 
   virtual String     FindJsonKey(String Data, String SearchFor) = 0;
public:
  bool                IsDayLightSaving = false;
  uint32_t            TimeZoneOffsetInSeconds = 0;
  time_t              UTCDateTime  = 0;
  time_t              DateTime = 0;
  String              Timezone; 
public:
  virtual   time_t    GetTime ( void ) = 0;
  virtual   void      ToSerial( void ) = 0;

};
#endif

