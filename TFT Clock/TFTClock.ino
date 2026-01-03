#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Fonts/FreeMono9pt7b.h>
#include <SPI.h>

// -------- TFT Pins --------
#define TFT_CS   10
#define TFT_RST  9
#define TFT_DC   8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// -------- Time & Date --------
int hour24 = 12;
int minute = 34;
int second = 0;

int day = 3;
int month = 1;
int year = 2026;
int weekday = 3; // 0=Sun

bool blinkColon = true;
unsigned long lastMillis = 0;

const char* daysOfWeek[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
const int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};

// -------- Leap Year --------
bool isLeapYear(int y) {
  return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

void setup() {
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);          // HORIZONTAL
  tft.fillScreen(ST77XX_BLACK);
  tft.setFont(&FreeMono9pt7b);
}

void loop() {
  if (millis() - lastMillis >= 500) {
    lastMillis = millis();
    blinkColon = !blinkColon;

    if (blinkColon) tickTime();
    drawClock();
  }
}

// -------- Time Logic --------
void tickTime() {
  second++;
  if (second >= 60) { second = 0; minute++; }
  if (minute >= 60) { minute = 0; hour24++; }
  if (hour24 >= 24) {
    hour24 = 0;
    day++;
    weekday = (weekday + 1) % 7;

    int maxDays = daysInMonth[month - 1];
    if (month == 2 && isLeapYear(year)) maxDays = 29;

    if (day > maxDays) {
      day = 1;
      month++;
      if (month > 12) {
        month = 1;
        year++;
      }
    }
  }
}

// -------- Draw Display --------
void drawClock() {
  tft.fillScreen(ST77XX_BLACK);

  // Seconds Progress Bar
  int barWidth = map(second, 0, 59, 0, 128);
  tft.fillRect(0, 2, barWidth, 4, ST77XX_RED);

  // 12-hour format
  int hour12 = hour24 % 12;
  if (hour12 == 0) hour12 = 12;
  bool isPM = hour24 >= 12;

  // Time String
  char timeStr[15];
  if (blinkColon)
    sprintf(timeStr, "%02d:%02d:%02d %s", hour12, minute, second, isPM ? "PM" : "AM");
  else
    sprintf(timeStr, "%02d %02d %02d %s", hour12, minute, second, isPM ? "PM" : "AM");

  centerTextBold(timeStr, 34, ST77XX_GREEN);

  // Date String
  char dateStr[25];
  sprintf(dateStr, "%s %02d/%02d/%04d",
          daysOfWeek[weekday], day, month, year);

  centerTextBold(dateStr, 58, ST77XX_CYAN);
}

// -------- Centered + LITTLE BOLD Text --------
void centerTextBold(const char* text, int y, uint16_t color) {
  int16_t x1, y1;
  uint16_t w, h;

  tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  int x = (128 - w) / 2;

  tft.setTextColor(color);

  // Draw twice for light bold effect
  tft.setCursor(x, y);
  tft.print(text);

  tft.setCursor(x + 1, y);
  tft.print(text);
}
