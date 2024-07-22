// Header files
#include "RichShieldTM1637.h"

// Define Macro
#define BUTTONK1 8
#define BUTTONK2 9
#define KNOB_PIN A0
#define CLK 10
#define DIO 11

// Global variable
int knobValue;
int knobArr[3];
int K1Counter;
int average;

// 
TM1637 disp(CLK,DIO);

// Function declaration
// void mainBlock();
void getValue();
int getAverage();
void setK1Counter();
void clearDisplay();

// Setup function
void setup(){
  pinMode(BUTTONK2, INPUT_PULLUP);
  pinMode(BUTTONK1, INPUT_PULLUP);
  Serial.begin(9600);
  disp.init();
}

// Loop function
void loop(){
  if(digitalRead(BUTTONK2) == 0){
    clearDisplay();
    while(digitalRead(BUTTONK2) == 0);
    //mainBlock();
    getValue();
    average = getAverage();
    disp.display(average);
  }
}

// Getting value from petitometer
void getValue(){
  K1Counter = 0;
  
  while(!K1Counter)
    setK1Counter();
  
  while(K1Counter){
    disp.display(analogRead(KNOB_PIN));
    delay(500);
    setK1Counter();
  }

  for(int i=0; i<4; i++){
    knobValue = analogRead(KNOB_PIN);
    knobArr[i] = knobValue;
    disp.clearDisplay();
    disp.display(i, 16);
    delay(1000);
  }
}

// Calculating average value from three value of petitometer
int getAverage(){
  int sum, avg;
  for(int i=0; i<3; i++)
    sum = knobArr[i];
  avg = sum/4;
  return avg;
}

// Button K1, opening the application and restarting it
void setK1Counter(){
  if(digitalRead(BUTTONK1) == 0){
    if(!K1Counter) K1Counter = 1;
    else K1Counter = 0;
  }
  while(digitalRead(BUTTONK1) == 0);
}

// Clearing out display in 4 digit segment
void clearDisplay(){
  for(int i=0; i<4; i++)
    disp.display(i, 16);
}