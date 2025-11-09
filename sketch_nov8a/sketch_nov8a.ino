int speedPin = 12; // Speed pin for all motors
int forwardPins[] = {14, 25, 16, 18} ; // Pins for all forward GPIO
int backwardPins[] = {27, 26, 17, 5}; // Pins for all backward GPIO
int numMotors = sizeof(forwardPins) / sizeof(forwardPins[0]); // Number of motor control pins

void setup() {
  // put your setup code here, to run once:

  for (int i=0; i < (numMotors); i++) {
    pinMode(forwardPins[i], OUTPUT);
    pinMode(backwardPins[i], OUTPUT);
  }

  pinMode(speedPin, OUTPUT); // Set speed pin to be output
  writeSpeed(255); // Write initial speed for car
  
}

void loop() {

  forward();
  delay(5000);


  stop();
  delay(100000);




}

void writeSpeed(int speed) {
  analogWrite(speedPin, speed);
}

void forwardMotor(int forwardPin, int backwardPin) {
  digitalWrite(forwardPin, HIGH);
  digitalWrite(backwardPin, LOW);
}

void backwardMotor(int forwardPin, int backwardPin) {
  digitalWrite(forwardPin, LOW);
  digitalWrite(backwardPin, HIGH);
}

void stopMotor(int forwardPin, int backwardPin) {
  digitalWrite(forwardPin, LOW);
  digitalWrite(backwardPin, LOW);
}


void forward() {

  for (int i=0; i < (numMotors); i++) {
    forwardMotor(forwardPins[i], backwardPins[i]);
  }

}

void backward() {

  for (int i=0; i < (numMotors); i++) {
    backwardMotor(forwardPins[i], backwardPins[i]);
  }

}

void stop() {

  for (int i=0; i < (numMotors); i++) {
    stopMotor(forwardPins[i], backwardPins[i]);
  }

}

void turnLeft() {

  for (int i=0; i < (numMotors); i++) {

    if (i < 2) {
      forwardMotor(forwardPins[i], backwardPins[i]);
    }
    else {
      backwardMotor(forwardPins[i], backwardPins[i]);
    }

  }
  
}

void turnRight() {
  
  for (int i=0; i < (numMotors); i++) {

    if (i >= 2) {
      forwardMotor(forwardPins[i], backwardPins[i]);
    }
    else {
      backwardMotor(forwardPins[i], backwardPins[i]);
    }
    
  }
  
}