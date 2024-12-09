#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

// ピン設定
constexpr uint8_t RST_PIN = 5;
constexpr uint8_t SS_PIN = 53;
constexpr uint8_t LED_GREEN = 7;
constexpr uint8_t LED_RED = 8;
constexpr uint8_t SERVO_PIN = 9;
MFRC522 rfid(SS_PIN, RST_PIN);
Servo myServo;
// 状態管理
byte registeredCard[4] = {0};
bool isRegistered = false;
bool isOpen = false;
//サーボモーター角度
int posOpen = 90;
int posClose = 10;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  myServo.attach(SERVO_PIN);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  // 初期状態はCLOSE,赤点灯
  myServo.write(posClose); 
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, HIGH);

  Serial.println("Started");
}

void loop() {
  if ( ! rfid.PICC_IsNewCardPresent()||! rfid.PICC_ReadCardSerial())
    return;
  // 検出されたカードのUIDを取得
  byte currentCard[4];
  for (byte i = 0; i < 4; i++) {
    currentCard[i] = rfid.uid.uidByte[i];
  }
  //登録済みでなければ登録
  if (!isRegistered) {
    for (byte i = 0; i < 4; i++) {
      registeredCard[i] = currentCard[i];
    }
    isRegistered = true;
    Serial.println("Card registered");
  } else {
    if (memcmp(registeredCard, currentCard, 4) == 0) {
      toggleState();
    } else {
      Serial.println("Not Registered");
    }
  }
  // カード停止
  rfid.PICC_HaltA();
}

//サーボモーターの状態をトグル
void toggleState() {
  if (isOpen) {
    // CLOSE状態に変更
    myServo.write(posClose); //10度
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);
    Serial.println("State: CLOSE");
  } else {
    // OPEN状態に変更
    myServo.write(posOpen);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);
    Serial.println("State: OPEN");
  }
  isOpen = !isOpen;
}
