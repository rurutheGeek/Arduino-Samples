int latchPin = 8;  // 74HC595のST_CPへ
int clockPin = 12; // 74HC595のSH_CPへ
int dataPin = 11;  // 74HC595のDSへ
int buttonPin = 2;
int state=0;
volatile unsigned long time_prev = 0, time_now;
unsigned long time_chat = 80;


int dataArray[] = {
  0b11111101, // 0
  0b01100001, // 1
  0b11011011, // 2
  0b11110011, // 3
  0b01100111, // 4
  0b10110111, // 5
  0b10111111, // 6
  0b11100001, // 7
  0b11111111, // 8
  0b11110111  // 9
};

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  attachInterrupt(
      digitalPinToInterrupt(buttonPin),
      stopDice,
      FALLING
    );
}

void loop() {
  if (state == 0) {
    byte number = random(0, 9); //ランダムに数字を選択
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, dataArray[number]);
    digitalWrite(latchPin, HIGH);
    delay(80);
  }
}

void stopDice() {
  time_now = millis();
  if( time_now-time_prev > time_chat)state = !state;
  time_prev = time_now;
}