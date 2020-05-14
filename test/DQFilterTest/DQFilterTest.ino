// Include the Arduino Stepper Library
#include <Stepper.h>

// Number of steps per output rotation
const int STEPS = 200;

// Create Instance of Stepper library
Stepper stepperC(STEPS, 37, 36, 35, 34);
Stepper stepperB(STEPS, 31, 30, 32, 33);
Stepper stepperA(STEPS, 29, 28, 27, 26);
Stepper stepperD(STEPS, 22, 23, 24, 25);

// Sensorvariables
bool state;

// Pwm pins to sensor
int motorAPWM = 6;
int motorBPWM = 5;
int motorCPWM = 3;
int motorDPWM = 2;

// Sensorpin
int sensorA = A5;
int sensorB = A4;
int sensorC = A2;
int sensorD = A3;

// Counters for the different optical forks.
int countAQ = 0;
int countBQ = 0;
int countCQ = 0;
int countDQ = 0;


// Lwasdadsd
float lengthRequiredToReachPosA;
float lengthRequiredToReachPosB;
float lengthRequiredToReachPosC;
float lengthRequiredToReachPosD;


// gearshit neccesary
float gearShiftAQ = 0;
float gearShiftBQ = 0;
float gearShiftCQ = 0;
float gearShiftDQ = 0;



// Arrays for coordinate system
float A[] = {0.0, 0.0, 0.0};
float  B[] = {84.5, 0.0, 0.0 };
float C[] = {84.5, 83.0, 0.0};
float D[] = { -1.5, 82.0, 0.0};

// Actual position of string
float AQ = 125.0;
float BQ = 117.0;
float CQ = 97.0;
float DQ = 107.5;

// Wanted position
float x = 50.0;
float y = 50.0;
float z = 20.0;

//Millis
long previousMillis = 0;
long interval = 15; //Altså delayet der angiver hastighed

//Millis2
long previousMillisCount = 0;
long intervalCount = 2;



void setup()
{
  // set the speed at 60 rpm:
  stepperA.setSpeed(99999999999999);
  stepperB.setSpeed(99999999999999);
  stepperC.setSpeed(99999999999999);
  stepperD.setSpeed(99999999999999);
  // initialize the serial port:
  Serial.begin(500000);

  // Switching the state of the optical fork.
  if (analogRead(sensorA) > 120) {
    state = true;
  } else if (analogRead(sensorA) <= 120) {
    state = false;
  }
}


// Optical fork function that counts gearshifts.
int counter(int sensorX, int localCount, int thresholdOver, int thresholdUnder) {
  int reading = analogRead(sensorX);
  unsigned long currentMillisCount = millis();
  //Serial.println(read);

    
       
      if (reading > thresholdUnder && state == false) {
        localCount++;
        previousMillisCount = currentMillisCount;
        state = true;
      } else if (reading <= thresholdOver && state == true) {
        
        if(currentMillisCount - previousMillisCount > intervalCount) {
        localCount++;
        state = false;
      }
    }

  


  return localCount;
}

// Function that calculates the required gearshifts for a defined length.
float calcGearshifts(float lengthToPosX) {
  float x = 1.20 + 6.56 * lengthToPosX;
  return abs(x);
}

void calculateLength() {
  // Calculates the vectorcoordinates for each string.
  float vectorCoordinatesA[] = {A[0] - x, A[1] - y, A[2] - z};
  float vectorCoordinatesB[] = {B[0] - x, B[1] - y, B[2] - z};
  float vectorCoordinatesC[] = {C[0] - x, C[1] - y, C[2] - z};
  float vectorCoordinatesD[] = {D[0] - x, D[1] - y, D[2] - z};

  // Calculates the lengths of each string with vectorcalculations.
  float vectorLengthA = sqrt(pow(abs(vectorCoordinatesA[0]), 2) + pow(abs(vectorCoordinatesA[1]), 2) + pow(abs(vectorCoordinatesA[2]), 2));
  float vectorLengthB = sqrt(pow(abs(vectorCoordinatesB[0]), 2) + pow(abs(vectorCoordinatesB[1]), 2) + pow(abs(vectorCoordinatesB[2]), 2));
  float vectorLengthC = sqrt(pow(abs(vectorCoordinatesC[0]), 2) + pow(abs(vectorCoordinatesC[1]), 2) + pow(abs(vectorCoordinatesC[2]), 2));
  float vectorLengthD = sqrt(pow(abs(vectorCoordinatesD[0]), 2) + pow(abs(vectorCoordinatesD[1]), 2) + pow(abs(vectorCoordinatesD[2]), 2));

  
  // Steps required to reach the wanted position by subtracting the steps equal to the length of the string.
  lengthRequiredToReachPosA = vectorLengthA - AQ;
  lengthRequiredToReachPosB = vectorLengthB - BQ;
  lengthRequiredToReachPosC = vectorLengthC - CQ;
  lengthRequiredToReachPosD = vectorLengthD - DQ;


  // Gearshifts required to reach position.
  gearShiftAQ = calcGearshifts(lengthRequiredToReachPosA);
  gearShiftBQ = calcGearshifts(lengthRequiredToReachPosB);
  gearShiftCQ = calcGearshifts(lengthRequiredToReachPosC);
  gearShiftDQ = calcGearshifts(lengthRequiredToReachPosD);
  Serial.println("Færdig med beregninger");
}

//
void driveToPos() {
  Serial.println("Påbegynder kørsel");
  while (countAQ < gearShiftAQ) {
    if (countAQ < gearShiftAQ) {
      analogWrite(motorAPWM, 127);
      countAQ = counter(sensorA, countAQ, 130, 150);
    } 
else if (countAQ >= gearShiftAQ) {
  break;
}
    
    if (lengthRequiredToReachPosA < 0) {
      stepperA.step(-1);
      Serial.println(countAQ);
    } else {
      stepperA.step(1);
      Serial.println(countAQ);
    }
  }
}

float microsprogram = 0;
float microsporgramend = 0;

  void loop()
  {      
    calculateLength();
    Serial.println(lengthRequiredToReachPosA);
   // analogWrite(motorDPWM, 127);
   // Serial.println(analogRead(sensorD));
    
    while (countDQ < gearShiftDQ){
      //microsprogram= micros();
      unsigned long currentMillis = millis();
      analogWrite(motorDPWM, 127);
      countDQ = counter(sensorD, countDQ, 100, 200);

      if(currentMillis - previousMillis > interval) {
        previousMillis = currentMillis; 
      
        stepperD.step(-1); 
        Serial.println(countDQ);
      
      }
      //Serial.println(micros()-microsprogram);
    }
  }
