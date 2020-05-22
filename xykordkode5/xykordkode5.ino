// Include the Arduino Stepper Library
#include <Stepper.h>

// Number of steps per output rotation
const int STEPS = 200;

const int sendPin = 7;

const int recievePin = A15;

// Create Instance of Stepper library
Stepper stepperC(STEPS, 44, 45, 46, 47);
Stepper stepperB(STEPS, 31, 30, 32, 33);
Stepper stepperA(STEPS, 29, 28, 27, 26);
Stepper stepperD(STEPS, 22, 23, 24, 25);

// Sensorvariables
//bool stateAQ;
//bool stateBQ;
//bool stateCQ;
//bool stateDQ;
bool stateArray[4];

float caliArray[16][3] = {
  {19.9, 61.59, 30.0}, //E
  {34.4, 47.78, 30.0}, //F
  {48.86, 33.99, 30.0}, //G
  {63.33, 20.18, 30.0}, //H
  {63.33, 20.18, 50.0}, //I
  {48.86, 33.99, 50.0}, //J
  {34.41, 47.76, 50.0}, //K
  {19.9, 61.59, 50.0}, //L
  {19.9, 61.59, 70.0}, //P
  {34.43, 47.75, 70.0}, //O
  {48.86, 33.99, 70.0}, //N
  {63.33, 20.18, 70.0}, //M
  {63.33, 20.18, 80.0}, //T
  {48.86, 33.99, 80.0}, //S
  {34.44, 47.73, 80.0}, //R
  {19.19, 61.59, 80.0}, //Q



};

bool serialState = 0;
void beginProgram() {
  Serial.println("Calibrate field? Type 1");
  Serial.println("Custom coordinates? Type 2");
  char incoming;
  while (serialState == 0) {
    if (Serial.available() > 0) {
      incoming = Serial.read();
      Serial.println(incoming);
      if (incoming == '2' or incoming == '1') {
        serialState = 1;
      }
    }
  }
  if (incoming == '1') {
    cali();
  }
  if (incoming == '2') {
    Serial.println("Hej");
    while (true);
  }
}


// Pwm pins to sensor
int motorAPWM = 6;
int motorBPWM = 5;
int motorCPWM = 3;
int motorDPWM = 2;

// Sensorpin
int sensorA = A12;
int sensorB = A8;
int sensorC = A0;
int sensorD = A3;

// Counters for the different optical forks.
float countAQ = 0.0;
float countBQ = 0.0;
float countCQ = 0.0;
float countDQ = 0.0;


// Lwasdadsd
float lengthRequiredToReachPosA = 0.0;
float lengthRequiredToReachPosB = 0.0;
float lengthRequiredToReachPosC = 0.0;
float lengthRequiredToReachPosD = 0.0;


// gearshit neccesary
float gearShiftAQ = 0.0;
float gearShiftBQ = 0.0;
float gearShiftCQ = 0.0;
float gearShiftDQ = 0.0;



// Arrays for coordinate system
float A[] = {0.0, 0.0, 0.0};
float  B[] = {84.5, 0.0, 0.0 };
float C[] = {84.5, 83.0, 0.0};
float D[] = { -1.5, 82.0, 0.0};

// Actual position of string
float AQ = 107.0;
float BQ = 109.0;
float CQ = 109.0;
float DQ = 108.0;

// Wanted position
float x = 50.0;
float y = 80.0;
float z = 40.0;

//Millis
long previousMillis[4] = {0, 0, 0, 0};
long interval = 25; //Altså delayet der angiver hastighed 15 ser ud til at virke

//Millis2
long previousMillisCount = 0;
float intervalCount = 1.5;



void setup()
{
  // set the speed at 60 rpm:
  // stepperD.setSpeed(50);
  //stepperD.step(-200);
  //stepperC.setSpeed(50);
  // stepperC.step(-200);
  // delay(30000);

  pinMode(sendPin, OUTPUT);

  stepperA.setSpeed(9999999999999999);
  stepperB.setSpeed(9999999999999999);
  stepperC.setSpeed(9999999999999999);
  stepperD.setSpeed(9999999999999999);
  // initialize the serial port:
  Serial.begin(1000000);

  // Switching the state of the optical fork.
  if (analogRead(sensorA) > 120) {
    stateArray[0] = true;
  } else if (analogRead(sensorA) <= 120) {
    stateArray[0] = false;
  }
  if (analogRead(sensorB) > 120) {
    stateArray[1] = true;
  } else if (analogRead(sensorB) <= 120) {
    stateArray[1] = false;
  }
  if (analogRead(sensorC) > 120) {
    stateArray[2] = true;
  } else if (analogRead(sensorC) <= 120) {
    stateArray[2] = false;
  }
  if (analogRead(sensorD) > 120) {
    stateArray[3] = true;
  } else if (analogRead(sensorD) <= 120) {
    stateArray[3] = false;
  }



}

unsigned long previousArray[4] = {0, 0, 0, 0}; //AQ, BQ, CQ, DQ


// Optical fork function that counts gearshifts.
int counter(int sensorX, int localCount, int thresholdOver, int thresholdUnder, int corner) {

  long sensRead = 0;

  for (int i = 0; i < 100; i++) {
    sensRead = sensRead + analogRead(sensorX);
  }
  int reading = sensRead / 100;
  Serial.print(corner);
  Serial.println(reading);

  unsigned long currentMillisCount = millis();
  //Serial.println(read);
  if (reading > thresholdUnder) {
    previousArray[corner] = currentMillisCount;
  }

  if ((reading > thresholdUnder) && (stateArray[corner] == false)) {
    localCount++;

    //previousMillisCount = currentMillisCount;

    stateArray[corner] = true;
  } else if ((reading <= thresholdOver) && (stateArray[corner] == true)) {

    if ((currentMillisCount - previousArray[corner]) > intervalCount) {
      localCount++;

      stateArray[corner] = false;
    }
  }


  return localCount;
}

// Function that calculates the required gearshifts for a defined length.
float calcGearshifts(float lengthToPosX) {
  float cmPrSkift = 9.425/60;
  float x = lengthToPosX/cmPrSkift;
  return abs(x);
}

void calculateLength(int arrayIndex) {
  // Calculates the vectorcoordinates for each string.
  float vectorCoordinatesA[] = {caliArray[arrayIndex][0] - A[0], caliArray[arrayIndex][1] - A[1], caliArray[arrayIndex][2] - A[2]};
  float vectorCoordinatesB[] = {caliArray[arrayIndex][0] - B[0], caliArray[arrayIndex][1] - B[1], caliArray[arrayIndex][2] - B[2]};
  float vectorCoordinatesC[] = {caliArray[arrayIndex][0] - C[0], caliArray[arrayIndex][1] - C[1], caliArray[arrayIndex][2] - C[2]};
  float vectorCoordinatesD[] = {caliArray[arrayIndex][0] - D[0], caliArray[arrayIndex][1] - D[1], caliArray[arrayIndex][2] - D[2]};

  // Calculates the lengths of each string with vectorcalculations.
  float vectorLengthA = sqrt(pow(abs(vectorCoordinatesA[0]), 2) + pow(abs(vectorCoordinatesA[1]), 2) + pow(abs(vectorCoordinatesA[2]), 2));
  float vectorLengthB = sqrt(pow(abs(vectorCoordinatesB[0]), 2) + pow(abs(vectorCoordinatesB[1]), 2) + pow(abs(vectorCoordinatesB[2]), 2));
  float vectorLengthC = sqrt(pow(abs(vectorCoordinatesC[0]), 2) + pow(abs(vectorCoordinatesC[1]), 2) + pow(abs(vectorCoordinatesC[2]), 2));
  float vectorLengthD = sqrt(pow(abs(vectorCoordinatesD[0]), 2) + pow(abs(vectorCoordinatesD[1]), 2) + pow(abs(vectorCoordinatesD[2]), 2));
  Serial.println(vectorLengthB);
  Serial.println(vectorLengthC);
  Serial.println(vectorLengthD);
  if (arrayIndex < 1) {
    // Steps required to reach the wanted position by subtracting the steps equal to the length of the string.
    lengthRequiredToReachPosA = vectorLengthA - AQ;
    lengthRequiredToReachPosB = vectorLengthB - BQ;
    lengthRequiredToReachPosC = vectorLengthC - CQ;
    lengthRequiredToReachPosD = vectorLengthD - DQ;
  }
  else {
    AQ = lengthRequiredToReachPosA + AQ;
    BQ = lengthRequiredToReachPosB + BQ;
    CQ = lengthRequiredToReachPosC + CQ;
    DQ = lengthRequiredToReachPosD + DQ;

    lengthRequiredToReachPosA = vectorLengthA - AQ;
    lengthRequiredToReachPosB = vectorLengthB - BQ;
    lengthRequiredToReachPosC = vectorLengthC - CQ;
    lengthRequiredToReachPosD = vectorLengthD - DQ;

  }
  Serial.println(arrayIndex);
  Serial.println(lengthRequiredToReachPosA);
  Serial.println(lengthRequiredToReachPosB);
  Serial.println(lengthRequiredToReachPosC);
  Serial.println(lengthRequiredToReachPosD);

  // Gearshifts required to reach position.
  gearShiftAQ = calcGearshifts(lengthRequiredToReachPosA);
  gearShiftBQ = calcGearshifts(lengthRequiredToReachPosB);
  gearShiftCQ = calcGearshifts(lengthRequiredToReachPosC);
  gearShiftDQ = calcGearshifts(lengthRequiredToReachPosD);
  Serial.println("Færdig med beregninger");
}



void cali() {
  for (int i = 10; i <= 15; i++) {
    Serial.println(i);
    delay(2000);
    calculateLength(i);
    countDQ = 0;
    countCQ = 0;
    countBQ = 0;
    countAQ = 0;
    while ((countDQ < gearShiftDQ) || (countBQ < gearShiftBQ) || (countCQ < gearShiftCQ) || (countAQ < gearShiftAQ)) {
      unsigned long currentMillis = millis();
      Serial.println(countAQ - gearShiftAQ);
      Serial.println(countBQ - gearShiftBQ);
      Serial.println(countCQ - gearShiftCQ);
      Serial.println(countDQ - gearShiftDQ);
      if (countAQ < gearShiftAQ) {
        analogWrite(motorAPWM, 255);
        countAQ = counter(sensorA, countAQ, 500, 500, 0);
        if (currentMillis - previousMillis[0] > interval) {
          previousMillis[0] = currentMillis;
          if (lengthRequiredToReachPosA < 0) {
            stepperA.step(-1);
          }
          else if (lengthRequiredToReachPosA > 0) {
            stepperA.step(1);
          }
        }
      }

      if (countBQ < gearShiftBQ) {
        analogWrite(motorBPWM, 255);
        countBQ = counter(sensorB, countBQ, 500, 500, 1);
        if (currentMillis - previousMillis[1] > interval) {
          previousMillis[1] = currentMillis;
          if (lengthRequiredToReachPosB < 0) {
            stepperB.step(-1);
          }
          else if (lengthRequiredToReachPosB > 0) {
            stepperB.step(1);
          }
        }
      }

      if (countCQ < gearShiftCQ) {
        analogWrite(motorCPWM, 255);
        countCQ = counter(sensorC, countCQ, 500, 500, 2);
        if (currentMillis - previousMillis[2] > interval) {
          previousMillis[2] = currentMillis;
          if (lengthRequiredToReachPosC < 0) {
            stepperC.step(-1);
          }
          else if (lengthRequiredToReachPosC > 0) {
            stepperC.step(1);
          }
        }
      }


      if (countDQ < gearShiftDQ) {
        analogWrite(motorDPWM, 255);
        countDQ = counter(sensorD, countDQ, 800, 900, 3);
        currentMillis = millis();
        if (currentMillis - previousMillis[3] > interval) {
          previousMillis[3] = currentMillis;
          if (lengthRequiredToReachPosD < 0) {
            stepperD.step(-1);
          }
          else if (lengthRequiredToReachPosD > 0) {
            stepperD.step(1);
          }
        }
      }
      /*
        Serial.print("AQ: ");
        Serial.println(countAQ - gearShiftAQ);
        Serial.print("BQ: ");
        Serial.println(countBQ - gearShiftBQ);
        Serial.print("DQ: ");
        Serial.println(countDQ - gearShiftDQ);
        Serial.print("CQ: ");
        Serial.println(countCQ - gearShiftCQ);
      */
    }
    digitalWrite(sendPin, HIGH);
    delay(20);
    digitalWrite(sendPin, LOW);
    Serial.println("Udfører måling");
    while (analogRead(recievePin) < 200);
  }
}

void loop()
{
  beginProgram();

}
