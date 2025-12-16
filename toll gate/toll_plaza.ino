#include <Servo.h>

const int trigPin = 10;
const int echoPin = 11;
const int servoPin = 9;

long duration;
int distance;

Servo myServo;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  myServo.attach(servoPin);
  myServo.write(0);   // initial position

  Serial.begin(115200);
}

void loop() {
  // Trigger ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);

  // Detection logic
  if (distance > 0 && distance < 20) {
    myServo.write(90);   // rotate servo
  } else {
    myServo.write(0);    // return to start
  }

  delay(2000);
}
