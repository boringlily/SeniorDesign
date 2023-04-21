/* 
    @author: Lily Moon

    @brief: ESP32 library for CH9121 uart serial to Ethernet converter.

    // Commands Datasheet for CH9121
    @link: https://www.waveshare.com/w/upload/e/ef/CH9121_SPCC.pdf 


*/

#include <Arduino.h>

// Pins

#define UART_ETH_RX1 0 // Replace with the actual pin number for UART_ETH_RX1
#define UART_ETH_TX1 1 // Replace with the actual pin number for UART_ETH_TX1
#define UART_ETH_CFG 10 // Replace with the actual pin number for UART_ETH_CFG


/* Config Command Begin */

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

/* Config Command End */

/* Functions */

void uartETHinit();
void configEnable(bool begin);
void sendSetConfig(byte command, uint8_t * parameters, size_t parameterCount);
void sendCommand(uint8_t command);
