#include <IRremote.h>

#define ENABLE 5
#define DIRA 9
#define DIRB 10
#define RECV_PIN 11

const int SPEED_FAST = 255; // 速い
const int SPEED_MEDIUM = 180; // 中
const int SPEED_SLOW = 100; // 遅い

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  Serial.begin(9600);
  Serial.println("Enabling IRin");\
  irrecv.enableIRIn(); // 受信開始
  Serial.println("Enabled IRin");
}


void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    controlFan(results.value);
    irrecv.resume(); // 受信を待機
  }
    delay(100);
}


void controlFan(unsigned long command) {
  if (command == 0xFF30CF) { //ボタン1
    Serial.println("Speed: FAST");
    runFan(SPEED_FAST);
  }else if (command == 0xFF18E7) { //ボタン2
    Serial.println("Speed: MEDIUM");
    runFan(SPEED_MEDIUM);
  }else if (command == 0xFF7A85) { // ボタン3
    Serial.println("Speed: SLOW");
    runFan(SPEED_SLOW);
  }
}


void runFan(int speed) {
  analogWrite(ENABLE, speed);
  digitalWrite(DIRA, HIGH);
  digitalWrite(DIRB, LOW);

  delay(3000); //3秒間回転

  analogWrite(ENABLE, 0); //停止
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
}
