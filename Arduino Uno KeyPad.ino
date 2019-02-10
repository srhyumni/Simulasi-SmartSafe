// This Arduino only used for input from KeyPad due to lack number of pins

// Libraries
#include <Keypad.h>

// KeyPad configuration
const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns

char myKey[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {8, 7, 6, 5}; 
byte colPins[COLS] = {4, 3, 2}; 

Keypad customKeypad = Keypad( makeKeymap(myKey), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
}
  
void loop(){
  	char inputKey = customKeypad.getKey();
  	Serial.print(inputKey);
    
  	if (inputKey){
    	Serial.write(inputKey);
  	}
  delay(1000);
}