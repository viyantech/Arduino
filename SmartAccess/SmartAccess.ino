#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 6
#define BUZZER_PIN 8

MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo doorServo;

// ✅ Your authorized RFID card UID
byte allowedUID1[4] = {0xB9, 0x82, 0xD8, 0x05};

int lockPos = 0;      // servo locked position
int unlockPos = 90;   // servo unlocked position

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  doorServo.attach(SERVO_PIN);
  doorServo.write(lockPos);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println("Scan your RFID card...");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  bool match1 = true;

  for (byte i = 0; i < 4; i++) {
    if (mfrc522.uid.uidByte[i] != allowedUID1[i]) match1 = false;
  }

  if (match1) {
    Serial.println("ACCESS ALLOWED ✅");

    // Unlock servo
    doorServo.write(unlockPos);

    // Passive buzzer beep for access allowed
    tone(BUZZER_PIN, 1000, 500); // 1000 Hz, 0.5 sec

    delay(3000);                 // keep unlocked
    doorServo.write(lockPos);    // lock again
  } else {
    Serial.println("ACCESS DENIED ❌");

    // Passive buzzer long beep for access denied
    tone(BUZZER_PIN, 400, 800);  // 400 Hz, 0.8 sec
    delay(800);                   // wait for beep to finish
  }

  delay(1000); // wait before next scan
}
