#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// OLED config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// DHT config
#define DHTPIN 2
#define DHTTYPE DHT11   // Change to DHT22 if needed
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while(true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("Temp:");
  display.print(temperature);
  display.print("C");

  display.setCursor(0, 32);
  display.print("Hum:");
  display.print(humidity);
  display.print("%");

  display.display();
  delay(2000);
}
