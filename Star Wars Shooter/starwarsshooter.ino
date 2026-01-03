#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  8

Adafruit_ST7735 display = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Screen
#define W 96
#define H 80

// Buttons
#define LEFT_BTN  2
#define RIGHT_BTN 3
#define FIRE_BTN  4

// Buzzer
#define BUZZER    5

// Player
int shipX = 44;
int shipY = 72;
int lives = 3;

// Bullet
int bulletX = -1;
int bulletY = -1;

// Death Star
int enemyX = 20;
int enemyY = 10;
bool enemyAlive = true;

// Asteroids
#define MAX_AST 5
int asteroidX[MAX_AST];
int asteroidY[MAX_AST];
bool asteroidAlive[MAX_AST];

// Score
int score = 0;

// Colors
#define DEATH_STAR_GRAY 0x8410
#define DEATH_STAR_LIGHT 0xC618
#define DEATH_STAR_CORE ST77XX_WHITE
#define PLAYER_COLOR ST77XX_WHITE
#define BULLET_COLOR ST77XX_CYAN
#define ASTEROID_COLOR ST77XX_YELLOW

// Functions
void laserSound(){ tone(BUZZER, 2000, 50);}
void explosionSound(){ for(int i=0;i<3;i++){ tone(BUZZER, 1000+i*200, 80); delay(80);} }

void drawShip(){
  display.fillTriangle(shipX, shipY, shipX+8, shipY, shipX+4, shipY-8, PLAYER_COLOR);
}

void drawDeathStar(){
  display.fillCircle(enemyX+8, enemyY+8, 8, DEATH_STAR_GRAY);
  display.fillCircle(enemyX+6, enemyY+6, 4, DEATH_STAR_LIGHT);
  display.fillCircle(enemyX+10, enemyY+6, 2, DEATH_STAR_CORE);
}

void drawAsteroids(){
  for(int i=0;i<MAX_AST;i++){
    if(asteroidAlive[i]){
      display.fillCircle(asteroidX[i], asteroidY[i], 3, ASTEROID_COLOR);
    }
  }
}

void spawnAsteroids(){
  for(int i=0;i<MAX_AST;i++){
    if(!asteroidAlive[i]){
      asteroidAlive[i]=true;
      asteroidX[i]=random(5,W-5);
      asteroidY[i]=0;
    }
  }
}

void setup() {
  pinMode(LEFT_BTN, INPUT_PULLUP);
  pinMode(RIGHT_BTN, INPUT_PULLUP);
  pinMode(FIRE_BTN, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  randomSeed(analogRead(0));

  display.initR(INITR_MINI160x80);
  display.fillScreen(ST77XX_BLACK);

  // Initialize asteroids
  for(int i=0;i<MAX_AST;i++) asteroidAlive[i]=false;
}

void loop() {
  if(lives<=0){
    display.fillScreen(ST77XX_BLACK);
    display.setCursor(15,35);
    display.setTextColor(ST77XX_RED);
    display.setTextSize(2);
    display.print("GAME");
    display.setCursor(15,50);
    display.print("OVER");
    while(1); // freeze game
  }

  display.fillScreen(ST77XX_BLACK);

  // Background stars
  for(int i=0;i<20;i++){
    display.drawPixel(random(0,W), random(0,H), ST77XX_WHITE);
  }

  // Player movement
  if(digitalRead(LEFT_BTN)==LOW && shipX>2) shipX-=2;
  if(digitalRead(RIGHT_BTN)==LOW && shipX<W-10) shipX+=2;

  // Fire bullet
  if(digitalRead(FIRE_BTN)==LOW && bulletY<0){
    bulletX=shipX+4;
    bulletY=shipY-8;
    laserSound();
  }

  // Bullet movement
  if(bulletY>=0){
    bulletY-=4;
    display.drawFastVLine(bulletX, bulletY, 4, BULLET_COLOR);
    if(bulletY<0) bulletY=-1;
  }

  // Death Star
  if(enemyAlive){
    drawDeathStar();
    enemyY+=1;
    if(enemyY>H){
      enemyY=10;
      enemyX=random(5,W-20);
    }
  }

  // Asteroids
  spawnAsteroids();
  drawAsteroids();
  for(int i=0;i<MAX_AST;i++){
    if(asteroidAlive[i]){
      asteroidY[i]+=1;
      if(asteroidY[i]>H) asteroidAlive[i]=false;

      // Collision with player
      int dx=asteroidX[i]- (shipX+4);
      int dy=asteroidY[i]- (shipY-4);
      if(dx*dx+dy*dy <= 49){ // radius^2
        lives--;
        asteroidAlive[i]=false;
        explosionSound();
      }

      // Collision with bullet
      if(bulletY>=0){
        int dx2=bulletX-asteroidX[i];
        int dy2=bulletY-asteroidY[i];
        if(dx2*dx2+dy2*dy2 <= 16){ // radius^2
          asteroidAlive[i]=false;
          bulletY=-1;
          score++;
          explosionSound();
        }
      }
    }
  }

  // Collision bullet with Death Star
  if(enemyAlive && bulletY>=0){
    int dx=bulletX-(enemyX+8);
    int dy=bulletY-(enemyY+8);
    if(dx*dx+dy*dy <= 8*8){
      enemyAlive=false;
      bulletY=-1;
      score+=5;
      explosionSound();
    }
  }

  // Respawn Death Star
  if(!enemyAlive){
    delay(150);
    enemyX=random(5,W-20);
    enemyY=10;
    enemyAlive=true;
  }

  // Draw player
  drawShip();

  // Display score & lives
  display.setCursor(2,2);
  display.setTextColor(ST77XX_WHITE);
  display.setTextSize(1);
  display.print("S:");
  display.print(score);
  display.setCursor(60,2);
  display.print("L:");
  display.print(lives);

  delay(40);
}
