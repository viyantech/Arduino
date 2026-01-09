#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>

// -------- OLED --------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// -------- Keypad --------
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// -------- Logic --------
String correctCode = "67";
String inputCode = "";

// -------- Passive Buzzer --------
#define BUZZER_PIN 10

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  showEnterCode();
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    beepKey();  // short beep for key press

    if (key == '*') {        // Clear input
      inputCode = "";
      showEnterCode();
    } 
    else if (key == '#') {   // Check code
      if (inputCode == correctCode) {
        beepSuccess();       // Two short beeps
        showDoorOpened();
      } else {
        beepError();         // Long buzz
        showWrongCode();
      }
      inputCode = "";
    }
    else {
      inputCode += key;
      showStars();
    }
  }
}

// -------- Display Functions --------
void drawCenteredText(const char* text, int y) {
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, y);
  display.print(text);
}

void showEnterCode() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  drawCenteredText("ENTER", 18);
  drawCenteredText("CODE", 38);
  display.display();
}

void showWrongCode() {
  display.clearDisplay();
  display.setTextSize(2);
  drawCenteredText("WRONG", 18);
  drawCenteredText("CODE", 38);
  display.display();
  delay(1500);
  showEnterCode();
}

void showDoorOpened() {
  display.clearDisplay();
  display.setTextSize(2);
  drawCenteredText("DOOR", 18);
  drawCenteredText("OPENED", 38);
  display.display();
  delay(2000);
  showEnterCode();
}

void showStars() {
  display.clearDisplay();
  display.setTextSize(2);
  drawCenteredText("PIN", 10);

  String stars = "";
  for (int i = 0; i < inputCode.length(); i++) stars += "*";

  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(stars.c_str(), 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 34);
  display.print(stars);
  display.display();
}

// -------- Passive Buzzer Sounds --------
void beepKey() {
  tone(BUZZER_PIN, 2500, 40);   // key press beep
}

void beepSuccess() {
  tone(BUZZER_PIN, 1500, 100);
  delay(150);
  tone(BUZZER_PIN, 1800, 100);  // two short beeps for success
}

void beepError() {
  tone(BUZZER_PIN, 400, 600);   // long low buzz for wrong code
}
