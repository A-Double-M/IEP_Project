// Header files 
#include "RichShieldDHT.h"
#include "RichShieldTM1637.h"
#include <Wire.h> 
#include "RichShieldPassiveBuzzer.h" 

// Define Macro 
#define BUTTONK1 8 
#define BUTTONK2 9 
#define KNOB_PIN A0 
#define CLK 10 
#define DIO 11 
#define LED_RED 4  
#define LED_YELLOW 7  
#define LED_BLUE 6  
#define LED_GREEN 5  
#define PassiveBuzzerPin 3  
#define NOTE_M1 523

// Global variable 
int knobValue; 
int knobArr[4]; 
int K1Counter; 
int average; 

// Creating objects
TM1637 disp(CLK,DIO); 
PassiveBuzzer buz(PassiveBuzzerPin); 

// Function declaration 
void getValue(); 
int getAverage(); 
void setK1Counter(); 
void clearDisplay(); 
void ledDisplay(); 
void displayValue(); 
void buzzer(); 


// Setup function 
void setup(){
  pinMode(BUTTONK2, INPUT_PULLUP); 
  pinMode(BUTTONK1, INPUT_PULLUP); 
  Serial.begin(9600); 
  disp.init(); 
  pinMode(LED_RED, OUTPUT); 
  pinMode(LED_GREEN, OUTPUT); 
  pinMode(LED_BLUE, OUTPUT); 
  pinMode(LED_YELLOW, OUTPUT); 
} 


// Loop function 
void loop(){ 
  if(digitalRead(BUTTONK2) == 0){
    clearDisplay();
  while(digitalRead(BUTTONK2) == 0) ; 

  getValue(); 
  average = getAverage();
  ledDisplay(); 
  displayValue(average);
  buzzer(); 
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


// Set a counter to start selecting value, pressed again to finish selecting
void setK1Counter(){ 
  if(digitalRead(BUTTONK1) == 0){ 
    
    if(!K1Counter) K1Counter = 1; 
  else K1Counter = 0; 
  } 
  while(digitalRead(BUTTONK1) == 0); 
} 


// Calculating average value of petitometer 
int getAverage(){ 
  int sum=0, avg=0;
  for(int i=0; i<=3; i++) 
    sum += knobArr[i];

  avg = sum/4;
  return avg; 
} 


// Clearing out display in 4 digit segment and previously used LED
void clearDisplay(){ 
  for(int i=0; i<4; i++) 
    disp.display(i, 16); 
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_YELLOW, LOW);
} 


// Light up one LED based on average petitiometer value
void ledDisplay() { 
  Serial.println(average);
  
  if( average > 0 && average <=256 ) {   
   digitalWrite(LED_YELLOW, HIGH); 
  } 
  else{ 
    digitalWrite(LED_YELLOW,LOW); 
  } 

  if( average >=257 && average <=512 ) { 
    digitalWrite(LED_BLUE,HIGH);   
  } 
  else{ 
    digitalWrite(LED_BLUE,LOW); 
  } 

  if( average >=513 && average <=768 ) { 
    digitalWrite(LED_GREEN,HIGH);  
  } 
  else{ 
    digitalWrite(LED_GREEN,LOW); 
  } 

  if( average >=769 && average <=1023 ) { 
    digitalWrite(LED_RED,HIGH); 
  } 
  else{ 
    digitalWrite(LED_RED,LOW); 
  } 
}  


// Display value in 7-segment display with an local array
void displayValue(int average) {

  int8_t temp[4];
  
  if(average < 1000) temp[0] = INDEX_BLANK;
  else temp[0] = average/1000;
  average %= 1000;
  
  temp[1] = average / 100;
  average %= 100;
  
  temp[2] = average / 10;
  temp[3] = average % 10;   
  disp.display(temp);
} 


// Buzzer will buzz when value is less than 256
void buzzer() { 
  if(average<=256) 
    buz.playTone(NOTE_M1, 3000); 
} 
