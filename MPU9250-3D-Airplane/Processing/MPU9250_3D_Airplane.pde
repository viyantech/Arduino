import processing.serial.*;

Serial port;

float ax = 0;
float ay = 0;
float az = 0;

void setup() {
  size(800, 600, P3D); // bigger canvas for larger jet
  smooth(8);
  lights();

  println(Serial.list());
  port = new Serial(this, Serial.list()[1], 115200);
  port.bufferUntil('\n');
}

void draw() {
  background(0);

  // Display panels
  displayLeftPanel();
  displayRightPanel();

  translate(width/2, height/2, 0);

  // Scale up jet size
  scale(2.0);

  // Apply rotations (pitch & roll)
  rotateX(radians(-ay * 2)); // up/down (pitch)
  rotateZ(radians(-ax * 2)); // left/right tilt (roll)

  drawAirplane();
}

// ---------------------------
// Read serial data
// ---------------------------
void serialEvent(Serial port) {
  String line = port.readStringUntil('\n');
  if (line == null) return;

  line = trim(line);
  String[] v = split(line, ',');

  if (v.length == 3) {
    ax = lerp(ax, float(v[0]), 0.15); // smooth X
    ay = lerp(ay, float(v[1]), 0.15); // smooth Y
    az = lerp(az, float(v[2]), 0.15); // smooth Z
  }
}

// ---------------------------
// Airplane model
// ---------------------------
void drawAirplane() {
  // Body
  fill(255, 165, 0); // orange
  box(20, 20, 400); // bigger body

  // Nose
  pushMatrix();
  translate(0, 0, -240);
  fill(0, 0, 255); // blue nose
  rotateX(PI/2);
  drawCylinder(0, 40, 40, 12); // bigger nose
  popMatrix();

  // Wings + tail
  fill(34, 139, 34); // green wings
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

// ---------------------------
// Left-side panel: AX, AY, AZ
// ---------------------------
void displayLeftPanel() {
  pushMatrix();
  camera(); // reset for 2D overlay
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

// ---------------------------
// Right-side panel: YouTube Channel
// ---------------------------
void displayRightPanel() {
  pushMatrix();
  camera();
  hint(DISABLE_DEPTH_TEST);

  fill(255, 0, 255); // bright magenta
  textSize(32);
  textAlign(RIGHT, TOP);
  text("ViyanTronics", width-10, 20);

  hint(ENABLE_DEPTH_TEST);
  popMatrix();
}
