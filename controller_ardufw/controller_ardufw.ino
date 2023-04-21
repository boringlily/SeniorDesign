#include "includes.h"
#include "calibrate.h"
#include "server.h"
#include "sbus.h"
#include "oledMenu.h"
#include "runningAverage.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  initDisplay();
  pinMode(xGim1, INPUT);
  pinMode(yGim1, INPUT);
  pinMode(xGim2, INPUT);
  pinMode(yGim2, INPUT);
  pinMode(pot1, INPUT);
  pinMode(pot2, INPUT);

  pinMode(bi_sw1, INPUT_PULLUP);
  pinMode(tri_sw1_pole1, INPUT_PULLUP);
  pinMode(tri_sw1_pole2, INPUT_PULLUP);

  pinMode(bi_sw2, INPUT_PULLUP);
  pinMode(tri_sw2_pole1, INPUT_PULLUP);
  pinMode(tri_sw2_pole2, INPUT_PULLUP);

  pinMode(but1, INPUT_PULLUP);
  pinMode(but2, INPUT_PULLUP);
  
  pinMode(configPin, INPUT_PULLUP);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED);
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  initSBUS();  
}

void loop() {
  configMode = digitalRead(configPin);

  if(configMode)
  {
    // DISPLAY_CONFIG_SCREEN();    
    timerAlarmDisable(timer); //pause SBUS
    runServer();
    timerAlarmEnable(timer); //resume SBUS
  }

  else
  {
    // DISPLAY_FLIGHT_SCREEN();
    
    // PRODUCE ADJUSTED VALUES FOR SBUS, RANGES FROM ~170 - ~1800ish, not sure
    
    // UPDATE GIMBAL READ METHOD //
    data_.ch[IO_order[0]] = analogRead(xGim1)/2; // xGim1 
    data_.ch[IO_order[1]] = analogRead(yGim1)/2; // yGim1
    data_.ch[IO_order[2]] = analogRead(xGim2)/2; // xGim2
    data_.ch[IO_order[3]] = analogRead(yGim2)/2; // yGim2
    // UPDATE GIMBAL READ METHOD //
    
    data_.ch[IO_order[4]] = (analogRead(pot1)/2.33 + 95.6); // pot1 - MODIFY SO THAT EACH KNOB SCALES IN THE CORRECT DIRECTION
    data_.ch[IO_order[5]] = (analogRead(pot2)/2.33 + 95.6); // pot2
    
    data_.ch[IO_order[6]] = digitalRead(bi_sw1) ? 990 : 172; // bi_sw1
    data_.ch[IO_order[7]] = digitalRead(bi_sw2) ? 990 : 172; // bi_sw2
    data_.ch[IO_order[8]] = !digitalRead(tri_sw1_pole1) ? 1800 : !digitalRead(tri_sw1_pole2) ? 172 : 990; // tri_sw1 - PROBABLY NEEDS TO BE TWEAKED
    data_.ch[IO_order[9]] = !digitalRead(tri_sw2_pole1) ? 1800 : !digitalRead(tri_sw2_pole2) ? 172 : 990; // tri_sw2
    if (IO_order[10] != IO_order[11]) // Standard button operation
    {
      data_.ch[IO_order[10]] = !digitalRead(but1) ? 990 : 172; // but1
      data_.ch[IO_order[11]] = !digitalRead(but2) ? 990 : 172; // but2
    }
    else
    {
      data_.ch[IO_order[10]] = (!digitalRead(but1) && !digitalRead(but2)) ? 990 : 172; // logical AND button
    }
    sbus_to_buf(data_, buf); // FORMAT SBUS, TIMER INTERRUPTS TRIGGER SEND VIA UART TO PIN 1
  }  
}
