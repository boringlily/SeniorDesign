#include <Arduino.h>
#include "uartETH.hpp"

byte byteToSend = 0x32;

void setup() 
{
  // put your setup code here, to run once:
    Serial.begin(9600);
    delay(1000);
    uartETHinit();
    Serial.println("Begin Sending");
}

void loop()
{
  
    Serial1.write(0x32);

    Serial.printf("Sending: %d \n", 0x32);

  delay(500);
}