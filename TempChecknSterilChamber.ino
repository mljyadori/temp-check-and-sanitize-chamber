/***************************************
Kelompok 5 Praktikum Sistem Tertanam
***************************************/

// include the library code:
#include <Wire.h>
#include <LiquidCrystal.h>

// define Pin/Port:
#define tmp36Pin A0
#define doorPinA A1
#define doorPinB A2
#define doorPinC A3
#define doorPinD A4
#define sprayPin A5
#define trigPin 6
#define echoPin 5
#define notsafePin 4
#define safePin 3

// declare variables:
int tempVoltage, dataADC, celcius, distance;
unsigned int i, j;
volatile byte button = LOW;
long duration;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
  // put your setup code here, to run once:
  pinMode(tmp36Pin,INPUT);
  pinMode(echoPin,INPUT);
  pinMode(doorPinA,OUTPUT);
  pinMode(doorPinB,OUTPUT);
  pinMode(doorPinC,OUTPUT);
  pinMode(doorPinD,OUTPUT);
  pinMode(sprayPin,OUTPUT);
  pinMode(trigPin,OUTPUT);
  pinMode(notsafePin,OUTPUT);
  pinMode(safePin,OUTPUT);

  // active external interrupt:
  attachInterrupt(0,buttonPressed,FALLING);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16,2);
}

void openDoor()
{
  int caseNumber = 0;
  for(int a = 0; a < 2048; a++)
  {
   switch(caseNumber)
   {
    case 0:
    digitalWrite(doorPinA, HIGH);
    digitalWrite(doorPinB, LOW);
    digitalWrite(doorPinC, LOW);
    digitalWrite(doorPinD, LOW);
    break;
    case 1:
    digitalWrite(doorPinA, LOW);
    digitalWrite(doorPinB, HIGH);
    digitalWrite(doorPinC, LOW);
    digitalWrite(doorPinD, LOW);
    break;
    case 2:
    digitalWrite(doorPinA, LOW);
    digitalWrite(doorPinB, LOW);
    digitalWrite(doorPinC, HIGH);
    digitalWrite(doorPinD, LOW);
    break;
    case 3:
    digitalWrite(doorPinA, LOW);
    digitalWrite(doorPinB, LOW);
    digitalWrite(doorPinC, LOW);
    digitalWrite(doorPinD, HIGH);
    break;
   }

   caseNumber++;
   if(caseNumber > 3){caseNumber = 0;}
   delay(2);
  }
}

void closeDoor()
{
  int caseNumber = 0;
  for(int a = 0; a < 2048; a++)
  {
   switch(caseNumber)
   {
    case 0:
    digitalWrite(doorPinA, LOW);
    digitalWrite(doorPinB, LOW);
    digitalWrite(doorPinC, LOW);
    digitalWrite(doorPinD, HIGH);
    break;
    case 1:
    digitalWrite(doorPinA, LOW);
    digitalWrite(doorPinB, LOW);
    digitalWrite(doorPinC, HIGH);
    digitalWrite(doorPinD, LOW);
    break;
    case 2:
    digitalWrite(doorPinA, LOW);
    digitalWrite(doorPinB, HIGH);
    digitalWrite(doorPinC, LOW);
    digitalWrite(doorPinD, LOW);
    break;
    case 3:
    digitalWrite(doorPinA, HIGH);
    digitalWrite(doorPinB, LOW);
    digitalWrite(doorPinC, LOW);
    digitalWrite(doorPinD, LOW);
    break;
   }

   caseNumber++;
   if(caseNumber > 3){caseNumber = 0;}
   delay(2);
  }
}

void buttonPressed()
{
  button = !button;
}

void loop()
{
  // put your main code here, to run repeatedly:
  main:
  i=0;
  j=0;
  lcd.clear();
  digitalWrite(safePin,LOW);
  digitalWrite(notsafePin,LOW);
  while(button==HIGH)
  {
    loop1:
    // clears the trigPin condition
    digitalWrite(trigPin,LOW);
    delayMicroseconds(2);
    // sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(trigPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin,LOW);
    // reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin,HIGH);
    // calculating the distance
    distance = duration * 0.034 / 2 ; // Speed of sound wave divided by 2 (go and back)

    if(distance<=50)
    {
      lcd.setCursor(0,0);
      lcd.print("Distance: ");
      lcd.setCursor(10,0);
      lcd.print(distance);
      lcd.println(" cm  ");
      lcd.setCursor(0,1);
      lcd.print("Temperature: ... ");
      delay(5000);
      
      dataADC = analogRead(tmp36Pin);
      float tempVoltage = dataADC / 1024.0 * 5000.0;
      float celcius = ((tempVoltage - 500.0) / 10.0);
      lcd.setCursor(12,1);
      lcd.print(celcius);
      lcd.println(" C");
  
      if(celcius<37)
      {
        delay(2000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Safe!");
        digitalWrite(sprayPin,HIGH);
        digitalWrite(safePin,HIGH);
        delay(5000);
        digitalWrite(sprayPin,LOW);
        lcd.setCursor(0,0);
        lcd.print("Come In!");        
        for(i=0;i<=5;++i)
        {
         openDoor(); 
        }
        for(j=0;j<=5;++j)
        {
         closeDoor();
        }
        delay(3000);
        goto main;
      }
      else
      {
        delay(2000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Not Safe!");
        digitalWrite(notsafePin,HIGH);
        delay(2000);
        lcd.setCursor(0,0);
        lcd.print("Go Away!");
        delay(3000);        
        goto main;
      }
    }

    else
    {
      lcd.setCursor(0,0);
      lcd.print("Move Closer!");
      delay(1000);
      goto loop1;
    }
  }
}
