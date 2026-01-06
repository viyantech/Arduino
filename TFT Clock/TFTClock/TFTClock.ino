#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---------- PINS ----------
#define JOY_X A1
#define JOY_Y A2
#define JOY_BTN 2

// ---------- MENU ----------
int menuIndex = 0;
bool inMenu = true;
const char* games[] = {"SNAKE", "SHOOTER", "PONG", "CATCH"};
const int gameCount = 4;

// ---------- TIMING ----------
unsigned long lastUpdate = 0;

// ---------- SNAKE ----------
int snakeX[64], snakeY[64];
int snakeLen, foodX, foodY, snakeDir;

// ---------- SHOOTER ----------
int shipX, bulletY, enemyX, enemyY;
bool bulletActive;
unsigned long enemyTimer;

// ---------- PONG ----------
int paddleY, ballX, ballY, ballVX, ballVY;

// ---------- CATCH ----------
int playerX, objX, objY, score, lives;

// ---------- SETUP ----------
void setup() {
  pinMode(JOY_BTN, INPUT_PULLUP);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
}

// ---------- LOOP ----------
void loop() {
  if (inMenu) {
    drawMenu();
    menuInput();
  } else {
    if (menuIndex == 0) playSnake();
    if (menuIndex == 1) playShooter();
    if (menuIndex == 2) playPong();
    if (menuIndex == 3) playCatch();
  }
}

// ---------- MENU ----------
void drawMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.println("Sevviyan's");
  display.setCursor(0, 12);
  display.println("Console");

  for (int i = 0; i < gameCount; i++) {
    display.setCursor(10, 28 + i * 10);
    display.print(i == menuIndex ? "> " : "  ");
    display.println(games[i]);
  }
  display.display();
}

void menuInput() {
  int y = analogRead(JOY_Y);

  if (y < 400) {
    menuIndex = (menuIndex + gameCount - 1) % gameCount;
    delay(180);
  }
  if (y > 600) {
    menuIndex = (menuIndex + 1) % gameCount;
    delay(180);
  }

  if (digitalRead(JOY_BTN) == LOW) {
    delay(200);
    startGame();
  }
}

// ---------- GAME INIT ----------
void startGame() {
  inMenu = false;
  lastUpdate = millis();

  // Snake
  snakeLen = 3;
  snakeDir = 1;
  for (int i = 0; i < snakeLen; i++) {
    snakeX[i] = 60 - i * 4;
    snakeY[i] = 32;
  }
  foodX = random(0, 124);
  foodY = random(0, 60);

  // Shooter
  shipX = 60;
  bulletActive = false;
  enemyX = random(0, 120);
  enemyY = 0;
  enemyTimer = millis();

  // Pong
  paddleY = 24;
  ballX = 64;
  ballY = 32;
  ballVX = -2;
  ballVY = 2;

  // Catch
  playerX = 60;
  objX = random(0, 120);
  objY = 0;
  score = 0;
  lives = 3;
}

// ---------- SNAKE ----------
void playSnake() {
  if (millis() - lastUpdate < 140) return;
  lastUpdate = millis();

  int x = analogRead(JOY_X);
  int y = analogRead(JOY_Y);

  if (x < 400) snakeDir = 3;
  if (x > 600) snakeDir = 1;
  if (y < 400) snakeDir = 0;
  if (y > 600) snakeDir = 2;

  for (int i = snakeLen; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  if (snakeDir == 0) snakeY[0] -= 4;
  if (snakeDir == 1) snakeX[0] += 4;
  if (snakeDir == 2) snakeY[0] += 4;
  if (snakeDir == 3) snakeX[0] -= 4;

  if (snakeX[0] < 0 || snakeX[0] > 124 || snakeY[0] < 0 || snakeY[0] > 60) endGame();

  if (abs(snakeX[0] - foodX) < 4 && abs(snakeY[0] - foodY) < 4) {
    snakeLen++;
    foodX = random(0, 124);
    foodY = random(0, 60);
  }

  display.clearDisplay();
  for (int i = 0; i < snakeLen; i++) {
    display.fillRect(snakeX[i], snakeY[i], 4, 4, WHITE);
  }
  display.fillRect(foodX, foodY, 4, 4, WHITE);
  display.display();
}

// ---------- SHOOTER ----------
void playShooter() {
  int x = analogRead(JOY_X);

  if (x < 400) shipX -= 3;
  if (x > 600) shipX += 3;
  shipX = constrain(shipX, 0, 120);

  if (!bulletActive && digitalRead(JOY_BTN) == LOW) {
    bulletActive = true;
    bulletY = 54;
  }

  if (bulletActive) bulletY -= 5;
  if (bulletY < 0) bulletActive = false;

  if (millis() - enemyTimer > 120) {
    enemyY++;
    enemyTimer = millis();
  }

  if (enemyY > 64) {
    enemyY = 0;
    enemyX = random(0, 120);
  }

  if (bulletActive && abs(enemyX - shipX) < 8 && abs(enemyY - bulletY) < 8) {
    enemyY = 0;
    enemyX = random(0, 120);
    bulletActive = false;
  }

  display.clearDisplay();
  display.fillRect(shipX, 56, 8, 8, WHITE);
  if (bulletActive) display.fillRect(shipX + 3, bulletY, 2, 4, WHITE);
  display.fillRect(enemyX, enemyY, 8, 8, WHITE);
  display.display();
}

// ---------- PONG ----------
void playPong() {
  int y = analogRead(JOY_Y);

  if (y < 400) paddleY -= 4;
  if (y > 600) paddleY += 4;
  paddleY = constrain(paddleY, 0, 48);

  ballX += ballVX;
  ballY += ballVY;

  if (ballY <= 0 || ballY >= 60) ballVY *= -1;
  if (ballX >= 124) ballVX *= -1;

  if (ballX <= 6) {
    if (ballY >= paddleY && ballY <= paddleY + 16) {
      ballVX = abs(ballVX);
    } else {
      endGame();
      return;
    }
  }

  display.clearDisplay();
  display.fillRect(0, paddleY, 4, 16, WHITE);
  display.fillRect(ballX, ballY, 4, 4, WHITE);
  display.display();
}

// ---------- CATCH ----------
void playCatch() {
  int x = analogRead(JOY_X);

  if (x < 400) playerX -= 3;
  if (x > 600) playerX += 3;
  playerX = constrain(playerX, 0, 120);

  objY += 2;

  if (objY > 64) {
    objY = 0;
    objX = random(0, 120);
    lives--;
  }

  if (objY > 52 && abs(objX - playerX) < 10) {
    score++;
    objY = 0;
    objX = random(0, 120);
  }

  if (lives <= 0) endGame();

  display.clearDisplay();
  display.fillRect(playerX, 56, 12, 6, WHITE);
  display.fillRect(objX, objY, 6, 6, WHITE);

  display.setCursor(0, 0);
  display.print("S:");
  display.print(score);
  display.print(" L:");
  display.print(lives);

  display.display();
}

// ---------- GAME OVER ----------
void endGame() {
  delay(500);
  inMenu = true;
}
