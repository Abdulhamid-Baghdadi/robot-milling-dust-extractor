#include <AccelStepper.h>
#include "definitions_esp32.h"

AccelStepper stepper(motorInterfaceType, MOTOR_STEP, MOTOR_DIR);   // 1 = Easy Driver interface
int setPosition = 28000;
int analogSignal = 0;
 
void setup()
{  
  Serial.begin(57600);
 
  pinMode(HOMING_SENSOR, INPUT_PULLUP);
  pinMode(MOTOR_ENABLE, OUTPUT);
  pinMode(MOTOR_DIR, OUTPUT);
  pinMode(MOTOR_STEP, OUTPUT);
  
  pinMode(BIG_WEIGHT, INPUT);
  pinMode(SMALL_WEIGHT, INPUT);
  
  digitalWrite(MOTOR_ENABLE, LOW);
  stepper.setMaxSpeed(25000); //100000
  stepper.setAcceleration(750000); //75000
  home();
}

void home() {
  long initial_homing = -1; // Used to Home Stepper at startup

  stepper.setCurrentPosition(0);  // Set the current position as zero for now
  while (digitalRead(HOMING_SENSOR) != LOW) {  // Make the Stepper move CCW until the switch is activated
    stepper.moveTo(initial_homing);  // Set the position to move to
    initial_homing--;  // Decrease by 1 for next move if needed
    stepper.run();  // Start moving the stepper
    delayMicroseconds(100); // 0.1
  }
  Serial.println("Homing Completed");
  Serial.println("");
  stepper.setCurrentPosition(0);  // Set the current position as zero for now
  initial_homing = 1;
}

void loop()
{  
  int newAnalogSignal = analogRead(ANALOG_SIGNAL);
  if(abs(newAnalogSignal - analogSignal) > 200) {
    analogSignal = newAnalogSignal;
  }
  //Serial.println("analogSignal=" + String(analogSignal));
  setPosition = map(analogSignal, 0, 4095, 5000, 28000);
  if(digitalRead(BIG_WEIGHT) == HIGH) {
    stepper.runToNewPosition(stepper.currentPosition()-4000); //30000
  } else if (digitalRead(SMALL_WEIGHT) == HIGH) {
    //Serial.println("chill");
  } else {
    stepper.moveTo(setPosition);
    if(stepper.distanceToGo() != 0) {
      stepper.run();
      //Serial.println("Run");;
    } else {
      Serial.println("Reached");;
    }
  }
}
