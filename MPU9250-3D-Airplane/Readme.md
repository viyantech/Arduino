# ESP32-C3 MPU9250 3D Airplane Visualization

This project uses an **ESP32-C3 Super Mini** with an **MPU9250 accelerometer/gyroscope** to control a **3D airplane visualization** in Processing. The airplane rotates in real-time according to the sensor’s orientation.

---

## **Hardware Required**

* ESP32-C3 Super Mini board
* MPU9250 sensor module
* Breadboard and jumper wires (optional, for prototyping)
* USB cable for ESP32-C3

---

## **Wiring Diagram**

| ESP32-C3 Pin | MPU9250 Pin | Notes          |
| ------------ | ----------- | -------------- |
| 8 (GPIO8)    | SDA         | I2C data line  |
| 9 (GPIO9)    | SCL         | I2C clock line |
| 3.3V         | VCC         | Power (3.3V)   |
| GND          | GND         | Ground         |

**Optional:** Add 4.7kΩ pull-up resistors to SDA and SCL if the sensor is unstable.

---

## **Arduino Code**

```cpp
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
```

**Explanation:**

* Initializes I2C on pins 8 (SDA) and 9 (SCL) for ESP32-C3.
* Reads accelerometer values from MPU9250 every 50 ms (~20 Hz).
* Sends `X, Y, Z` accelerometer data over Serial to Processing.

---

## **Processing Code**

```java
import processing.serial.*;

Serial port;
float ax = 0, ay = 0, az = 0;

void setup() {
  size(800, 600, P3D);
  smooth(8);
  lights();

  println(Serial.list());
  port = new Serial(this, Serial.list()[1], 115200);
  port.bufferUntil('\n');
}

void draw() {
  background(0);

  displayLeftPanel();
  displayRightPanel();

  translate(width/2, height/2, 0);
  scale(2.0);

  rotateX(radians(-ay * 2));
  rotateZ(radians(-ax * 2));

  drawAirplane();
}

// Serial data reading
void serialEvent(Serial port) {
  String line = port.readStringUntil('\n');
  if (line == null) return;

  line = trim(line);
  String[] v = split(line, ',');

  if (v.length == 3) {
    ax = lerp(ax, float(v[0]), 0.15);
    ay = lerp(ay, float(v[1]), 0.15);
    az = lerp(az, float(v[2]), 0.15);
  }
}

// Airplane drawing
void drawAirplane() {
  fill(255, 165, 0);
  box(20, 20, 400);

  pushMatrix();
  translate(0, 0, -240);
  fill(0, 0, 255);
  rotateX(PI/2);
  drawCylinder(0, 40, 40, 12);
  popMatrix();

  fill(34, 139, 34);
  beginShape(TRIANGLES);
  vertex(-200,  4, 60); vertex(0,  4, -160); vertex(200,  4, 60);
  vertex(-200, -4, 60); vertex(0, -4, -160); vertex(200, -4, 60);
  vertex(-4, 0, 196); vertex(-4, -60, 196); vertex(-4, 0, 140);
  vertex( 4, 0, 196); vertex( 4, -60, 196); vertex( 4, 0, 140);
  endShape();
}

// Cylinder for airplane nose
void drawCylinder(float topRadius, float bottomRadius, float tall, int sides) {
  float angle = 0;
  float step = TWO_PI / sides;

  beginShape(QUAD_STRIP);
  for (int i = 0; i <= sides; i++) {
    vertex(topRadius*cos(angle), 0, topRadius*sin(angle));
    vertex(bottomRadius*cos(angle), tall, bottomRadius*sin(angle));
    angle += step;
  }
  endShape();
}

// Left panel
void displayLeftPanel() {
  pushMatrix();
  camera();
  hint(DISABLE_DEPTH_TEST);

  fill(50, 50, 50, 220);
  rect(5, 5, 180, 120, 10);

  fill(255);
  textSize(20);
  text("  AX: " + nf(ax,1,2), 70, 40);
  text("  AY: " + nf(ay,1,2), 70, 70);
  text("  AZ: " + nf(az,1,2), 70, 100);

  hint(ENABLE_DEPTH_TEST);
  popMatrix();
}

// Right panel
void displayRightPanel() {
  pushMatrix();
  camera();
  hint(DISABLE_DEPTH_TEST);

  fill(255, 0, 255);
  textSize(32);
  textAlign(RIGHT, TOP);
  text("ViyanTronics", width-10, 20);

  hint(ENABLE_DEPTH_TEST);
  popMatrix();
}
```

**Explanation:**

* Reads Serial data from ESP32-C3.
* Smoothly updates accelerometer values using `lerp`.
* Rotates 3D airplane based on `ax` (roll) and `ay` (pitch).
* Left panel shows `AX, AY, AZ`.
* Right panel displays branding.

---

## **Setup Instructions**

1. **Install Arduino IDE**

   * Download from [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)

2. **Install ESP32 Board Support**

   * File → Preferences → Additional Board Manager URLs:

     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   * Tools → Board → Board Manager → Install “esp32 by Espressif Systems”

3. **Install MPU9250_asukiaaa Library**

   * Sketch → Include Library → Manage Libraries → Search `MPU9250_asukiaaa` → Install

4. **Connect Hardware**

   * Wire SDA, SCL, 3.3V, GND as shown in the wiring table above

5. **Upload Arduino Code**

   * Select board: “ESP32C3 Dev Module”
   * Select correct COM port
   * Upload code

6. **Install Processing IDE**

   * Download from [https://processing.org/download/](https://processing.org/download/)

7. **Run Processing Sketch**

   * Open the Processing `.pde` file
   * Select the correct serial port (matching your ESP32-C3)
   * Run → The airplane should rotate according to MPU9250 motion

---

## **How It Works**

1. **Arduino Code**

   * Reads accelerometer values (`ax, ay, az`)
   * Sends them over Serial as CSV every 50 ms

2. **Processing Code**

   * Reads Serial values
   * Smoothly interpolates to avoid jitter
   * Applies rotations to the 3D airplane
   * Displays AX, AY, AZ and branding panels

---

## **Troubleshooting**

| Problem               | Solution                                                             |
| --------------------- | -------------------------------------------------------------------- |
| Serial port not found | Check ESP32-C3 COM port in Arduino IDE; update Processing port index |
| Airplane not moving   | Verify wiring (SDA/SCL) and ensure 3.3V power                        |
| Serial data incorrect | Check baud rate in Arduino (`115200`) matches Processing             |
| Sensor unstable       | Add 4.7kΩ pull-up resistors on SDA and SCL                           |

---

This README is now **fully detailed**, with installation steps, wiring, and explanations for both Arduino and Processing code.

--- 