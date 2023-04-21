#ifndef CALIBRATE_H
#define CALIBRATE_H

#include "device_config.h"

void calibrate() // Calibration Routine... only runs one gimbal right now... needs full redo
{
  int xMin = 0;
  int xMax = 0;

  int yMin = 0;
  int yMax = 0;

  int xCur, yCur;
  int xPrev = 0;
  int yPrev = 0;
  int xPrevP = 0;
  int yPrevP = 0;

  Serial.println(" ");
  Serial.println("Gimbal Calibration");

  Serial.println("Bottom Left");  
  while (1)
  {
    xCur = analogRead(xGim1);

    if (abs(xCur - xPrevP) < 7) //hold within +/- 7 val for 3 seconds...
    {
      if(xCur > 2800) //should be atleast 2800
      {
        xMax = xCur;
        break; //move on
      }       
    }
    
    xPrev = xCur;
    xPrevP = xPrev;
    //Serial.println(xCur);
    delay(1000);
  }

  Serial.println("Bottom Right");  
  while (1)
  {
    yCur = analogRead(yGim1);

    if (abs(yCur - yPrevP) < 7) //hold within +/- 7 val for 3 seconds...
    {
      if(yCur > 2800) //should be at least 2800
      {
        yMax = yCur;
        break; //move on
      }       
    }
    
    yPrev = yCur;
    yPrevP = yPrev; //prev value for ref...
    //Serial.println(yCur);
    delay(1000);
  }

  Serial.println("Top Left");  
  while (1)
  {
    yCur = analogRead(yGim1);

    if (abs(yCur - yPrevP) < 7) //hold within +/- 7 val for 3 seconds...
    {
      if(yCur < 900) //should be under 900
      {
        yMin = yCur;
        break; //move on
      }       
    }
    
    yPrev = yCur;
    yPrevP = yPrev;
    //Serial.println(yCur);
    delay(1000);
  }

  Serial.println("Top Right");  
  while (1)
  {
    xCur = analogRead(xGim1);

    if (abs(xCur - xPrevP) < 7) //hold within +/- 7 val for 3 seconds...
    {
      if(xCur < 900) //should be under 900
      {
        xMin = xCur;
        break; //move on
      }       
    }
    
    xPrev = xCur;
    xPrevP = xPrev;
    //Serial.println(xCur);
    delay(1000);
  }

  Serial.println("xMin to xMax");
  Serial.print(xMin);
  Serial.print(" - ");
  Serial.println(xMax);

  Serial.println("yMin to yMax");
  Serial.print(yMin);
  Serial.print(" - ");
  Serial.println(yMax);
  
}


#endif
