#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU9250_asukiaaa.h>

// OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// MPU setup
MPU9250_asukiaaa mySensor;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (1);
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);

  // MPU init
  mySensor.setWire(&Wire);
  mySensor.beginAccel();
  mySensor.beginGyro();

  display.setCursor(0, 0);
  display.println("MPU9250 Ready!");
  display.display();
  delay(1000);
}

void loop() {
  // Update accelerometer
  mySensor.accelUpdate();
  float ax = mySensor.accelX();
  float ay = mySensor.accelY();
  float az = mySensor.accelZ();

  // Update gyroscope
  mySensor.gyroUpdate();
  float gx = mySensor.gyroX();
  float gy = mySensor.gyroY();
  float gz = mySensor.gyroZ();

  // Clear display
  display.clearDisplay();
  display.setCursor(0, 0);

  // Accelerometer
  display.println("Accel (g):");
  display.print("X: "); display.println(ax, 2);
  display.print("Y: "); display.println(ay, 2);
  display.print("Z: "); display.println(az, 2);

  display.println(); // blank line

  // Gyroscope
  display.println("Gyro (deg/s):");
  display.print("X: "); display.println(gx, 1);
  display.print("Y: "); display.println(gy, 1);
  display.print("Z: "); display.println(gz, 1);

  display.display();
  delay(200);
}
