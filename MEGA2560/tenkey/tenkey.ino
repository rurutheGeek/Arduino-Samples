#include  "Keyboard.h"

const int KEYIN[] = { 7, 8, 9};
const int KEYOUT[] = { 2, 3, 4};

unsigned long previousMillis = 0;
const long interval = 30;
int columnNum = 0;
int sw[3][3] = { 0 };
char keyMap[3][3] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8' , '9' }
  };

void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(KEYOUT[i], OUTPUT);
    digitalWrite(KEYOUT[i], LOW);
  }
  for (int i = 0; i < 3; i++) {
    pinMode(KEYIN[i], INPUT);
  }
  Keyboard.begin();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    digitalWrite(KEYOUT[columnNum], HIGH);

    for (int rowNum = 0; rowNum < 3; rowNum++) {
      int readValue = digitalRead(KEYIN[rowNum]);
      if (readValue != sw[columnNum][rowNum] && readValue == HIGH) {
        Keyboard.press(keyMap[columnNum][rowNum]);
      }

      sw[columnNum][rowNum] = readValue;
      delay(10);
      Keyboard.releaseAll();
    }

    digitalWrite(KEYOUT[columnNum], LOW);おｊぼお
    columnNum++;
    columnNum %= 3;     
  }
}