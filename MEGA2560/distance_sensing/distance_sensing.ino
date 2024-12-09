#include <LiquidCrystal.h>
#include "SR04.h"
#define TRIG_PIN 10
#define ECHO_PIN 9
#define LED_PIN 8
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long a;


void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  delay(1000);
}

void loop() {
  a=sr04.Distance();
  lcd.clear();

  if(a<=10){
    lcd.setCursor(0, 1);
    lcd.print("Too close!");
    digitalWrite(LED_PIN, HIGH);
  }else  digitalWrite(LED_PIN, LOW);

  lcd.setCursor(0, 0);
  lcd.print(String(a)+"cm");
  Serial.print(a);
  Serial.println("cm");
  delay(1000);
}
