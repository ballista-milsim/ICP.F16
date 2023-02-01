#include <Joystick.h>
#include <HID_Buttons.h>

const int ROW_COUNT = 5;
const int COLUMN_COUNT = 6;

const int KEYFREE = 0;
const int KEYDOWN = 1;
const int KEYUP = 2;
const int KEYHOLD = 3;

int rowPinsCount [] = {6, 5, 5, 5, 6};
int rowPins [] = {15, 14, 16, 6, 7};
int columnPins [] = {0, 1, 2, 3, 5, 4};

int keyStatus [ ROW_COUNT ][ COLUMN_COUNT ] = { };

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   31, // Button Count
                   0,                        // Hat Switch Count
                   true, true, false,        // No X, Y, or Z axes
                   true, true, false,      // No Rx, Ry, or Rz
                   false, false,             // No rudder or throttle
                   false, false, false);     // No accelerator, brake, or steering

int x = 0;
int y = 0;

void setup() {
  Serial.begin(9600);

  // Set all key status in matrix as FREE or not pressed
  for (int i = 0; i < ROW_COUNT; i++) {
    for (int j = 0; j < COLUMN_COUNT; j++) {
      keyStatus [ i ][ j ] = KEYFREE;
    }
  }
  // Configure pin column as an input and enable the internal pull-up resistor
  for (int i = 0; i < COLUMN_COUNT; i++) {
    pinMode(columnPins [i], INPUT_PULLUP);
  }
  // Configure all row pins as input
  for (int i = 0; i < ROW_COUNT; i++) {
    pinMode(rowPins [i], INPUT);
  }

  Joystick.begin();
  Joystick.setXAxisRange(0, 1024);
  Joystick.setYAxisRange(0, 1024);
  Joystick.setRxAxisRange(0, 1024);
  Joystick.setRyAxisRange(0, 1024);
}

void loop() {
  
  pinMode(21, INPUT);
  pinMode(20, INPUT);
  pinMode(19, INPUT);
  pinMode(18, INPUT);
  pinMode(10, INPUT);
  pinMode(9, INPUT);
  x = analogRead(9);
  //Serial.println(x);
  if (x > 750) {
    Joystick.setButton(0, true);
  } else if (x < 250) {
    Joystick.setButton(1, true);
  } else {
    Joystick.setButton(0, false);
    Joystick.setButton(1, false);
  }
  y = analogRead(10);
  //Serial.println(y);
  if (y > 750) {
    Joystick.setButton(2, true);
  } else if (y < 250) {
    Joystick.setButton(3, true);
  } else {
    Joystick.setButton(2, false);
    Joystick.setButton(3, false);
  }
  Joystick.setXAxis(analogRead(21));
  Joystick.setYAxis(map(analogRead(20), 0, 1024, 1024, 0));
  Joystick.setRxAxis(analogRead(19));
  Joystick.setRyAxis(map(analogRead(18), 0, 1024, 1024, 0));

  int currentButton = 4;
  for (int i = 0; i < ROW_COUNT; i++) {
    pinMode(rowPins [i], OUTPUT);
    digitalWrite(rowPins [i], LOW);
    for (int j = 0; j < COLUMN_COUNT; j++) {
      if (rowPinsCount[i] <= j) {
        Serial.println("Break");
        break;
      }
      int sensorVal = digitalRead(columnPins [j]);
      Serial.print(sensorVal);
      Serial.print(":");
      Serial.print(keyStatus[i][j]);
      Serial.println();
      if (sensorVal == LOW && keyStatus[i][j] == KEYFREE) {
        Serial.print((String)"Button " + currentButton + " :");
        Serial.println("KEYDOWN");
        keyStatus[i][j] = KEYDOWN;
        Joystick.setButton(currentButton, true);
      } else if (sensorVal == LOW && keyStatus[i][j] == KEYDOWN) {
        Serial.print((String)"Button " + currentButton + " :");
        Serial.println("KEYHOLD");
        keyStatus[i][j] = KEYHOLD;
        Joystick.setButton(currentButton, true);
      } else if (sensorVal == HIGH && (keyStatus[i][j] == KEYDOWN || keyStatus[i][j] == KEYHOLD)) {
        Serial.print((String)"Button " + currentButton + " :");
        Serial.println("KEYUP");
        keyStatus[i][j] = KEYUP;
        Joystick.setButton(currentButton, false);
      } else if (sensorVal == HIGH && keyStatus[i][j] == KEYUP) {
        Serial.print((String)"Button " + currentButton + " :");
        Serial.println("KEYFREE");
        keyStatus[i][j] = KEYFREE;
        Joystick.setButton(currentButton, false);
      }
      currentButton++;
    }
    pinMode(rowPins [i], INPUT);
  }
  delay(10);
}
