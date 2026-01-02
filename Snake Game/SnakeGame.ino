#include <LedControl.h>

LedControl lc = LedControl(11, 13, 10, 1);

// ---------- PINS ----------
int joyX = A0;
int joyBtn = 2;
int buzzer = 3; // single buzzer

// ---------- PLAYER ----------
int shipX = 3;
int laserX = -1;
int laserY = -1;

// ---------- DEATH STAR ----------
int weakX = 3;
int weakY = 1;
int weakDir = 1;

// ---------- GAME STATE ----------
int hits = 0;
const int maxHits = 8;
bool win = false;
bool winShown = false; // shows heart once
bool gameOver = false;  // wait for restart
int speedDelay = 120;

// ---------- SETUP ----------
void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  pinMode(joyBtn, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  randomSeed(analogRead(0));
  dramaticStart();
}

// ---------- LOOP ----------
void loop() {

  // Show heart once
  if (win && !winShown) {
    winSound();   // Play victory sound
    showHeart();  // Display heart
    winShown = true;
    gameOver = true;  // Wait for restart
  }

  // Wait for player to restart
  if (gameOver) {
    if (digitalRead(joyBtn) == LOW) {
      resetGame();
    }
    return;
  }

  lc.clearDisplay(0);

  drawHitBar();
  moveWeakPoint();
  moveShip();
  fireLaser();
  moveLaser();
  checkHit();
  drawObjects();

  delay(speedDelay);
}

// ---------- MOVEMENT ----------
void moveWeakPoint() {
  weakX += weakDir;
  if (weakX <= 1 || weakX >= 6) weakDir *= -1;
}

void moveShip() {
  int xVal = analogRead(joyX);
  if (xVal < 400 && shipX > 1) shipX--;
  if (xVal > 600 && shipX < 6) shipX++;
}

// ---------- LASER ----------
void fireLaser() {
  if (digitalRead(joyBtn) == LOW && laserY == -1) {
    laserX = shipX;
    laserY = 6;
    blasterSound();
  }
}

void moveLaser() {
  if (laserY >= 0)
    laserY--;
}

// ---------- HIT CHECK ----------
void checkHit() {
  if (laserX == weakX && laserY == weakY) {
    hits++;
    laserY = -1;
    hitSound();

    if (hits >= maxHits) {
      win = true;
    }
  }
}

// ---------- DRAW ----------
void drawObjects() {
  // Weak point
  lc.setLed(0, weakY, weakX, true);

  // Ship
  lc.setLed(0, 7, shipX, true);
  if (shipX > 0) lc.setLed(0, 7, shipX - 1, true);
  if (shipX < 7) lc.setLed(0, 7, shipX + 1, true);

  // Laser
  if (laserY >= 0)
    lc.setLed(0, laserY, laserX, true);
}

// ---------- HIT BAR ----------
void drawHitBar() {
  for (int i = 0; i < hits; i++) {
    lc.setLed(0, 7 - i, 0, true);
  }
}

// ---------- HEART DISPLAY ----------
void showHeart() {
  byte heart[8] = {
    B01100110,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100,
    B00011000,
    B00000000
  };

  lc.clearDisplay(0);
  for (int i = 0; i < 8; i++) {
    lc.setRow(0, i, heart[i]);
  }
  delay(2000); // Show heart for 2 seconds
  lc.clearDisplay(0);
}

// ---------- SOUND ----------
void dramaticStart() {
  for (int f = 200; f < 800; f += 20) {
    tone(buzzer, f, 30);
    delay(30);
  }
  noTone(buzzer);
}

void blasterSound() {
  tone(buzzer, 1200, 50);
  delay(60);
  noTone(buzzer);
}

void hitSound() {
  tone(buzzer, 700, 100);
  delay(120);
  noTone(buzzer);
}

void winSound() {
  int notes[] = {400, 600, 800, 1000};
  for (int i = 0; i < 4; i++) {
    tone(buzzer, notes[i], 200);
    delay(220);
  }
  noTone(buzzer);
}

// ---------- RESET ----------
void resetGame() {
  shipX = 3;
  laserY = -1;

  weakX = random(2, 6);
  weakDir = random(2) ? 1 : -1;

  hits = 0;
  win = false;
  winShown = false;
  gameOver = false;
  lc.clearDisplay(0);
}
