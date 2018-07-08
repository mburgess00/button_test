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

unsigned long previousMillis = 0;
boolean timerRunning = false;
boolean currentGame = false;
int timer = 30;
int score = 0;
int highscore = 0;
int currentButton = 0; //none=0, A=1, B=2, C=3, D=4, reset=-1
int previousButton =0;
unsigned long presstime = 0;

const int eeAddress = 0;

boolean noDecimals[4] = {false, false, false, false};

void setup()
{
  Serial.begin(9600);
  Serial.println("Putter Scoreboard");

  pinMode(segmentClock, OUTPUT);
  pinMode(segmentData, OUTPUT);
  pinMode(segmentLatch, OUTPUT);

  digitalWrite(segmentClock, LOW);
  digitalWrite(segmentData, LOW);
  digitalWrite(segmentLatch, LOW);

  //remote receiver
  pinMode(aPin, INPUT);
  pinMode(bPin, INPUT);
  pinMode(cPin, INPUT);
  pinMode(dPin, INPUT);

  //buzzer
  pinMode(buzzer, OUTPUT);
  //digitalWrite(buzzer, HIGH);
  //delay(250);
  //digitalWrite(buzzer, LOW);
  tone(buzzer, 1200, 250);

  //reset
  pinMode(resetButton, INPUT_PULLUP);

  //read highscore from eeprom
  EEPROM.get(eeAddress, highscore);
  if ((highscore > 200) || (highscore < 0))
  {
    highscore = 0;
  }

  sendString("    ", noDecimals);
}


void loop()
{
  //handle buttons
  /*if (digitalRead(aPin) == HIGH)
  {
    postNumber('a', false);
  }
  if (digitalRead(bPin) == HIGH)
  {
    postNumber('b', false);
  }
  if (digitalRead(cPin) == HIGH)
  {
    postNumber('c', false);
  }
  if (digitalRead(dPin) == HIGH)
  {
    postNumber('d', false);
  }
  delay(250);*/

  char timer1 = ' ';
  char timer2 = ' ';
  int remainder = timer % 10;
  timer1 = char(remainder);
  int temptimer = timer / 10;
  timer2 = char(temptimer);

  char str[4] = "    ";

  if (digitalRead(aPin) == HIGH)
  {
    str[0] = ' ';
    str[1] = '3';
    str[2] = timer1;
    str[3] = timer2;
    sendString(str, noDecimals);
    tone(buzzer, 1000, 250);
    delay(750);
    str[1] = '2';
    sendString(str, noDecimals);
    tone(buzzer, 1000, 250);
    delay(750);
    str[1] = '1';
    sendString(str, noDecimals);
    tone(buzzer, 1000, 250);
    delay(750);
    str[0] = 'o';
    str[1] = 'g';
    sendString(str, noDecimals);
    tone(buzzer, 1200, 1000);
  }

  while (timer > 0)
  {
    delay(1000);
    timer--;
    remainder = timer % 10;
    timer1 = char(remainder);
    temptimer = timer / 10;
    timer2 = char(temptimer);
    str[2] = timer1;
    str[3] = timer2;
    sendString(str, noDecimals);
  }

  
}


void sendString(char string[4], boolean decimals[4])
{
  for (byte x = 0; x < 4; x++)
  {
    postChar(string[x], decimals[x]);
  }
  digitalWrite(segmentLatch, LOW);
  digitalWrite(segmentLatch, HIGH);  
}

//Given a number, or '-', shifts it out to the display
void postChar(char digit, boolean decimal)
{
  //    -  A
  //   / / F/B
  //    -  G
  //   / / E/C
  //    -. D/DP

#define a  1<<0
#define b  1<<6
#define c  1<<5
#define d  1<<4
#define e  1<<3
#define f  1<<1
#define g  1<<2
#define dp 1<<7

  Serial.print("sending the following character: ");
  Serial.println(digit);

  byte segments;

  switch (digit)
  {
    case '1': segments = b | c; break;
    case '2': segments = a | b | d | e | g; break;
    case '3': segments = a | b | c | d | g; break;
    case '4': segments = f | g | b | c; break;
    case '5': segments = a | f | g | c | d; break;
    case '6': segments = a | f | g | e | c | d; break;
    case '7': segments = a | b | c; break;
    case '8': segments = a | b | c | d | e | f | g; break;
    case '9': segments = a | b | c | d | f | g; break;
    case '0': segments = a | b | c | d | e | f; break;
    case ' ': segments = 0; break;
    case 'a': segments = a | b | c | e | f | g; break;
    case 'b': segments = c | d | e | f | g; break;
    case 'c': segments = g | e | d; break;
    case 'd': segments = b | c | d | e | g; break;
    case 'e': segments = a | d | e | f | g; break;
    case 'f': segments = a | e | f | g; break;
    case 'g': segments = a | b | c | d | f | g; break;
    case 'h': segments = c | e | f | g; break;
    case 'o': segments = a | b | f | g; break;
    case '-': segments = g; break;
    default: segments = 0;
  }

  if (decimal) segments |= dp;

  Serial.println(segments);

  //Clock these bits out to the drivers
  for (byte x = 0 ; x < 8 ; x++)
  {
    digitalWrite(segmentClock, LOW);
    digitalWrite(segmentData, segments & 1 << (7 - x));
    digitalWrite(segmentClock, HIGH); //Data transfers to the register on the rising edge of SRCK
  }

}

