// This Arduino behave as the main controller
// This device represents security device prototype

// Libraries
#include <LiquidCrystal.h>
#include <Servo.h>

// LCD configuration
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Servo configuration
Servo myServo;
int pos;

// Variables 
int digit;
String pass = "2468";
String input_pass = "";
char input_char;
char start = 'a'; // Trigger the system

void setup() {
 Serial.begin(9600);
 lcd.begin(16, 2);
 myServo.attach(6);
}

void loop() {
  // System begin
  digit = 0;
  int lcdCursor = 6;
    
  lcd.setCursor(3, 0);
  lcd.print("Welcome :)");
  lcd.setCursor(2 , 1);
  lcd.print("Press Any Key");
  
  if (Serial.available() > 0){
    Serial.print("Checkpoint 1");
    start = Serial.read();
    if (start != 'a') {	// Any key pressed
      lcd.setCursor (1, 0);
      lcd.print("Enter Your PIN");
      
      Serial.print("Checkpoint 2");
      while (digit < 4){ 
       input_char = Serial.read();
      
       if ((input_char != '*')&&(input_char != '#')){ // Input valid
        input_pass = input_pass + input_char;
        lcd.setCursor(lcdCursor, 1);
        lcd.print("*");
        lcdCursor += 1;
        digit += 1;
       }
      }
      
      // Password validation
      if (input_pass == pass) { // Password is true
        lcd.setCursor(3, 0);
        lcd.print("Unlocked");
        
        // Open the lock 
        for (pos = 0; pos <= 90; pos += 1){
          myServo.write(pos);
          delay(10);
        }
        
        // Press the KeyPad '*' or '#' to lock again
        char lock = Serial.read();
        if ((lock == '*')||(lock == '#')){
          for (pos = 90; pos >= 0; pos -= 1){
            myServo.write(pos);
            delay(10);
          }
        }
      }  
      else { // Password is false
        lcd.setCursor(1, 0);
        lcd.print("Wrong Password");
      }
    }
  }
  delay(2000);
}
 