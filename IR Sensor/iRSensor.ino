#include <IRremote.h>

const int IR_PIN = 3;

// RGB LED pins
const int RED_PIN   = 9;
const int GREEN_PIN = 10;
const int BLUE_PIN  = 11;

bool busy = false; // lock while blinking

// COMMON ANODE: LOW = ON, HIGH = OFF
void offRGB() {
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, HIGH);
}

void setRGB(int r, int g, int b) {
  digitalWrite(RED_PIN, r);
  digitalWrite(GREEN_PIN, g);
  digitalWrite(BLUE_PIN, b);
}

void blinkRGB(int times, int r, int g, int b) {
  busy = true;
  for (int i = 0; i < times; i++) {
    setRGB(r, g, b);
    delay(300);
    offRGB();
    delay(300);
  }
  busy = false;
}

void rainbowBlink(int times) {
  busy = true;
  for (int i = 0; i < times; i++) {
    setRGB(LOW,HIGH,HIGH); delay(200);  // Red
    setRGB(HIGH,LOW,HIGH); delay(200);  // Green
    setRGB(HIGH,HIGH,LOW); delay(200);  // Blue
    setRGB(LOW,LOW,HIGH); delay(200);   // Yellow
    setRGB(LOW,HIGH,LOW); delay(200);   // Purple
    setRGB(HIGH,LOW,LOW); delay(200);   // Cyan
    setRGB(LOW,LOW,LOW); delay(200);    // White
    offRGB(); delay(100);
  }
  busy = false;
}

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  offRGB(); // Default OFF
  Serial.println("IR RGB 1-9 Ready");
}

void loop() {
  if (!busy && IrReceiver.decode()) {

    uint32_t code = IrReceiver.decodedIRData.decodedRawData;

    if (code != 0x0) { // ignore repeats
      switch (code) {
        case 0xF30CFF00: blinkRGB(1, LOW, HIGH, HIGH); break;   // 1 Red
        case 0xE718FF00: blinkRGB(2, HIGH, LOW, HIGH); break;   // 2 Green
        case 0xA15EFF00: blinkRGB(3, HIGH, HIGH, LOW); break;   // 3 Blue
        case 0xF708FF00: blinkRGB(4, LOW, LOW, HIGH); break;    // 4 Yellow
        case 0xE31CFF00: blinkRGB(5, LOW, HIGH, LOW); break;    // 5 Purple
        case 0xA55AFF00: blinkRGB(6, HIGH, LOW, LOW); break;    // 6 Cyan
        case 0xBD42FF00: blinkRGB(7, LOW, LOW, LOW); break;     // 7 White
        case 0xAD52FF00: blinkRGB(8, LOW, LOW, HIGH); break;    // 8 Orange
        case 0xB54AFF00: rainbowBlink(3); break;               // 9 Rainbow
      }
    }

    IrReceiver.resume();
  }
}
