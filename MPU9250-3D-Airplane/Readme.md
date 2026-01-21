# Real-Time 3D Airplane Simulation Using MPU9250 and Processing

Control a 3D airplane model on your PC using real-world motion with an MPU9250 sensor, Arduino, and Processing.  
This project is a hands-on way to learn electronics, programming, and 3D visualization.

---

## 1. Components Required

**Arduino Board (Uno/Nano/Mega)**  
Quantity: 1  
Notes: Any standard Arduino board  

**MPU9250 9-Axis IMU Sensor**  
Quantity: 1  
Notes: Accelerometer + Gyroscope + Magnetometer  

**Jumper Wires**  
Quantity: 4–6  
Notes: Male-to-female or male-to-male depending on sensor module  

**Breadboard**  
Quantity: 1  
Notes: Optional, for easier wiring  

**USB Cable**  
Quantity: 1  
Notes: For Arduino-PC connection  

**Computer**  
Quantity: 1  
Notes: For Arduino IDE and Processing IDE  

**Cardboard or small case**  
Quantity: 1  
Notes: Optional, for holding the sensor  

---

## 2. Installing the Arduino IDE

1. Visit https://www.arduino.cc/en/software  
2. Download the version for your OS  
3. Install the software  
4. Connect your Arduino and verify it appears under **Tools → Port**

---

## 3. Installing the Processing IDE

1. Visit https://processing.org/download  
2. Download and install Processing  
3. Open Processing to confirm it runs correctly

---

## 4. Installing Libraries

### Arduino Libraries
- **MPU9250_asukiaaa**  
  - Arduino IDE → *Sketch → Include Library → Manage Libraries*  
  - Search for **MPU9250_asukiaaa** and install  
- **Wire** (pre-installed)

### Processing Libraries
- **Serial** (included by default)

---

## 5. Wiring the MPU9250 Sensor

**VCC → 5V**  
Description: Power supply  

**GND → GND**  
Description: Ground  

**SDA → D8**  
Description: I²C Data  

**SCL → D9**  
Description: I²C Clock  

Optional pins (AD0, FSYNC, INT) can remain unconnected.

---
## 6. Arduino Code
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

  delay(50); // 20 readings per second
}
```

## 7. Processing Code
```cpp
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
  rotateX(radians(-ay * 2)); // pitch
  rotateZ(radians(-ax * 2)); // roll
  drawAirplane();
}

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

// Airplane model
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
  fill(50,50,50,220);
  rect(5,5,180,120,10);
  fill(255);
  textSize(20);
  text("  AX: "+nf(ax,1,2),70,40);
  text("  AY: "+nf(ay,1,2),70,70);
  text("  AZ: "+nf(az,1,2),70,100);
  hint(ENABLE_DEPTH_TEST);
  popMatrix();
}

// Right panel
void displayRightPanel() {
  pushMatrix();
  camera();
  hint(DISABLE_DEPTH_TEST);
  fill(255,0,255);
  textSize(32);
  textAlign(RIGHT, TOP);
  text("ViyanTronics", width-10, 20);
  hint(ENABLE_DEPTH_TEST);
  popMatrix();
}

```
---
 

## 8. Possible Enhancements

- Use gyroscope data for smoother rotation  
- Add magnetometer for yaw control  
- Add terrain or sky background  
- Build a mini flight simulator with keyboard controls  

---

## 9. Conclusion

This project is a fun and educational way to explore sensors, Arduino programming, and 3D visualization.  
By combining MPU9250 motion data with Processing graphics, you can turn real-world movement into an interactive virtual simulation.