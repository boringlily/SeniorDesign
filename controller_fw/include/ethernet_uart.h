/* 
    @author: Lily Moon

    @brief: ESP32 library for CH9121 uart serial to Ethernet converter.

    // Commands Datasheet for CH9121
    @link: https://www.waveshare.com/w/upload/e/ef/CH9121_SPCC.pdf 


*/

#include <Arduino.h>
#include "device_config.h"

/* Define Begin */

#define COMMAND_START 0x57 0xab 
#define GET_CHIP_VERSION 0x01
#define RESET_CHIP 0x02
#define GET_TCP1_STATUS 0x03
#define GET_TCP2_STATUS 0x04
#define SAVE_PARAMETERS 0x0d
#define SET_PARAMETERS 0x0e
#define SET_ETH1_MODE 0x10 // <param> : 0x00 TCP Server, 0x01 TCP Client, 0x02 UDP Server, 0x03 UDP Client
#define SET_DEVICE_IP 0x11
#define SET_SUBNET_MASK 0x12
#define SET_GATEWAY_IP 0x13
#define SET_ETH1_SOURCE_PORT 0x14 // 2 bytes, inverted byte position
#define SET_ETH1_DESTINATION_IP 0x15 
#define SET_ETH1_DESTINATION_PORT 0x16 // 2 bytes, inverted byte position
#define SET_ETH1_SERIAL_BAUD 0x21 // 4 bytes, inverted byte position
#define SET_ETH1_SERIAL_CALIBRATION 0x22 // 3 bytes: <stop bits>, <check>, <data length in bits>; 
    /*

    Available Values for <check>:
    0x00: Even Parity
    0x01: Odd Parity
    0x02: Mark 
    0x03: Space
    0x04: None
    */

#define SET_ETH1_SERIAL_TIMEOUT 0x23 
    /* 
    0x01 0x00 0x00 0x00
    (Serial timeout 1*5ms,
    after which four bytes
    need to be filled, and
    the space is filled with
    zeros) */

#define SET_ETH1_SERIAL_PACKET_LENGTH 0x25 
    /* 
    0x00 0x02 0x00 0x00
    (Packet length 2*256=512 bytes)
    */

#define GET_ETH_MAC_ADDR 0x81 // Returns 6 byte device MAC address
#define GET_ETH1_SERIAL_BAUD 0x71 // Returns 4 bytes 

/* Define End */


/* Functions */

void ethernetDefaultInit();
void configEnable(bool begin);
void sendSetConfig(byte command, uint8_t * parameters, size_t parameterCount);
void sendCommand(uint8_t command);

// Default Parameters
#define UART_ETH_RX1 RXD2
#define UART_ETH_TX1 TXD2
#define UART_ETH_CFG CFG2

  uint8_t ipAddr[4] = {0xc0, 0xa8, 0x01, 0x64};
  uint8_t subnetMask[4] = {0xff, 0xff, 0xff, 0x00};
  uint8_t gatewayAddr[4] = {0xc0, 0xa8, 0x01, 0x01};
  uint8_t sourcePort[2] = {0xd0, 0x07};                         // Port value is inverted Port: 2000 (0x07D0) should be sent as 0xD0, 0x07.
  uint8_t destinationPort[2] = {0xe8, 0x03};                    // Port value is inverted Port: 1000 (0x03E8)should be sent as 0xE8, 0x03.
  uint8_t destinationAddr[4] = {0xc0, 0xa8, 0x01, 0xc8};        
  uint8_t baudRate[4] = {0xA0,0x86,0x01,0x00};               // Baud Rate value is inverted
  uint8_t serialConfig[3] = {0x02,0x00,0x08};                   // 3 bytes: {<stop bits>, <parity>, <data length in bits>}
  uint8_t dataLength[4] = {0x00, 0x02, 0x00, 0x00};             // eg. 0x00 0x02 0x00 0x00 (Packing length 2*256=512 bytes).
  uint8_t mode[1] = {0x03};                                          // 0x00 TCP Server, 0x01 TCP Client, 0x02 UDP Server, 0x03 UDP Client
  uint8_t serialTimeout[4] = {0x01, 0x00, 0x00, 0x00,};


void ethernetDefaultInit()  
{
    pinMode(UART_ETH_CFG, OUTPUT);
    Serial.print("\nConfig Begin\n");
    Serial2.begin(9600, SERIAL_8E2, UART_ETH_RX1, UART_ETH_TX1);
    delay(1000);
    configEnable(true);
    
    sendSetConfig(SET_DEVICE_IP, ipAddr, 4);
    sendSetConfig(SET_ETH1_SOURCE_PORT, sourcePort, 2);
    sendSetConfig(SET_ETH1_DESTINATION_PORT, destinationPort, 2);
    sendSetConfig(SET_ETH1_DESTINATION_IP, destinationAddr, 4);
    sendSetConfig(SET_ETH1_MODE, mode, 1);
    sendSetConfig(SET_ETH1_SERIAL_CALIBRATION, serialConfig, 3);
    sendSetConfig(SET_ETH1_SERIAL_PACKET_LENGTH, dataLength, 4);
    sendSetConfig(SET_ETH1_SERIAL_BAUD, baudRate, 4);
    sendSetConfig(SET_ETH1_SERIAL_TIMEOUT, serialTimeout, 4);
    sendCommand(SAVE_PARAMETERS);
    sendCommand(SET_PARAMETERS);

    Serial.println("Config Complete");

    configEnable(false);
    delay(1000);
    Serial2.end();
    Serial2.begin(100000, SERIAL_8E2, UART_ETH_RX1, UART_ETH_TX1);
    delay(1000);
}

void configEnable(bool begin)
{
    if(begin)
    {
    // Set UART_ETH_CFG to LOW to enable config mode
    digitalWrite(UART_ETH_CFG, LOW);
    }
    else{
    // Set UART_ETH_CFG to HIGH to disable config mode
    digitalWrite(UART_ETH_CFG, HIGH);
    }
}

void sendSetConfig(byte command, uint8_t * parameters, size_t parameterCount) 
{
  // Send command and optional parameters
  Serial2.write(0x57);
  Serial2.write(0xAB);
  Serial2.write(command);
  for (int i = 0; i < parameterCount; i++) 
  {
    Serial2.write(parameters[i]);
  }

  // Wait for response
  while (Serial2.available() == 0) 
  {
    // Do nothing
  }
    // Read response
    uint8_t response = Serial2.read();

    if (response == (uint8_t)0xaa){
        // Command acknowledgment response
        Serial.printf("\nCommand: %d Response: %#04x\n Success\n", command, response);
    }
    else 
    {
        Serial.print("Command: ");
        Serial.print(command);
        Serial.println(" FAILED");
  }
  delay(500);
}

void sendCommand(uint8_t command)
{
 // Send command and optional parameters
  Serial2.write(0x57);
  Serial2.write(0xAB);
  Serial2.write(command);

  // Wait for response
  while (Serial2.available() == 0) 
  {
    // Do nothing
  }
  
    // Read response
    uint8_t response = Serial2.read();

    if (response == (uint8_t)0xaa){
        // Command acknowledgment response
        Serial.printf("\nCommand: %d Response: %#04x\n Success\n", command, response);
    }
    else 
    {
        Serial.print("Command: ");
        Serial.print(command);
        Serial.println(" FAILED");
  }
  delay(500);
}