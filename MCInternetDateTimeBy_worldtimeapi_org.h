/*
 Name:		MCInternetDateTimeBy_worldtimeapi_org.h
 Created:	3/29/2020 8:00:00 PM
 Author:	   Calfizzi Mario
*/
#pragma once
#if !defined(MCInternetDateTimeBy_worldtimeapi_org_h)
#define MCInternetDateTimeBy_worldtimeapi_org_h
#include "MCInternetDateTimeInterface.h"

class MCInternetDateTimeBy_worldtimeapi_org_Calss : public MCInternetDateTimeInterface
{
private: 
   String FindJsonKey ( String Data, String SearchFor );
public:
  time_t    GetTime   ( void );
  void      ToSerial  ( void );

};

extern MCInternetDateTimeBy_worldtimeapi_org_Calss MCInternetDateTimeBy_worldtimeapi_org_Instance;
#define MCInternetDateTime MCInternetDateTimeBy_worldtimeapi_org_Instance

#endif
