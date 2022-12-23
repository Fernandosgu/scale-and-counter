#include<Arduino.h>
#include <TM1637Display.h>
#define CLK 2 // diplay pin
#define DIO 3 //display pin
TM1637Display display(CLK,DIO);
#define debounce_delay 100
int irPin = 7;
int count = -1;
bool bPress = false;  
const int IncbuttonPin = 11;
const int DecbuttonPin = 9;
const int ResetbuttonPin = 10;
boolean prev_state = true;
int current_number = 0;
int buttonPushCounter = 0;   // counter for the number of button presses

int IncbuttonState = 0;         // current state of the button
int lastIncbuttonState = 0;     // previous state of the button
int ResetbuttonState = 0;
int lastResetbuttonstate = 0;
int DecbuttonState = 0;         // current state of the button
int lastDecbuttonState = 0;

void setup() {
  Serial.begin(9600);
  pinMode(irPin, INPUT);
pinMode( IncbuttonPin , INPUT_PULLUP );
  pinMode( DecbuttonPin , INPUT_PULLUP );
 pinMode( ResetbuttonPin, INPUT_PULLUP);
display.setBrightness(0x0f);
}


void loop () {
  bool state = digitalRead(irPin);
  if (state != prev_state) {
    if (state) {
      count++;
      display.showNumberDec(count,true);
      Serial.print("Count : ");
      Serial.println(count);
      delay(debounce_delay);
      
    }
    prev_state = state;
  }
  IncbuttonState = digitalRead(IncbuttonPin);
   DecbuttonState = digitalRead(DecbuttonPin);
   ResetbuttonState= digitalRead(ResetbuttonPin);
   checkIncButtonPress();
   checkDecButtonPress();
   ResetButtonPress();
    if( bPress ){
    bPress = false;
     delay(50);
    }
    display.showNumberDec(count, true);
}

void checkIncButtonPress()
{
   // compare the IncbuttonState to its previous state
  if (IncbuttonState != lastIncbuttonState) {
    // if the state has changed, increment the counter
    if (IncbuttonState == HIGH ) {
      // if the current state is HIGH then the button went from off to on:
    //  bPress = true;
      count++;
      if( buttonPushCounter > 9999) buttonPushCounter = 0 ;
      Serial.println("on");
      
    } else {
      // if the current state is LOW then the button went from on to off:
      Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    delay(10);
  }
  // save the current state as the last state, for next time through the loop
  lastIncbuttonState = IncbuttonState;

}
void checkDecButtonPress()
{
   // compare the IncbuttonState to its previous state
  if (DecbuttonState != lastDecbuttonState) {
    // if the state has changed, increment the counter
    if (DecbuttonState == LOW) {
      // if the current state is HIGH then the button went from off to on:
      bPress = true;
      count--;
      if( count < 0) count = 9999 ;
     Serial.println("on");
      
    } else {
      // if the current state is LOW then the button went from on to off:
    //  Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    delay(10);
  }
  // save the current state as the last state, for next time through the loop
  lastDecbuttonState = DecbuttonState;
}
void ResetButtonPress(){
 if (ResetbuttonState != lastResetbuttonstate) {
   bPress = true;
    if(count>= 1) {
     count=0;
}
delay(10);
  }
  lastResetbuttonstate= ResetbuttonState;
}
