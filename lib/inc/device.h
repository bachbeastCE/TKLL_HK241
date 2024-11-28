#include "global.h"

#include <iostream>

class Device
{
public:
   Device(uint8_t init_GPIO);
   ~Device();
   String getStatus();
   void turnON();
   void turnOFF();
   String getName();
   void setName(String new_name);
   String getMode();
   void setAutomatic();
   void setManual();
   uint32_t getDuration(); // DURATION: ACTIVE TIME?
   void setDuration(uint32_t new_duration);
   uint8_t getGPIO();
   void setGPIO(uint8_t new_GPIO); 
private:
   String name;
   String status; // STATUS: ON ? OFF
   String mode; //MODE: AUTOMATIC? MANUAL?
   uint32_t duration; // DURATION: ACTIVE TIME?
   uint8_t GPIO; // GPIO ON ESP32
};

class Motor : public Device{
   private:
   uint8_t speed; // Toc do tinh bang phan tram cua toc do dong co (%)
   public:
   Motor(uint8_t init_GPIO) : Device(init_GPIO) { 
      this->setName ("Motor"); 
      this->speed=100;
   }
   ~Motor ();
   uint8_t iShowSpeed();
   void setSpeed( uint8_t new_speed);
};

Device::Device(uint8_t init_GPIO) 
{
   name = "Device";
   duration = 0;
   GPIO = init_GPIO;
   status = "OFF";
   mode = "Auto";
   digitalWrite(init_GPIO, HIGH);
}
Device::~Device()
{

}

String Device::getStatus()
{
   return this->status;
}
void Device::turnON()
{
   this->status = "ON";
   digitalWrite(this->GPIO, LOW);
}
void Device::turnOFF()
{
   this->status = "OFF";
   digitalWrite(this->GPIO, HIGH);
}

String Device::getName()
{
   return this->name;
}
void Device::setName(String new_name)
{
   this->name = new_name;
}

String Device::getMode()
{
   return this->mode;
}
void Device::setAutomatic()
{
   this->mode = "Automatic";
}
void Device::setManual()
{
   this->mode = "Manual";
}

uint32_t Device::getDuration()
{
   return this->duration;
}
void Device::setDuration(uint32_t new_duration)
{
   this->duration = new_duration;
}

uint8_t Device::getGPIO()
{
   return this->GPIO;
}
void Device::setGPIO(uint8_t new_GPIO){
   this->GPIO = new_GPIO;
}

uint8_t Motor::iShowSpeed()
{
   return this->speed;
}
void Motor::setSpeed( uint8_t new_speed )
{
   this->speed = new_speed;
}

