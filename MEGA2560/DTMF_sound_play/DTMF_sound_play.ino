#include <Keypad.h>
#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin2048_int8.h>
#include <LiquidCrystal.h>
#include "SR04.h"
#define CONTROL_RATE 64
#define TRIG_PIN 10
#define ECHO_PIN 9
#define BUZZER_PIN 11

// メンブレンスイッチの行と列のピン設定
const byte ROWS = 4; // 行数
const byte COLS = 4; // 列数
// キーパッドのピン設定
byte rowPins[ROWS] = {9, 8, 7, 6}; // 行ピン
byte colPins[COLS] = {5, 4, 3, 2}; // 列ピン
// キーに対応する文字を格納
char keys[ROWS][COLS] = {
  {'A', 'B', 'C', 'D'},  // 1, 2, 3, A
  {'E', 'F', 'G', 'H'},  // 4, 5, 6, B
  {'I', 'J', 'K', 'L'},  // 7, 8, 9, C
  {'M', 'N', 'O', 'P'}   // *, 0, #, D
};
// キーに対応する文字を格納
char keymap[ROWS][ROWS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// DTMFマトリックス
int dtmf_low[4] = {697, 770, 852, 941};
int dtmf_high[4] = {1209, 1336, 1477, 1633};

// 合成音を担当するOscilインスタンス
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> lowOscil(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> highOscil(SIN2048_DATA);

//LCDディスプレイ設定
const int rs = 27, en = 26, d4 = 25, d5 = 24, d6 = 23, d7 = 22;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long a;

// 音を鳴らしているかどうか
bool isPlaying = false;
unsigned long playStartTime = 0;
const unsigned long playDuration = 300; // 音を鳴らす時間（ミリ秒）

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  startMozzi(CONTROL_RATE);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("DTMF sound sim");
  lcd.setCursor(0, 1);
  lcd.print("Push to play");
  Serial.begin(9600);
}

void loop() {
  audioHook();

  char key = keypad.getKey();
  if (key) {
    lcd.clear();
    int row = (key - 65) / 4;  // 行番号
    int col = (key - 65) % 4;  // 列番号
    printDTMF(row, col); 
    playDTMF(row, col);
  }

  // 指定時間が経過したら音を停止
  if (isPlaying && millis() - playStartTime > playDuration) {
    lowOscil.setFreq(0);  // 音を停止
    highOscil.setFreq(0);  // 音を停止
    isPlaying = false;  // 音を停止中フラグを設定
  }
}

// 対応する文字を表示
void printDTMF(int row, int col) {
  if (row >= 0 && row < 4 && col >= 0 && col < 4) {
    char t = keymap[row][col];
    lcd.setCursor(0, 0);
    lcd.print((String)t+" Row: "+row+" col: "+col);
  }
}

// 低周波数と高周波数を設定
void playDTMF(int row, int col) {
  int low = dtmf_low[row];
  int high = dtmf_high[col];
  lcd.setCursor(0, 1);
  lcd.print(String(low)+"Hz + "+high+"Hz");
  lowOscil.setFreq(low);
  highOscil.setFreq(high);
  isPlaying = true;  // 音を再生中フラグを設定
  playStartTime = millis();  // 音を鳴らし始めた時間を記録
}


int updateAudio() {
  // オシレータから生成した音をミックスして出力
  return (lowOscil.next() + highOscil.next()) / 2;
}

void updateControl() {}
