#define BLYNK_TEMPLATE_ID "TMPL6RDg-RYOD"
#define BLYNK_TEMPLATE_NAME "ESMART DHT11"
#define BLYNK_AUTH_TOKEN "UYW4HKPe-w44_FQ0P87piCGpHau0cgX0"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN D5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

unsigned long times = millis();

WidgetLED led(V0);
#define led_pin D3
BlynkTimer timer;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Địa chỉ I2C và kích thước màn hình (16x2)

void blinkLedWidget() {
  if (led.getValue()) {
    led.off();
  } else {
    led.on();
  }
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, "VuDan", "66668888", "blynk.cloud", 80);
  dht.begin();
  Wire.begin();  // Kích hoạt giao tiếp I2C
  pinMode(led_pin, OUTPUT);
  timer.setInterval(1000L, blinkLedWidget);
  lcd.begin(16, 2);  // Khởi động màn hình LCD
  lcd.backlight();   // Bật đèn nền
}

void loop() {
  Blynk.run();
  timer.run();

  if (millis() - times > 2000) {
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (isnan(h) || isnan(t)) {
      Serial.println(F("Connected"));
      return;
    }

    Blynk.virtualWrite(V1, t);
    Blynk.virtualWrite(V2, h);

    Serial.print(F("Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.println(F("%"));

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(t);
    lcd.print(" C");

    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(h);
    lcd.print("%");

    if (t > 2) {
      led.on();
      digitalWrite(led_pin, HIGH);
    } else {
      digitalWrite(led_pin, LOW);
      led.off();
    }

    times = millis();
  }
}