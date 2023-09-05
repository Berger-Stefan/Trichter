#include <Arduino.h>
#include <time.h>
#include <TM1637Display.h>

#define BUTTON_LEAVER_PIN 15
#define CLK 19
#define DIO 18

unsigned long start;
unsigned long end;
unsigned long duration;
float duration_float;

void timer();

enum state { Init, Ready, Start, Stop, End, Error, Reset};

state current_state;

TM1637Display display(CLK, DIO);

const uint8_t SEG_STOP[] = {
  SEG_A | SEG_C | SEG_D | SEG_F | SEG_F | SEG_G,   // S
  SEG_A | SEG_B | SEG_C,                           // T
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_A | SEG_B | SEG_E | SEG_F | SEG_G            // P
  };

//------------Setup-------------
void setup() {

  display.clear();
  display.setBrightness(7); // set the brightness to 7 (0:dimmest, 7:brightest)
  
  Serial.begin(115200);
  Serial.println("Setup");
  
  pinMode(BUTTON_LEAVER_PIN, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(BUTTON_LEAVER_PIN), timer, CHANGE);
  
  current_state = Ready;
  Serial.println("Ready");
  
}

//------------Loop-------------
void loop() {
  
  //Serial.println(digitalRead(BUTTON_LEAVER_PIN));
  
  if(current_state == Start){
    duration = millis() - start;
    duration_float = float(duration)/1000.0;
    Serial.print("counting: ");
    Serial.println(duration_float);
    display.showNumberDec(duration);
  }
  if (current_state == Stop){
    Serial.print("Stop! Time = ");
    Serial.println(duration_float);
    display.showNumberDec(duration);
  }

}

//------------Timer Interrupt-------------
void timer(){

  if (digitalRead(BUTTON_LEAVER_PIN) == HIGH){
    current_state = Start;
      
    start = millis();
  }
  if (digitalRead(BUTTON_LEAVER_PIN) == LOW){ 
    current_state = Stop;
  
    //duration = millis() - start;
    //if (duration>= 100){ //counteract switch bounce 
    //duration_float = float(duration)/1000.0;
    //}
  }
}
