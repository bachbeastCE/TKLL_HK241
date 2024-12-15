#include "relay.h"

Relay::Relay(uint8_t GPIO)
{
   this->changeflag = 0;
   this->mode = 0; 
   this->status = 0;
   this->GPIO = GPIO;
   pinMode(this->GPIO, OUTPUT);
   digitalWrite(this->GPIO,HIGH);
}

void Relay::turnRelayON()
{
   this->status = 1;
   digitalWrite(this->GPIO,LOW);
   this->changeflag = 1;
}

void Relay::turnRelayOFF()
{
   this->status = 0;
   digitalWrite(this->GPIO,HIGH);
   this->changeflag = 1;
}

void Relay::toolgeRelay()
{
   if(this->status == 1){
      this->status = 0;
      digitalWrite(this->GPIO,HIGH);
   }
   else{
      this->status = 1;
      digitalWrite(this->GPIO,LOW);
   }
   this->changeflag = 1;
}


