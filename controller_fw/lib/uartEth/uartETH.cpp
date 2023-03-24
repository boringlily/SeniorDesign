#include <Arduino.h>
#include "uartETH.hpp"

#define UART_ETH_RX1 15 // Replace with the actual pin number for UART_ETH_RX1
#define UART_ETH_TX1 16 // Replace with the actual pin number for UART_ETH_TX1
#define UART_ETH_CFG 17 // Replace with the actual pin number for UART_ETH_CFG


  uint8_t ipAddr[4] = {0xc0, 0xa8, 0x01, 0x64};
  uint8_t subnetMask[4] = {0xff, 0xff, 0xff, 0x00};
  uint8_t gatewayAddr[4] = {0xc0, 0xa8, 0x01, 0x01};
  uint8_t sourcePort[2] = {0xd0, 0x07};                         // Port value is inverted Port: 2000 (0x07D0) should be sent as 0xD0, 0x07.
  uint8_t destinationPort[2] = {0xe8, 0x03};                    // Port value is inverted Port: 1000 (0x03E8)should be sent as 0xE8, 0x03.
  uint8_t destinationAddr[4] = {0xc0, 0xa8, 0x01, 0xc8};        
  uint8_t baudRate[4] = {0x80, 0x25, 0x00, 0x00};               // Baud Rate value is inverted
  uint8_t serialConfig[3] = {0x01,0x04,0x08};                   // 3 bytes: {<stop bits>, <parity>, <data length in bits>}
  uint8_t dataLength[4] = {0x00, 0x02, 0x00, 0x00};             // eg. 0x00 0x02 0x00 0x00 (Packing length 2*256=512 bytes).
  uint8_t mode[1] = {0x03};                                          // 0x00 TCP Server, 0x01 TCP Client, 0x02 UDP Server, 0x03 UDP Client
  uint8_t serialTimeout[4] = {0x01, 0x00, 0x00, 0x00,};

void uartETHinit() 
{
    pinMode(UART_ETH_CFG, OUTPUT);
    Serial.print("\nConfig Begin\n");
    Serial1.begin(9600, SERIAL_8N1, UART_ETH_RX1, UART_ETH_TX1);
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
  Serial1.write(0x57);
  Serial1.write(0xAB);
  Serial1.write(command);
  for (int i = 0; i < parameterCount; i++) 
  {
    Serial1.write(parameters[i]);
  }

  // Wait for response
  while (Serial1.available() == 0) 
  {
    // Do nothing
  }
  
    // Read response
    uint8_t response = Serial1.read();

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
  Serial1.write(0x57);
  Serial1.write(0xAB);
  Serial1.write(command);

  // Wait for response
  while (Serial1.available() == 0) 
  {
    // Do nothing
  }
  
    // Read response
    uint8_t response = Serial1.read();

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