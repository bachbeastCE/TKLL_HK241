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

Fan::Fan(uint8_t GPIO) : Relay(GPIO)
{
    this->channel = 0;
    this->speed = 0;
    this->freq = 5000;
}

void Fan::setSpeed(int speed)
{
    this->speed = constrain(speed, 0, 100); // Limit speed to 0-100%
    ledcWrite(this->channel, (this->speed * 255) / 100); // Map to 8-bit value
}

void Fan::speedControllerFan(float temperature)
{
    if (temperature < 25.0) {
        setSpeed(0); // Fan off
    } else if (temperature >= 25.0 && temperature < 30.0) {
        setSpeed(50); // Fan at 50% speed
    } else {
        setSpeed(100); // Fan at 100% speed
    }
}
