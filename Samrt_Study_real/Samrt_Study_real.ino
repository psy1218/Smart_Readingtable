#include "DHT.h" // 온습도 라이브러리
#include <Wire.h> // 날짜 라이브러리
#include <DS1302.h> // 날짜 라이브러리
#include <LiquidCrystal_I2C.h> // LCD 라이브러리
// 모드 변수
boolean laststyle = HIGH;
boolean currstyle = HIGH;
int stylecount = 0;
int style = A3;

// 스탠드 변수
int LEDbutton = 8;
int LED = 9;
int count = 0;
boolean last = HIGH;
boolean curr = HIGH;

// realtime 타이머 변수
LiquidCrystal_I2C lcd(0x27, 16, 2);
unsigned long mulai, selesai, dataStopWatch;
int i = 0;
int fPaus = 0;
long lastButton = 0;
long delayAntiBouncing = 50;
long dataPaus = 0;

// ht 온습도 변수
#define DHTPIN 7
#define DHTTYPE DHT11
#define HTLED 10
#define ERLED 11
DHT dht(DHTPIN, DHTTYPE);
long time = millis();

// LEDdistance 거리 측정 센서 변수
char Sensor = A2;
#define VOLTS_PER_UNIT .0049F
int Sensor_val;
float volts;
float inches;
float cm;

// daysAndTimer DS1302 모듈 포트
DS1302 rtc(2, 3, 4);

// 셋업
void setup() {
  Serial.begin(9600);
  // 모드 변경
  pinMode(style, INPUT_PULLUP);
  // 스탠드
  pinMode(LEDbutton, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  // 시간
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  //rtc.setDOW(TUESDAY);
  //rtc.setTime(22, 39, 00);
  //rtc.setDate(13, 12, 2022);
  // 온습도
  pinMode(HTLED, OUTPUT); // 온습도 측정
  pinMode(ERLED, OUTPUT); // 연결 오류 혹은 측정 범위 초과
  dht.begin();
  // 타이머
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  digitalWrite(A0, 1);
  digitalWrite(A1, 1);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" ARDUINO ");
  lcd.setCursor(0, 1);
  lcd.print(" STOPWATCH");
  delay(2000);
  lcd.clear();
  lcd.print(" study");
  lcd.setCursor(0, 1);
  lcd.print(" Start / Stop");
}
// 날짜 받는 데이터
void data() {
  lcd.setCursor(0, 0);
  lcd.print(rtc.getDOWStr());
  lcd.setCursor(3, 1);
  lcd.print(rtc.getDateStr());
}
// 날짜 프린터
void printTime() {
  lcd.setCursor(8, 0);
  lcd.print(rtc.getTimeStr());
}
void led_control(int mode) {

  if (mode == 1) {
    //  밝기 1단계 (제일 밝음)
    analogWrite(LED, 255);
  }
  else if (mode == 2) {
    //  밝기 2단계
    analogWrite(LED, 205);
  }
  else if (mode == 3) {
    //  밝기 3단계 (제일 어두움)
    analogWrite(LED, 155);
  }
  else if (mode == 0) {
    //  꺼짐
    digitalWrite(LED, LOW);
  }
}
// 거리 측정 함수
void ditance() {
  static unsigned long time = 0;
  Sensor_val = analogRead(Sensor);
  volts = (float)Sensor_val * VOLTS_PER_UNIT;
  inches = 23.897 * pow(volts, -1.1907);
  cm = 60.495 * pow(volts, -1.10 - 4);
  Serial.println(cm);
  if (3 < cm && cm  < 5) {
    if (time + 1000 < millis()) {  //0~1초동안
      digitalWrite(LED, HIGH) ;  //켜기
    }
    if (time + 2000 < millis()) {  //1~2초동안
      time = millis() ; //time초기화
      digitalWrite(LED, LOW) ;  //꺼라
    }
  }
}
// 모드 변경 함수 (타이머 , 날짜)
void mode_style(int stylecount) {

  if (stylecount == 1) { // 타이머
    static unsigned long prevtime = 0;
    if (prevtime + 1000 < millis()) {
      lcd.clear();
      data();
      printTime();
      Serial.println("daysAndTimer");
      prevtime = millis();
    }
  }
  else if (stylecount == 2) { // 타이머
    if (digitalRead(A0) == 0) {
      if ((millis() - lastButton) > delayAntiBouncing) {
        if (i == 0) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Start Timer");
          mulai = millis();
          fPaus = 0;
        }
        else if (i == 1) {
          lcd.setCursor(0, 0);
          lcd.print("Stop Timer ");
          dataPaus = dataStopWatch;
          fPaus = 1;
        }
        i = !i;
      }
      lastButton = millis();
    }
    else if (digitalRead(A1) == 0) {
      dataStopWatch = 0;
      dataPaus = 0;
      lcd.clear();
      lcd.print("Reset Stopwatch");
      lcd.setCursor(0, 1);
      lcd.print("0:0:0");
      delay(2000);
      lcd.clear();
      lcd.print(" Tekan Tombol");
      lcd.setCursor(0, 1);
      lcd.print(" Start / Stop");
    }
    if (i == 1) {
      selesai = millis();

      float jam, menit, detik;
      unsigned long over;
      // MATH time!!!
      dataStopWatch = selesai - mulai;
      dataStopWatch = dataPaus + dataStopWatch;
      jam = int(dataStopWatch / 3600000);
      over = dataStopWatch % 3600000;
      menit = int(over / 60000);
      over = over % 60000;
      detik = int(over / 1000);

      lcd.setCursor(0, 1);
      lcd.print(jam, 0);
      lcd.print(":");
      lcd.print(menit, 0);
      lcd.print(":");
      lcd.print(detik, 0);

      if (jam < 10) {
        lcd.print(" ");
      }
    }
  }
}

void loop() {
  //모드
  currstyle = digitalRead(style);
  mode_style(stylecount);
  if (laststyle == HIGH && currstyle == LOW) {
    stylecount++;
    if (stylecount == 3) {
      stylecount = 1;
    }
    laststyle = currstyle;
  }
  else if (laststyle == LOW && currstyle == HIGH) {
    laststyle = currstyle;
  }
  //습도
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("연결 오류"));
    digitalWrite(ERLED, HIGH);
    digitalWrite(HTLED, LOW);
    return;
  }
  else {
    digitalWrite(ERLED, LOW);
  }
  float hic = dht.computeHeatIndex(t, h, false);
  // 최적의 온습도
  if (h >= 70) {
    if (t < 15) {
      digitalWrite(HTLED, HIGH);
    }
    else {
      if (time + 1000 < millis()) {  //0~1초동안
        digitalWrite(10, HIGH) ;  //출력해라
      }
      if (time + 2000 < millis()) {  //1~2초동안
        time = millis() ; //time초기화
        digitalWrite(10, LOW) ;  //꺼라
      }
    }
  }
  if (h >= 60 && h < 70) {
    if (t >= 15 && t < 18) {
      digitalWrite(HTLED, HIGH);
    }
    else {
      if (time + 1000 < millis()) {  //0~1초동안
        digitalWrite(10, HIGH) ;  //출력해라
      }
      if (time + 2000 < millis()) {  //1~2초동안
        time = millis() ; //time초기화
        digitalWrite(10, LOW) ;  //꺼라
      }
    }
  }
  if (h >= 50 && h < 60) {
    if (t >= 18 && t < 21) {
       digitalWrite(HTLED, HIGH);
    }
    else {
      if (time + 1000 < millis()) {  //0~1초동안
        digitalWrite(10, HIGH) ;  //출력해라
      }
      if (time + 2000 < millis()) {  //1~2초동안
        time = millis() ; //time초기화
        digitalWrite(10, LOW) ;  //꺼라
      }
    }
  }
  if (h >= 40 && h < 50) {
    if (t >= 21 && t < 24) {
      digitalWrite(HTLED, HIGH);
    }
    else {
      if (time + 1000 < millis()) {  //0~1초동안
        digitalWrite(10, HIGH) ;  //출력해라
      }
      if (time + 2000 < millis()) {  //1~2초동안
        time = millis() ; //time초기화
        digitalWrite(10, LOW) ;  //꺼라
      }
    }
  }
  if (h < 40) {
    if (t >= 24) {
      digitalWrite(HTLED, HIGH);
    }
    else {
      if (time + 1000 < millis()) {  //0~1초동안
        digitalWrite(10, HIGH) ;  //출력해라
      }
      if (time + 2000 < millis()) {  //1~2초동안
        time = millis() ; //time초기화
        digitalWrite(10, LOW) ;  //꺼라
      }
    }
  }
  
  //스탠드 (밝기 조절)
  curr = digitalRead(LEDbutton);
  if (last == HIGH && curr == LOW) {
    count++;
    if (count == 4) {
      count = 0;
    }
    led_control(count);
    last = curr;
  }
  else if (last == LOW && curr == HIGH) {
    last = curr;
  }
  //거리 측정 센서 함수 호출
  ditance();
}
