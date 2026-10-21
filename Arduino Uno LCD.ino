// Arduino #2 - Main controller
// Receives key presses from the keypad Arduino over Serial (RX),
// checks the PIN, shows status on the LCD and drives the servo lock.
//
// Controls:
//   - Any key on the welcome screen  -> start entering the PIN
//   - 0-9                            -> PIN digit
//   - '*' while entering the PIN     -> clear what was typed
//   - '*' or '#' while unlocked      -> lock the safe again

// Libraries
#include <LiquidCrystal.h>
#include <Servo.h>

// LCD configuration: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Servo configuration
Servo myServo;
const int SERVO_PIN = 6;
const int LOCKED_ANGLE = 0;
const int UNLOCKED_ANGLE = 90;

// PIN configuration
const String PASS = "2468";
const byte PIN_LENGTH = 4;

// Wrong-PIN protection
const byte MAX_ATTEMPTS = 3;
const unsigned long LOCKOUT_MS = 10000;   // 10 seconds
const unsigned long MESSAGE_MS = 1500;    // how long short messages stay on screen

// System states
enum State { WELCOME, ENTER_PIN, UNLOCKED };
State state = WELCOME;

String inputPass = "";
byte wrongAttempts = 0;

// ---------- Helpers ----------

// Read one character from Serial. Returns 0 if nothing has arrived yet.
char readKey() {
  if (Serial.available() > 0) {
    return (char)Serial.read();
  }
  return 0;
}

void showMessage(const char *line1, const char *line2) {
  lcd.clear();
  lcd.setCursor((16 - strlen(line1)) / 2, 0);  // center the text
  lcd.print(line1);
  lcd.setCursor((16 - strlen(line2)) / 2, 1);
  lcd.print(line2);
}

void showWelcome() {
  showMessage("Welcome :)", "Press Any Key");
  state = WELCOME;
}

void showEnterPin() {
  inputPass = "";
  showMessage("Enter Your PIN", "");
  lcd.setCursor(6, 1);
  state = ENTER_PIN;
}

void moveServo(int from, int to) {
  int step = (to > from) ? 1 : -1;
  for (int pos = from; pos != to + step; pos += step) {
    myServo.write(pos);
    delay(10);
  }
}

// Throw away keys pressed while a message was on screen
void flushSerial() {
  while (Serial.available() > 0) {
    Serial.read();
  }
}

// ---------- Main program ----------

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  myServo.attach(SERVO_PIN);
  myServo.write(LOCKED_ANGLE);  // make sure the safe starts locked
  showWelcome();
}

void loop() {
  char key = readKey();
  if (key == 0) {
    return;  // nothing pressed yet
  }

  switch (state) {

    case WELCOME:
      // Any key starts the PIN entry
      showEnterPin();
      break;

    case ENTER_PIN:
      if (key == '*') {
        // Clear what was typed so far
        showEnterPin();
      }
      else if (key >= '0' && key <= '9') {
        inputPass += key;
        lcd.print('*');

        if (inputPass.length() == PIN_LENGTH) {
          if (inputPass == PASS) {
            // Correct PIN -> open the lock
            wrongAttempts = 0;
            showMessage("Unlocked", "*/# to lock");
            moveServo(LOCKED_ANGLE, UNLOCKED_ANGLE);
            state = UNLOCKED;
          }
          else {
            // Wrong PIN
            wrongAttempts++;
            if (wrongAttempts >= MAX_ATTEMPTS) {
              showMessage("Too many tries", "Wait 10 sec");
              delay(LOCKOUT_MS);
              wrongAttempts = 0;
            }
            else {
              showMessage("Wrong Password", "");
              delay(MESSAGE_MS);
            }
            flushSerial();
            showWelcome();
          }
        }
      }
      // '#' is ignored while entering the PIN
      break;

    case UNLOCKED:
      if (key == '*' || key == '#') {
        // Lock the safe again
        moveServo(UNLOCKED_ANGLE, LOCKED_ANGLE);
        showMessage("Locked", "");
        delay(MESSAGE_MS);
        flushSerial();
        showWelcome();
      }
      break;
  }
}
