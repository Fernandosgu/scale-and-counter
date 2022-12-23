const char EOPmarker = '='; //This is the end of packet marker
char serialbuf[8]; 
#include <EEPROM.h>
#include <TimerOne.h>
#include <SoftwareSerial.h>//needed for serial read (sepciall case when using arduino uno)
#include <TM1637TinyDisplay6.h>// led display library
#include <string.h> // we'll need this for subString
#define MAX_STRING_LEN 20 // like 3 lines above, change as needed.
#define DIO    4  // pin 4
#define CLK    3  // pin 3


TM1637TinyDisplay6 display(CLK,DIO);
// the interger definition is for each character within the scale 
unsigned int a;
unsigned int b;
unsigned int c;
unsigned int d;
unsigned int e;
unsigned long millis();
int weight;

//display delay
unsigned long previousMillis = 0;  //will store last the display updated
const long period = 100;         // period at which to update display in ms

//substr setup
char* subStr (char* input_string, char *separator, int segment_number) {
  char *act, *sub, *ptr;
  static char copy[MAX_STRING_LEN];
  int i;
  strcpy(copy, input_string);
  for (i = 1, act = copy; i <= segment_number; i++, act = NULL) {
    sub = strtok_r(act, separator, &ptr);
    if (sub == NULL) break;
  }
 return sub;
}

void setup() 
{ 

  display.setBrightness(BRIGHT_HIGH);
  display.clear();
  Serial.begin(9600); 
  Serial2.begin(1200);// scale serial port is on serial port 2
  


  
} 
 
void loop() {
  readSerialC(); 
   weight=(d*1000)+(e*1000)+(c*100)+(b*10)+a;// this is to make the array from the character
   unsigned long currentMillis = millis(); // store the current time
  if (currentMillis - previousMillis >= period) { // check if 1000ms passed
   previousMillis = currentMillis;   // save the last time you blinked the LED
   display.showNumber((float)weight/1000.000, 3); 
  }
}
void readSerialC() { // 3 spaces 1 dots between values
  if (Serial2.available() > 0) { //makes sure something is ready to be read
      static int bufpos = 0; //starts the buffer back at the first position in the incoming serial.read
      char inchar = Serial2.read(); //assigns one byte (as serial.read()'s only input one byte at a time
      if (inchar != EOPmarker) { //if the incoming character is not the byte that is the incoming package ender
        serialbuf[bufpos] = inchar; //the buffer position in the array get assigned to the current read
        bufpos++; //once that has happend the buffer advances, doing this over and over again until the end of package marker is read.
      }
      else { //once the end of package marker has been read
        serialbuf[bufpos] = '\0'; //restart the buff// end of string
        bufpos = 0; //restart the position of the buff
         
         
        int p = atoi(subStr(serialbuf,"", 1));
        millis();
        int x = atoi(subStr(serialbuf, "", 1));
        millis();
        int y = atoi(subStr(serialbuf, "", 1));
        millis();
        int z = atoi(subStr(serialbuf, ".", 2));
        a=(p%1000/100);
        b=(x%100/10);
        c=(y%10);
        d=(z%100);
        e=(z/100);
         
      // Serial.print(a);
        //Serial.print(b);
       //Serial.println(c);
        Serial.println(d);
        // Serial.println(e);
         //Serial.println(z);
         Serial.println(serialbuf);

}}
}





 
  
