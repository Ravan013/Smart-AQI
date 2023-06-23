#include <MQ135.h>
#include <LiquidCrystal.h>
#include <DHT.h>
#include<SoftwareSerial.h>
SoftwareSerial comm(2, 3);

#define MQ135_PIN A0
#define RL_VALUE 47000.0
#define RO_CLEAN_AIR_FACTOR 9.83
#define DANGER_PPM 1000
#define BUZZER_PIN 8
#define DHT_PIN 7
#define DHT_TYPE DHT11

MQ135 mq135 = MQ135(MQ135_PIN, RL_VALUE, RO_CLEAN_AIR_FACTOR);
DHT dht(DHT_PIN, DHT_TYPE);

// Set up the LCD screen
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// variable to toggle between the data
int toggle = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();

  lcd.begin(16, 2);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  float resistance = mq135.getResistance();
  float apm = mq135.getPPM();
  int ppm = apm / 1000000;
  float voltage = (5.0 * resistance) / RL_VALUE;

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();



  if (toggle == 0) {
    Serial.print("Resistance: ");
    Serial.print(resistance);
    Serial.print(" Ohms\t");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Resistance: ");
    lcd.setCursor(0, 1);
    lcd.print(resistance);
    lcd.print(" Ohms\t");
    delay(1000);
    toggle++;
  }


  else if (toggle == 1) {
    Serial.print("PPM: ");
    Serial.print(ppm);
    Serial.print("\t");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CO2 level in PPM: ");
    lcd.setCursor(0, 1);
    lcd.print(ppm);
    lcd.print("\t");
    if (ppm >= DANGER_PPM) {
      digitalWrite(BUZZER_PIN, HIGH);
    }
    else {
      digitalWrite(BUZZER_PIN, LOW);
    }
    delay(1000);
    toggle++;
  }

  else if (toggle == 2) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("°C\t");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temperature: ");
    lcd.setCursor(0, 1);
    lcd.print(temperature);
    lcd.print("°C\t");
    delay(1000);
    toggle++;
  }

  else if (toggle == 3) {
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("%");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Humidity: ");
    lcd.setCursor(0, 1);
    lcd.print(humidity);
    lcd.print("%");
    delay(1000);
    toggle++;
  }

  else {
    Serial.print("Voltage: ");
    Serial.print(voltage);
    Serial.println(" V");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Voltage: ");
    lcd.setCursor(0, 1);
    lcd.print(voltage);
    lcd.print(" V");
    delay(1000);
    toggle = 0;
  }
}
