int pin = 2;
int LEDpin = 13;
int readPin = A0;
int reading = 0;

bool fieldReadingState = true;
bool criteria;

void setup() {
  Serial.begin(9600);
  pinMode(pin, OUTPUT);
  pinMode(LEDpin, OUTPUT);
}

void recieve() {
  reading = analogRead(A0);
  while (reading < 200) {
    reading = analogRead(A0);
    digitalWrite(LEDpin, LOW);
  }
}

void sendSucces() {
  digitalWrite(pin, HIGH);
  delay(100);
  digitalWrite(pin, LOW);
}

void loop() {
  recieve();
  if (Serial.available()) {
    char readingnoget = Serial.read();
    while (readingnoget != '1') {
      readingnoget = Serial.read();
      digitalWrite(LEDpin, HIGH);
    }
    sendSucces();
  }
}
