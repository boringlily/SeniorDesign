#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define SCLpin 35
#define SDApin 36

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void OperationMode(void);
void ConfigModeUpload();
void ConfigModeComplete();
void GimbalConfigBR();
void GimbalConfigTR();
void  GimbalConfigTL();
void  GimbalConfigBL();

void initDisplay() 
{

  Wire.setPins(SDApin, SCLpin);
  Serial.begin(115200);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  OperationMode();
  ConfigModeUpload();
  ConfigModeComplete();
  GimbalConfigBR();
  GimbalConfigTR();
  GimbalConfigTL();
  GimbalConfigBL();
}

void OperationMode(void)
{
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println("Operation Mode");
  display.setCursor(0, 30);
  display.println("SBUS Enabled");
  display.display(); 
}

void ConfigModeUpload()
{
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println("Configuration Mode");
  display.setCursor(0, 30);
  display.println("Upload config JSON");
  display.display(); 
}

void ConfigModeComplete()
{
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println("Configuration Mode");
  display.setCursor(0, 30);
  display.println("JSON Received");
  display.display(); 
}

void GimbalConfigBR()
{
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println("Gimbal Configuration Mode (Bottom Right)");
  display.drawPixel(32, 40, WHITE);
  display.drawPixel(96, 40, WHITE);
  display.drawCircle(32, 40, 15, WHITE);
  display.drawCircle(96, 40, 15, WHITE);
  display.drawCircle(43, 50, 2, WHITE);
  display.drawCircle(107, 50, 2, WHITE);
  display.display();
}

void GimbalConfigTR()
{
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println("Gimbal Configuration Mode (Top Right)");
  display.drawPixel(32, 40, WHITE);
  display.drawPixel(96, 40, WHITE);
  display.drawCircle(32, 40, 15, WHITE);
  display.drawCircle(96, 40, 15, WHITE);
  display.drawCircle(43, 30, 2, WHITE);
  display.drawCircle(107, 30, 2, WHITE);
  display.display();
}

void GimbalConfigTL()
{
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println("Gimbal Configuration Mode (Top Left)");
  display.drawPixel(32, 40, WHITE);
  display.drawPixel(96, 40, WHITE);
  display.drawCircle(32, 40, 15, WHITE);
  display.drawCircle(96, 40, 15, WHITE);
  display.drawCircle(21, 30, 2, WHITE);
  display.drawCircle(85, 30, 2, WHITE);
  display.display();
}

void GimbalConfigBL()
{
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println("Gimbal Configuration Mode (Bottom Left)");
  display.drawPixel(32, 40, WHITE);
  display.drawPixel(96, 40, WHITE);
  display.drawCircle(32, 40, 15, WHITE);
  display.drawCircle(96, 40, 15, WHITE);
  display.drawCircle(21, 50, 2, WHITE);
  display.drawCircle(85, 50, 2, WHITE);
  display.display();
}