#include <DHT.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <RTClib.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define PIR2_PIN      41
#define PIR1_PIN      42
#define BUZZER        2
#define DHTPIN        16
#define SDA_PIN       21
#define SCL_PIN       20
#define DOOR0_PIN     0
#define DOOR1_PIN     1
#define RED_LED       12
#define YELLOW_LED    11
#define GREEN_LED     10

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

unsigned long lastDHT = 0;
const long dhtInterval = 2000;

int motionCount = 0;
int startMinute = -1;
const int TIME_WINDOW_MIN = 5;

Adafruit_MPU6050 mpu;

RTC_DS1307 rtc;

void setup() {
  pinMode(PIR1_PIN, INPUT);
  pinMode(PIR2_PIN, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(DOOR0_PIN, INPUT_PULLUP);
  pinMode(DOOR1_PIN, INPUT_PULLUP);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  Serial.begin(115200);
  while (!Serial);
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!mpu.begin(0x68, &Wire)) {
    Serial.println("Không tìm thấy MPU6050!");
    while (1) {
      delay(10);
    }
  }
  if (!rtc.begin()) {
    Serial.println("Không tìm thấy DS1307, kiểm tra dây nối!");
    while (1);
  }
  if (!rtc.isrunning()) {
    Serial.println("RTC chưa chạy, thiết lập thời gian...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();

  dht.begin();
}

float MPU6050Call(int i) {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* i = 1 => X
   * i = 2 => Y
   * i = 3 => Z
   */

  switch(i) {
    case 1:
      return a.acceleration.x;
    case 2:
      return a.acceleration.y;
    case 3:
      return a.acceleration.z;
    default:
      Serial.print("Accel X: ");
      Serial.print(a.acceleration.x);
      Serial.print(" m/s^2, Y: ");
      Serial.print(a.acceleration.y);
      Serial.print(", Z: ");
      Serial.println(a.acceleration.z);
      break;
  }
  return 0.0;
}

void DHT22Call() {
  unsigned long now = millis();
  if (now - lastDHT >= dhtInterval) {
    lastDHT = now;

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    Serial.print("Nhiet do: ");
    Serial.print(String(humidity, 2));
    Serial.print(" °C  |  Do am: ");
    Serial.print(String(temperature, 1));
    Serial.println(" %");
  }
}

bool PIRCall() {
  int motion1 = digitalRead(PIR1_PIN);
  int motion2 = digitalRead(PIR2_PIN);

  if (motion1 == HIGH || motion2 == HIGH) {
    Serial.println("Phat hien chuyen dong (PIR1 hoac PIR2)!");
    return true;
    // digitalWrite(BUZZER, HIGH);  
  } else {
    Serial.println("Khong co chuyen dong.");
    return false;
    // digitalWrite(BUZZER, LOW);
  }
}

DateTime RTCCall() {
  return rtc.now();
}

void DPrint(String text, int line) { 
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, line * 8);
  display.println(text);
  display.display();
}

void ClearScreen() {
  display.clearDisplay();
  display.display();
}

int PhyAct() {
  if ((digitalRead(DOOR0_PIN) == LOW) ||
      (digitalRead(DOOR1_PIN) == LOW)) {
    return 1;
  } else {
    return 0;
  }
}

void LEDStatus(int i) {
  switch (i) {
    case 2: digitalWrite(RED_LED, HIGH); break;
    case 1: digitalWrite(YELLOW_LED, HIGH); break;
    default: digitalWrite(GREEN_LED, HIGH); break;
  }
}

void LEDClean() {
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
}

void Algorithm() {
  bool motion = PIRCall();
  int door = PhyAct();
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  DateTime now = RTCCall();

  const float TEMP_THRESHOLD = 35.0;
  const float HUM_THRESHOLD  = 80.0;

  LEDClean();
  ClearScreen();

  if (door == 1) {
    if (motion) {
      if (motionCount == 0) {
        startMinute = now.minute();
      }
      motionCount++;

      int diff = (now.minute() - startMinute + 60) % 60;

      if (diff <= TIME_WINDOW_MIN && motionCount >= 3 &&
          (temp > TEMP_THRESHOLD || hum > HUM_THRESHOLD)) {
        Serial.println("Nguy hiem! Tre em bi bo quen trong xe!");
        LEDStatus(2);
        digitalWrite(BUZZER, HIGH);
        DPrint("!!! Tre bi bo quen !!!", 0);
        DPrint("Nhiet: " + String(temp) + "C", 1);
        DPrint("Do am: " + String(hum) + "%", 2);
        DPrint("!!! Co Tre Em !!!", 4);
      } else {
        Serial.println("Co tre trong xe. Dang theo doi...");
        LEDStatus(1);
        digitalWrite(BUZZER, LOW);
        DPrint("Theo doi tre em", 0);
        DPrint("Lan: " + String(motionCount), 1);
        DPrint("Nhiet: " + String(temp) + "C", 2);
        DPrint("Do am: " + String(hum) + "%", 3);
      }
    } else {
      int diff = (now.minute() - startMinute + 60) % 60;
      if (motionCount > 0 && diff > TIME_WINDOW_MIN) {
        motionCount = 0;
        startMinute = -1;
      }
      Serial.println("Xe an toan (khong co tre).");
      LEDStatus(3);
      digitalWrite(BUZZER, LOW);
      DPrint("Xe an toan", 0);
      DPrint("Nhiet: " + String(temp) + "C", 1);
      DPrint("Do am: " + String(hum) + "%", 2);
    }
  } else {
    motionCount = 0;
    startMinute = -1;
    Serial.println("Cua dang mo. Reset he thong.");
    LEDStatus(3);
    digitalWrite(BUZZER, LOW);
    DPrint("Cua mo", 0);
    DPrint("Nhiet: " + String(temp) + "C", 1);
    DPrint("Do am: " + String(hum) + "%", 2);
  }

  delay(1000);
}

void loop() {
  Algorithm();
}
