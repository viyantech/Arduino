#include <Wire.h>
#include <MPU9250_asukiaaa.h>

#define SDA_PIN 8
#define SCL_PIN 9

MPU9250_asukiaaa mySensor;

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  mySensor.setWire(&Wire);
  mySensor.beginAccel();
  mySensor.beginGyro();

  Serial.println("MPU9250 ready");
}

void loop() {
  mySensor.accelUpdate();

  Serial.print(mySensor.accelX());
  Serial.print(",");
  Serial.print(mySensor.accelY());
  Serial.print(",");
  Serial.println(mySensor.accelZ());

  delay(50);
}