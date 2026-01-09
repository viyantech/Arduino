#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 6
#define BUZZER_PIN 8

MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo doorServo;

// ✅ Authorized RFID card UID
byte allowedUID1[4] = {0xB9, 0x82, 0xD8, 0x05};

// Servo positions
int lockPos = 0;       // locked position
int unlockPos = 90;   // unlocked position

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  doorServo.attach(SERVO_PIN);
  doorServo.write(lockPos);   // start locked

  pinMode(BUZZER_PIN, OUTPUT);

  Serial.println("Scan your RFID card...");
}

void loop() {
  // Wait for card
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  bool match1 = true;

  // Check UID
  for (byte i = 0; i < 4; i++) {
    if (mfrc522.uid.uidByte[i] != allowedUID1[i]) {
      match1 = false;
    }
  }

  if (match1) {
    Serial.println("ACCESS ALLOWED ✅");

    // Unlock servo
    doorServo.write(unlockPos);

    // Passive buzzer success beep
    tone(BUZZER_PIN, 1000);
    delay(500);
    noTone(BUZZER_PIN);

    delay(3000);               // keep unlocked
    doorServo.write(lockPos);  // lock again

  } else {
    Serial.println("ACCESS DENIED ❌");

    // Passive buzzer error beep
    tone(BUZZER_PIN, 400);
    delay(800);
    noTone(BUZZER_PIN);
  }

  mfrc522.PICC_HaltA(); // stop reading
  delay(1000);          // delay before next scan
}
