const int soundPin = 2;

const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

bool played = false;

void setup() {
  pinMode(soundPin, INPUT);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  turnOff();
}

void loop() {
  int soundState = digitalRead(soundPin);

  if (soundState == LOW && !played) {
    rainbowOnce();
    played = true;
    delay(500);
  }

  if (soundState == HIGH) {
    played = false; // reset for next clap
  }
}

void rainbowOnce() {
  // Red
  setColor(255, 0, 0); delay(500);
  // Orange
  setColor(255, 80, 0); delay(500);
  // Yellow
  setColor(255, 255, 0); delay(500);
  // Green
  setColor(0, 255, 0); delay(500);
  // Blue
  setColor(0, 0, 255); delay(500);
  // Indigo
  setColor(75, 0, 130); delay(500);
  // Violet
  setColor(148, 0, 211); delay(500);

  turnOff();
}

void setColor(int r, int g, int b) {
  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);
}

void turnOff() {
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
}
