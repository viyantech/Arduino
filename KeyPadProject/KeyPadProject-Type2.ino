#include <Wire.h>               // Library for I2C
#include <LiquidCrystal_I2C.h>  // Library for I2C LCD
#include <MFRC522.h>            // Library for RFID
#include <Servo.h>              // Library for Servo motor
#include <Keypad.h>             // Library for Keypad

// Initialize LCD with I2C address (usually 0x27 or 0x3F for 16x2 LCD)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define RFID Pins
#define SS_PIN 53
#define RST_PIN 49
MFRC522 rfid(SS_PIN, RST_PIN);

// Servo motor object
Servo doorServo;

// Keypad setup
const byte ROWS = 4;            // Four rows
const byte COLS = 4;            // Four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {33, 35, 37, 39};    // Connect to the row pinouts of the keypad
byte colPins[COLS] = {31, 29, 27, 25};    // Connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Define correct RFID UID and keypad password
String correctUID = "A0916120";   // Replace with your RFID tag UID
String correctPassword = "123";       // Define your desired password
String inputPassword = "";

// Variables to manage door state
bool doorOpen = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  doorServo.attach(9);        // Servo connected to Pin 9
  doorServo.write(0);         // Door locked position

  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Door Lock System");

  delay(2000);                // Initial message delay
  lcd.clear();
  lcd.print("Scan Card or");
  lcd.setCursor(0, 1);
  lcd.print("Enter Password");
}

void loop() {
  // Check for RFID card
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String uid = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      uid += String(rfid.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();
    Serial.print("Card UID: ");
    Serial.println(uid);

    if (uid == correctUID) {
      openDoor();
    } else {
      lcd.clear();
      lcd.print("Access Denied!");
      delay(2000);
      lcd.clear();
      lcd.print("Scan Card or");
      lcd.setCursor(0, 1);
      lcd.print("Enter Password");
    }
    rfid.PICC_HaltA();
  }

  // Check for keypad input
  char key = keypad.getKey();
  if (key) {
    lcd.setCursor(0, 1);
    if (key == '#') {
      if (inputPassword == correctPassword) {
        openDoor();
      } else {
        lcd.clear();
        lcd.print("Incorrect Pass");
        delay(2000);
        lcd.clear();
        lcd.print("Enter Password");
      }
      inputPassword = "";  // Reset password after attempt
    } else if (key == '*') {
      inputPassword = "";  // Clear input
      lcd.clear();
      lcd.print("Enter Password");
    } else {
      inputPassword += key;  // Append key to password input
      lcd.print("*");        // Display '*' for each input (password masking)
    }
  }
}

// Function to unlock the door
void openDoor() {
  lcd.clear();
  lcd.print("Access Granted!");
  doorServo.write(90);         // Adjust to desired unlock angle
  delay(2000);                 // Keep door unlocked for 5 seconds
  doorServo.write(0);          // Lock the door again
  lcd.clear();
  lcd.print("Door Locked");
  delay(1000);
  lcd.clear();
  lcd.print("Scan Card or");
  lcd.setCursor(0, 1);
  lcd.print("Enter Password");
}
