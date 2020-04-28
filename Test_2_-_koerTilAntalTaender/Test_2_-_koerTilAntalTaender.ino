// Sensor
int sensor = A0;
bool state;
int count = 1;

// Motorer
#include <Stepper.h>

// change this to the number of steps on your motor
#define STEPS 200

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepper(STEPS, 13, 12, 11, 10);


// 
int i = 0;
int t = 0;

// Setup



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // set the speed of the motor to 30 RPMs
  stepper.setSpeed(15);
  Serial.begin(9600);

  if (analogRead(sensor) > 100){
    state = true;
  } else if (analogRead(sensor) <= 100){
    state = false;
  }
}

void counter() {
  int reading = analogRead(sensor);
  //Serial.println(read);

  if (reading > 100 && state == false){
    count++;
    state = true;
  } else if (reading <= 100 && state == true){
    count++;
    state = false;
  }
}

// Kører indtil at der er talt 10 tænder.
void koerIndtilAntalTaender(int x){
  Serial.println(analogRead(sensor));
  while (count < x){
    counter();
    stepper.step(1);
    Serial.println(count);
  }
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  koerIndtilAntalTaender(10)
}
