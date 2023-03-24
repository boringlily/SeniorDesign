#include <Arduino.h>
#include "uartETH.hpp"
int incomingByte = 0; // for incoming serial data
char data[30];
int size;

void setup() 
{
  // put your setup code here, to run once:
    Serial.begin(100000);
    // Serial1.begin(9600, SERIAL_8N1, UART_ETH_RX1, UART_ETH_TX1);
    delay(1000);
    uartETHinit();

    delay(1000);

}

void loop() 
{
  // send data only when you receive data:
  if (Serial1.available() > 0) 
  {
    // read the incoming byte:
    incomingByte = Serial1.read();
    if(incomingByte != 0)
    {
      Serial.printf("Received: [%d] \n", incomingByte);
    }
  }
  else
  {
    Serial.println("Serial1 not available");
  }
  delay(500);
}