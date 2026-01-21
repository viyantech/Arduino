#include <Servo.h>

Servo lidServo;

int irPin = 2;          // IR sensor OUT pin
int servoPin = 9;       // Servo signal pin

bool isOpen = false;    // To avoid repeated opening

void setup() {
  pinMode(irPin, INPUT);
  lidServo.attach(servoPin);

  lidServo.write(0);    // Lid closed position
}

void loop() {
  int irValue = digitalRead(irPin);

  // IR sensor gives LOW when obstacle is detected
  if (irValue == LOW && isOpen == false) {
    isOpen = true;

    lidServo.write(180);     // Open lid
    delay(3000);            // Keep lid open

    lidServo.write(0);      // Close lid
    delay(1000);
  }

  // Reset when hand is removed
  if (irValue == HIGH) {
    isOpen = false;
  }
}
