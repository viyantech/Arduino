#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include "DHT.h"
#include "Adafruit_CCS811.h"
#include "time.h"

/* ================= WiFi ================= */
const char* ssid = "WiFi-9427";
const char* password = "33168432";

/* ================= Time ================= */
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 36000; // GMT+10
const int daylightOffset_sec = 0;

/* ================= Sensors ================= */
#define DHTPIN 4
#define DHTTYPE DHT11
#define UV_PIN 1  // UV analog sensor

DHT dht(DHTPIN, DHTTYPE);
Adafruit_CCS811 ccs;

WebServer server(80);

/* ================= Status Function ================= */
String getStatus(float value, float g, float o, float w) {
  if (value <= g) return "<span class='good'>GOOD</span>";
  if (value <= o) return "<span class='ok'>OK</span>";
  if (value <= w) return "<span class='warn'>WARNING</span>";
  return "<span class='poor'>POOR</span>";
}

/* ================= Web Page ================= */
String webpage(float t, float h, uint16_t co2, uint16_t tvoc, float uv) {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>AirSense Mini</title>

<style>
body{
  background:#000;
  color:#fff;
  font-family:Arial, Helvetica, sans-serif;
  margin:0;
  padding:0;
  -webkit-tap-highlight-color: transparent;
}
.header{
  text-align:center;
  font-size: clamp(20px, 6vw, 28px);
  padding:15px;
  font-weight:bold;
}
.grid{
  display:grid;
  grid-template-columns: repeat(auto-fit, minmax(180px, 1fr));
  gap:16px;
  padding:20px;
}
.card{
  background:#050505;
  border-radius:18px;
  padding:18px;
  text-align:center;
  box-shadow:0 0 12px #fff;
  border:1px solid #fff;
}
.label{color:#aaa; font-size: clamp(10px, 2.5vw, 14px); margin-bottom:6px;}
.value{font-size: clamp(24px,5vw,48px); font-weight:bold; word-wrap:break-word;}
.unit{font-size: clamp(12px,3vw,18px); color:#bbb;}
.icon{font-size: clamp(20px,5vw,38px); display:block; margin-bottom:6px;}

.temp{color:#ffd000;}
.humi{color:#2dd4bf;}
.co2{color:#ff4d4d;}
.tvoc{color:#facc15;}
.uv{color:#a855f7;}

.good{color:#22c55e;}
.ok{color:#facc15;}
.warn{color:#fb923c;}
.poor{color:#ef4444;}

.aqi{
  height:18px;
  display:flex;
  margin:10px 20px;
  border-radius:10px;
  overflow:hidden;
}
.aqi div{flex:1;}
.g{background:#22c55e;}
.y{background:#facc15;}
.o{background:#fb923c;}
.r{background:#ef4444;}
.p{background:#7c3aed;}

.footer{
  text-align:center;
  color:#aaa;
  padding:10px;
  font-size: clamp(10px,2.5vw,14px);
}

/* Responsive Clock */
#clockCanvas {
  width: 100%;
  height: auto;
  max-width: 250px;
  aspect-ratio: 1/1;
  background:#050505;
  border-radius:50%;
  margin:0 auto;
  display:block;
}
#dateText {
  margin-top:8px;
  font-size:14px;
  text-align:center;
  font-weight:bold;
  color:#aaa;
}
</style>
</head>

<body>

<div class="header">AirSense Mini</div>

<div class="grid">

  <!-- Clock Card Matching Sensor Cards -->
  <div class="card">
    <canvas id="clockCanvas"></canvas>
    <div id="dateText"></div>
  </div>

  <div class="card">
    <span class="icon co2">🫁</span>
    <div class="label">CO₂</div>
    <div class="value co2">)rawliteral" + String(co2) + R"rawliteral(</div>
    <div class="unit">PPM</div>
    )rawliteral" + getStatus(co2, 800, 1200, 2000) + R"rawliteral(
  </div>

  <div class="card">
    <span class="icon tvoc">🧪</span>
    <div class="label">TVOC</div>
    <div class="value tvoc">)rawliteral" + String(tvoc) + R"rawliteral(</div>
    <div class="unit">PPB</div>
    )rawliteral" + getStatus(tvoc, 150, 300, 500) + R"rawliteral(
  </div>

  <div class="card">
    <span class="icon temp">🌡</span>
    <div class="label">TEMP</div>
    <div class="value temp">)rawliteral" + String(t,1) + R"rawliteral(</div>
    <div class="unit">°C</div>
    )rawliteral" + getStatus(t, 26, 30, 35) + R"rawliteral(
  </div>

  <div class="card">
    <span class="icon humi">💧</span>
    <div class="label">HUMI</div>
    <div class="value humi">)rawliteral" + String(h,1) + R"rawliteral(</div>
    <div class="unit">%</div>
    )rawliteral" + getStatus(h, 60, 70, 85) + R"rawliteral(
  </div>

  <div class="card">
    <span class="icon uv">☀️</span>
    <div class="label">UV INDEX</div>
    <div class="value uv">)rawliteral" + String(uv,1) + R"rawliteral(</div>
    )rawliteral" + getStatus(uv, 2, 5, 7) + R"rawliteral(
  </div>

</div>

<div class="aqi">
  <div class="g"></div><div class="y"></div><div class="o"></div><div class="r"></div><div class="p"></div>
</div>

<div class="footer">
&copy; 2026 Sevviyan • AirSense Mini • ESP32-C3 • VIYAN TECH
</div>

<script>
function drawClock() {
  const canvas = document.getElementById('clockCanvas');
  const ctx = canvas.getContext('2d');

  function resizeCanvas() {
    const size = canvas.clientWidth;
    canvas.width = size;
    canvas.height = size;
  }
  window.addEventListener('resize', resizeCanvas);
  resizeCanvas();

  const radius = canvas.height / 2;
  ctx.translate(radius, radius);

  function drawFace(ctx, radius) {
    ctx.beginPath();
    ctx.arc(0, 0, radius, 0, 2*Math.PI);
    ctx.fillStyle = '#050505';
    ctx.fill();

    ctx.strokeStyle = '#fff';
    ctx.lineWidth = radius*0.05;
    ctx.stroke();

    // Numbers in white
    ctx.font = radius*0.15 + "px Arial";
    ctx.fillStyle = '#fff';
    ctx.textBaseline = "middle";
    ctx.textAlign = "center";
    for(let num=1; num<=12; num++){
      let ang = num * Math.PI/6;
      ctx.rotate(ang);
      ctx.translate(0, -radius*0.85);
      ctx.rotate(-ang);
      ctx.fillText(num.toString(), 0, 0);
      ctx.rotate(ang);
      ctx.translate(0, radius*0.85);
      ctx.rotate(-ang);
    }
  }

  function drawHand(ctx, pos, length, width, color) {
    ctx.beginPath();
    ctx.lineWidth = width;
    ctx.lineCap = "round";
    ctx.strokeStyle = color;
    ctx.moveTo(0,0);
    ctx.rotate(pos);
    ctx.lineTo(0, -length);
    ctx.stroke();
    ctx.rotate(-pos);
  }

  function drawTime(ctx, radius) {
    const now = new Date();
    const hour = now.getHours();
    const minute = now.getMinutes();
    const second = now.getSeconds();

    drawHand(ctx, ((hour%12)+minute/60+second/3600)*Math.PI/6, radius*0.5, radius*0.07, '#fff'); // hour white
    drawHand(ctx, (minute+second/60)*Math.PI/30, radius*0.75, radius*0.05, '#fff'); // minute white
    drawHand(ctx, second*Math.PI/30, radius*0.85, radius*0.02, '#ff4d4d'); // second red

    // Day & date in gray like card labels
    const options = { weekday: 'long', year: 'numeric', month: 'long', day: 'numeric' };
    const dateStr = now.toLocaleDateString('en-US', options);
    const dateText = document.getElementById('dateText');
    dateText.style.color = '#aaa';
    dateText.style.fontWeight = 'bold';
    dateText.innerHTML = dateStr;
  }

  setInterval(() => {
    ctx.clearRect(-radius, -radius, canvas.width, canvas.height);
    drawFace(ctx, radius);
    drawTime(ctx, radius);
  }, 1000);
}

drawClock();
</script>

</body>
</html>
)rawliteral";

  return html;
}

/* ================= Handler ================= */
void handleRoot() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  uint16_t co2 = 0, tvoc = 0;
  float uv = analogRead(UV_PIN) * (3.3 / 4095.0) * 10.0;

  if (ccs.available() && !ccs.readData()) {
    co2 = ccs.geteCO2();
    tvoc = ccs.getTVOC();
  }

  server.send(200, "text/html", webpage(t, h, co2, tvoc, uv));
}

/* ================= Setup ================= */
void setup() {
  Serial.begin(115200);
  dht.begin();
  Wire.begin();
  ccs.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  server.on("/", handleRoot);
  server.begin();
}

/* ================= Loop ================= */
void loop() {
  server.handleClient();
}
