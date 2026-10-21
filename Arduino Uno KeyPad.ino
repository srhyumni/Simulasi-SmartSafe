// Arduino #1 - Keypad reader
// This Arduino is only used to read the keypad (the main controller
// doesn't have enough free pins). Every key press is sent as ONE
// character over Serial (TX) to the main controller (RX).

// Libraries
#include <Keypad.h>

// Keypad configuration (4 rows x 3 columns)
const byte ROWS = 4;
const byte COLS = 3;

char myKey[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {8, 7, 6, 5};
byte colPins[COLS] = {4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(myKey), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
}

void loop() {
  char inputKey = customKeypad.getKey();

  // getKey() returns NO_KEY (0) when nothing is pressed,
  // so only send a character when a key was actually pressed.
  if (inputKey != NO_KEY) {
    Serial.write(inputKey);
  }
  // No delay here: a delay would make the keypad miss key presses.
}
