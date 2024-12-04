#ifndef relay_h
#define relay_h

#include "Arduino.h"
#include "global.h"

#define MAX_RELAY 4

class Relay
{
public:
   uint8_t changeflag;
   uint8_t status; 
   uint8_t mode;
   int GPIO;
   Relay(uint8_t GPIO);
   ~Relay(){};      
   void turnRelayON();
   void turnRelayOFF();
   void toolgeRelay();
};
#endif