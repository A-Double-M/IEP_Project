// Header files 
#include "RichShieldDHT.h"
#include "RichShieldTM1637.h" 

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
 
// Global variable 
int knobValue; 
int knobArr[4]; 
int K1Counter; 
int average; 

// Creating class for four digit display 
TM1637 disp(CLK,DIO); 

// Function declaration 
void getValue(); 
int getAverage(); 
void setK1Counter(); 
void clearDisplay(); 
void ledDisplay(); 
void displayValue();

// Setup function 
void setup(){
  pinMode(BUTTONK2, INPUT_PULLUP); 
  pinMode(BUTTONK1, INPUT_PULLUP); 
  Serial.begin(9600); 
  disp.init(); 
  pinMode(LED_RED,OUTPUT); 
  pinMode(LED_GREEN,OUTPUT); 
  pinMode(LED_BLUE,OUTPUT); 
  pinMode(LED_YELLOW,OUTPUT); 
} 

// Loop function 
void loop(){ 
  if(digitalRead(BUTTONK2) == 0){ 
    clearDisplay();
  while(digitalRead(BUTTONK2) == 0); 
    getValue(); 
  average = getAverage();
  displayValue(average); 
  ledDisplay(); 
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

  double sum = 0, avg = 0;

  for(int i=0; i<=3; i++) 
    sum += knobArr[i];

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
    pinMode(LED_RED, LOW);
    pinMode(LED_GREEN, LOW);
    pinMode(LED_BLUE, LOW);
    pinMode(LED_YELLOW, LOW);
} 

void ledDisplay() { 
  if( average >=0 && average <=256 ) { 
   Serial.println(average); 
   digitalWrite(LED_YELLOW,HIGH); 
   Serial.print("Hello"); 
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

void displayValue(int average){
  int8_t temp[4];
  
  if(average < 1000) temp[0] = INDEX_BLANK;
  else temp[0] = average/1000;
  average %= 1000;
  
  temp[1] = average / 100;
  average %= 100;
  
  temp[2] = average / 10;
  temp[3] = average % 10;   
  disp.display(temp);//disp object calls TM1637 class member function to display numbers stored in temp array
}