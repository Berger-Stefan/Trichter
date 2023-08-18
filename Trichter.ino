#include <Arduino.h>
#include <time.h>
#include "SevSeg.h"
#include <string>

// pinn defintions
#define BLUE_LED_PIN 4
#define YELLOW_LED_PIN 15
#define GREEN_LED_PIN 16
#define RED_LED_PIN 2

#define BUTTON_LEAVER_PIN 35
#define BUTTON_RESET_PIN 34

#define DIGIT_PINS {5,23,21,26}
#define SEGMENT_PINS {18,19,33,14,27,22,25,32}


enum state { Init, Ready, Start, Stop, End, Error, Reset};
 
// global variables
state current_state;
SevSeg sevseg; // 4 Seven Segment displays
const int led_pins[] = {YELLOW_LED_PIN, GREEN_LED_PIN, BLUE_LED_PIN, RED_LED_PIN};
const int pin_count = 4;

unsigned long start;
unsigned long end;
unsigned long duration;
float duration_float;

// function defintions
void start_timer();
void stop_timer();

void reset();

void set_all_lights_off();

void set_state_error();
void set_state_ready();
void set_state_init();
void set_state_start();
void set_state_stop();
void set_state_reset();


void setup() {

  Serial.begin(115200);

  // initalize all pins
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  
  pinMode(BUTTON_LEAVER_PIN, INPUT_PULLDOWN);
  pinMode(BUTTON_RESET_PIN, INPUT_PULLDOWN);

  set_state_init();


  // initialize all interput functions   
  attachInterrupt(digitalPinToInterrupt(BUTTON_LEAVER_PIN), start_timer, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_LEAVER_PIN), stop_timer, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_RESET_PIN), reset, RISING);

  // initalize the display
  byte numDigits     = 4;
  byte digitPins[]   = DIGIT_PINS;
  byte segmentPins[] = SEGMENT_PINS;
  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins);
  sevseg.blank();

  set_state_ready();
}

void loop() {
  sevseg.refreshDisplay();

  // update the timer value
  if(current_state == Start){
    duration = millis() - start;
    duration_float = float(duration)/1000.0;
    sevseg.setNumberF(duration_float, 2);
    Serial.println(duration_float);
  }
}

void start_timer(){
  set_state_start();
  
  start = millis();
}

void stop_timer(){
  set_state_stop();

  duration = millis() - start;
  duration_float = float(duration)/1000.0;
  sevseg.setNumberF(duration_float, 2);
}

void reset(){
  set_state_reset();
  Serial.print("Reseting Microcontroller!");
  delay(100);
  ESP.restart();
}

void all_lights_off(){
  for (int i = 0; i <= pin_count; i++){
    digitalWrite(led_pins[i], LOW);
  }
}

// state machine switching functions
void set_state_error(){
  current_state = Error;
  Serial.print("State set to \"Error\"  \n");
  sevseg.setChars("ERR.");
  all_lights_off();
  digitalWrite(led_pins[3],HIGH);
}

void set_state_ready(){
  current_state = Ready;
  Serial.print("State set to \"Ready\"  \n");
  sevseg.setChars("READY");
  all_lights_off();
  digitalWrite(led_pins[1],HIGH);
}

void set_state_init(){
  current_state = Init;
  Serial.print("State set to \"Init\"  \n");
  sevseg.setChars("INIT");
  all_lights_off();
  digitalWrite(led_pins[0],HIGH);
}

void set_state_start(){
  current_state = Start;
  Serial.print("State set to \"Start\"  \n");
  sevseg.setChars("STAR");
  all_lights_off();
  digitalWrite(led_pins[2],HIGH);
}

void set_state_stop(){
  current_state = Stop;
  Serial.print("State set to \"Stop\"  \n");
  sevseg.setChars("STOP");
  all_lights_off();
  digitalWrite(led_pins[1],HIGH);
}

void set_state_reset(){
  current_state = Reset;
  Serial.print("State set to \"Reset\"  \n");
  sevseg.setChars("RES.");
  all_lights_off();
  reset();
}
