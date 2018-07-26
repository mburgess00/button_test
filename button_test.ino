/*
 Arduino pin 6 -> CLK (Green on the 6-pin cable)
 5 -> LAT (Blue)
 7 -> SER on the IN side (Yellow)
 5V -> 5V (Orange)
 Power Arduino with 12V and connect to Vin -> 12V (Red)
 GND -> GND (Black)
 There are two connectors on the Large Digit Driver. 'IN' is the input side that should be connected to
 your microcontroller (the Arduino). 'OUT' is the output side that should be connected to the 'IN' of addtional
 digits.
 Each display will use about 150mA with all segments and decimal point on.
*/

#include <EEPROM.h>

//GPIO declarations
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
byte segmentClock = 6;
byte segmentLatch = 5;
byte segmentData = 7;

//Remote receiver
byte aPin = 13;
byte bPin = 12;
byte cPin = 11;
byte dPin = 10;

//Buzzer
byte buzzer = 8;

//Reset button
byte resetButton = 9;
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


const int eeAddress = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Button Test!!!");

  //remote receiver
  pinMode(aPin, INPUT_PULLUP);
  pinMode(bPin, INPUT_PULLUP);
  pinMode(cPin, INPUT_PULLUP);
  pinMode(dPin, INPUT_PULLUP);

}


void loop()
{
  //handle buttons
  if (digitalRead(aPin) == LOW)
  {
    Serial.println("A is being pressed");
  }
  if (digitalRead(bPin) == LOW)
  {
    Serial.println("B is being pressed");
  }
  if (digitalRead(cPin) == LOW)
  {
    Serial.println("C is being pressed");
  }
  if (digitalRead(dPin) == LOW)
  {
    Serial.println("D is being pressed");
  }

  delay(100);
  
}


