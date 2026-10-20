#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_1_DOUT_PIN = 8;
const int LOADCELL_1_SCK_PIN = 7;

// HX711 circuit wiring
const int LOADCELL_2_DOUT_PIN = 6;
const int LOADCELL_2_SCK_PIN = 5;

const int BIG_WEIGHT = 12;
const int SMALL_WEIGHT = 11;

#define SMALL_WEIGHT_THRESHOLD 40000
#define BIG_WEIGHT_THRESHOLD 100000

HX711 scale1;
HX711 scale2;

void setup() {
  pinMode(BIG_WEIGHT, OUTPUT);
  pinMode(SMALL_WEIGHT, OUTPUT);
  Serial.begin(57600);
  scale1.begin(LOADCELL_1_DOUT_PIN, LOADCELL_1_SCK_PIN, 32);
  scale2.begin(LOADCELL_2_DOUT_PIN, LOADCELL_2_SCK_PIN, 32);
  scale1.tare();
  scale2.tare();
}

void loop() {

  if (scale1.is_ready()) {
    Serial.println("-----");
    long reading1 = scale1.get_value(10);
    Serial.print("HX711 reading 1: ");
    Serial.println(reading1);

    long reading2 = -scale2.get_value(10);
    Serial.print("HX711 reading 2: ");
    Serial.println(reading2);
    Serial.println("-----");

    if(abs(reading1) > BIG_WEIGHT_THRESHOLD || abs(reading2) > BIG_WEIGHT_THRESHOLD) {
      Serial.println("Big Weight");
      digitalWrite(BIG_WEIGHT, HIGH);
      digitalWrite(SMALL_WEIGHT, LOW);
    } else if (abs(reading1) > SMALL_WEIGHT_THRESHOLD || abs(reading2) > SMALL_WEIGHT_THRESHOLD) {
      Serial.println("Small Weight");
      digitalWrite(BIG_WEIGHT, LOW);
      digitalWrite(SMALL_WEIGHT, HIGH);
    } else {
      Serial.println("No Weight");
      digitalWrite(BIG_WEIGHT, LOW);
      digitalWrite(SMALL_WEIGHT, LOW);
    }
    
  } else {
    Serial.println("HX711 not found.");
  }
  
}
