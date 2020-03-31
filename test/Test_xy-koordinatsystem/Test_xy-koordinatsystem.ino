// Include the Arduino Stepper Library
#include <Stepper.h>

// Number of steps per output rotation
const int stepsPerRevolution = 200;
const int steps = 200;

// Create Instance of Stepper library
Stepper myStepperB(stepsPerRevolution, 4, 5, 6, 7);
Stepper myStepperA(stepsPerRevolution, 8, 9, 10, 11);

float vectorStepLengthB;
float vectorStepLengthA;

// Arrays for coordinate system
float A[] = {0.0, 0.0};
float  B[] = {75.5, 0.0};

// Actual length of each string
float v = 50.0;
float h = 50.0;

// Wanted position
float x = 30.0;
float y = 50.0;



// One step
float stringLengthOneRev = 4.5/200.0;

// Calculate the steps for the given position. 
float vStepsEqualLength = v/stringLengthOneRev;
float hStepsEqualLength = h/stringLengthOneRev;


float stepsRequiredToReachPosA;
float stepsRequiredToReachPosB;


void setup()
{
  // set the speed at 60 rpm:
  myStepperB.setSpeed(100);
  myStepperA.setSpeed(100);
  // initialize the serial port:
  Serial.begin(9600);
}


void calculateLength(){
  float vectorCoordinatesA[] = {A[0] - x, A[1] - y};
  float vectorCoordinatesB[] = {B[0] - x, B[1] - y};

  float vectorLengthA = sqrt(pow(vectorCoordinatesA[0], 2) + pow(vectorCoordinatesA[1], 2));
  float vectorLengthB = sqrt(pow(vectorCoordinatesB[0], 2) + pow(vectorCoordinatesB[1], 2));

  // 
   vectorStepLengthA = vectorLengthA/stringLengthOneRev;
   vectorStepLengthB = vectorLengthB/stringLengthOneRev;

  // Steps required to reach the wanted position by subtracting the steps equal to the length of the string. 
  stepsRequiredToReachPosA = vectorStepLengthA - vStepsEqualLength;
  stepsRequiredToReachPosB = vectorStepLengthB - hStepsEqualLength;
  Serial.println("Snorlængder");
  Serial.println(vectorLengthA);
  Serial.println(vectorLengthB);
  Serial.println(vectorStepLengthA);
  Serial.println(vectorStepLengthB);
  
  Serial.println("Calculate length udført");
}




void driveToPos(){
  calculateLength();
  myStepperB.step(stepsRequiredToReachPosB);
  myStepperA.step(stepsRequiredToReachPosA);
  Serial.println("Steppermotor 1 aktiveret");
  Serial.println("Steppermotor 2 aktiveret");
}





void loop() 
{
  driveToPos();
  delay(10000000);
}
