#ifndef SERVER_H
#define SERVER_H

#include "device_config.h"
#include "WiFi.h"
#include "ArduinoJson.h"

bool configMode;

const char* ssid = "network";
const char* password = "password";

WiFiServer server(80);

String header;
byte payload[JSON_SIZE];
StaticJsonDocument<JSON_SIZE> doc;

int IO_order[12]; // IO channel settings in order: xGim1, yGim1, xGim2, yGim2, pot1, pot2, bi_sw1, bi_sw2, tri_sw1, tri_sw2, but1, but2

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void runServer()
{
  server.begin();
  Serial.println("Starting Server");

  while (1)
  {
    if(digitalRead(configPin) == LOW)
      break;
    
    WiFiClient client = server.available();   // Listen for incoming clients
    
    if (client) {                             // If a new client connects,
      currentTime = millis();
      previousTime = currentTime;
      Serial.println("New Client.");          // print a message out in the serial port
      String currentLine = "";                // make a String to hold incoming data from the client
      while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
        currentTime = millis();
        if (client.available()) {             // if there's bytes to read from the client,
          char c = client.read();             // read a byte, then
          Serial.write(c);                    // print it out the serial monitor
          header += c;
          if (c == '\n') {
            if (currentLine.length() == 0) {
              for (int i = 0; i < JSON_SIZE; i++)
              {
                payload[i] = client.read();
                Serial.write(payload[i]);
                if (payload[i] == '\n')
                  break;
              }
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();

              if(payload[0] == 'C') // GO TO JOYSTICK CALIBRATION...
              {
                calibrate();
              }

              else 
              {
                deserializeJson(doc, payload); // CONFIG DATA IS LOADED TO GLOBAL ARRAY
                IO_order[0] = atoi(doc["xGim1"]);
                IO_order[1] = atoi(doc["yGim1"]);
                IO_order[2] = atoi(doc["xGim2"]);
                IO_order[3] = atoi(doc["yGim2"]);
                IO_order[4] = atoi(doc["pot1"]);
                IO_order[5] = atoi(doc["pot2"]);
                IO_order[6] = atoi(doc["bi_sw1"]);
                IO_order[7] = atoi(doc["bi_sw2"]);
                IO_order[8] = atoi(doc["tri_sw1"]);
                IO_order[9] = atoi(doc["tri_sw2"]);
                IO_order[10] = atoi(doc["but1"]);
                IO_order[11] = atoi(doc["but2"]);
              }
              
              break;
            } else { // if you got a newline, then clear currentLine
              currentLine = "";
            }
          } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
          }
        }
      }
       
      // Clear the header variable
      header = "";
      // Close the connection
      client.stop();
      Serial.println("Client disconnected.");
      Serial.println("");
    }
  }

  server.end();
}

#endif
