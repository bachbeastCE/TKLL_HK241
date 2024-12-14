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

class Fan : public Relay
{
public:
    Fan(uint8_t GPIO);
    ~Fan(){};

    void setSpeed(int speed); // Set the speed of the fan
    void speedControllerFan(float temperature); // Adjust fan speed based on temperature
};
#endif