#include <SPI.h>        //SPI.h must be included as DMD is written by SPI (the IDE complains otherwise)
#include <DMD.h>        //
#include <TimerOne.h>   //
#include "Arial_black_16.h"

#define irInput 2
#define irStatus digitalRead(irInput)
#define Away 1
#define Close 0

#define audioOutput 3
#define stopSound digitalWrite(audioOutput, LOW)
#define playSound digitalWrite(audioOutput, HIGH)

DMD dmd(1,1);
int time = 0;

void ScanDMD()
{
  dmd.scanDisplayBySPI();
}

void drawText()
{
  dmd.clearScreen(true);
  dmd.drawString(0,0," abcdefghijklm ",16,GRAPHICS_NORMAL);
  delay( 2000 );
}

void setup(void)
{
  Serial.begin(9600);
  pinMode(irInput, INPUT);
  pinMode(audioOutput, OUTPUT);

  Timer1.initialize( 5000 );           //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
  Timer1.attachInterrupt( ScanDMD );   //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()

  dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)
  dmd.selectFont(Arial_Black_16);

}



void loop(void)
{
  if (irStatus == Away)
  {
    Serial.println("Away"); // Отдалились

    playSound;
    delay(8000);
    stopSound;
    while ( irStatus == Away)
    {
    }
  } else {
    Serial.println("Close"); // Приблизились

    time++;
    dmd.clearScreen(true);
    String stringOne =  String(time);
    if(time > 99) {
        dmd.drawString(3,0,stringOne.c_str (),3,GRAPHICS_NORMAL);
    }
    else if(time > 9) {
        String stringTwo =  String("0" + stringOne);
        dmd.drawString(3,0,stringTwo.c_str (),3,GRAPHICS_NORMAL); 
    }
    else {
        String stringTwo =  String("00" + stringOne);
        dmd.drawString(3,0,stringTwo.c_str (),3,GRAPHICS_NORMAL);      
    }

    while ( irStatus == Close) //Ждем пока не отдалится
    {
    }
  }
}
