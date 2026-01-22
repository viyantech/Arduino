# 🗑️ Smart Dustbin using Obstacle sensor module 

This project uses an **IR sensor** and a **servo motor** to automatically open and close a lid when a hand or object is detected. It’s ideal for **smart dustbins**, **touchless containers**, or **automation demos** 🤖

---

## 📌 How It Works (Overview)

1. The **IR sensor** detects an object (like a hand).
2. When detected, the **servo motor rotates** to open the lid.
3. The lid stays open for a few seconds.
4. The servo then **closes the lid automatically**.
5. The system waits until the object is removed before triggering again.

---

## 🧰 Components Required

| Component                     | Quantity  |
| ----------------------------- | --------- |
| Arduino Uno / Nano            | 1         |
| IR Sensor Module              | 1         |
| Servo Motor (SG90 or similar) | 1         |
| Jumper Wires                  | As needed |
| Breadboard (optional)         | 1         |

---

## 🔌 Circuit Connections (Step by Step)

### 🔹 IR Sensor

| IR Sensor Pin | Arduino Pin       |
| ------------- | ----------------- |
| VCC           | 5V                |
| GND           | GND               |
| OUT           | Digital Pin **2** |

### 🔹 Servo Motor

| Servo Wire             | Arduino Pin       |
| ---------------------- | ----------------- |
| Red (VCC)              | 5V                |
| Brown/Black (GND)      | GND               |
| Yellow/Orange (Signal) | Digital Pin **9** |

---

## 🧠 Arduino Code Explanation (Step by Step)

### 1️⃣ Include Servo Library

```cpp
#include <Servo.h>
```

This allows Arduino to control the servo motor.

---

### 2️⃣ Create Servo Object

```cpp
Servo lidServo;
```

Used to control lid movement.

---

### 3️⃣ Define Pins

```cpp
int irPin = 2;
int servoPin = 9;
```

* IR sensor output → pin 2
* Servo signal → pin 9

---

### 4️⃣ State Variable

```cpp
bool isOpen = false;
```

Prevents the lid from opening repeatedly while the hand is still present.

---

### 5️⃣ Setup Function

```cpp
void setup() {
  pinMode(irPin, INPUT);
  lidServo.attach(servoPin);
  lidServo.write(0);
}
```

* Sets IR sensor as input
* Attaches servo
* Keeps lid closed initially

---

### 6️⃣ Loop Function

```cpp
void loop() {
  int irValue = digitalRead(irPin);
```

Reads IR sensor value.

---

### 7️⃣ Detect Object & Open Lid

```cpp
if (irValue == LOW && isOpen == false) {
  isOpen = true;

  lidServo.write(180);
  delay(3000);

  lidServo.write(0);
  delay(1000);
}
```

* IR sensor gives **LOW** when an object is detected
* Servo rotates to **180°** (open)
* Waits 3 seconds
* Closes lid

---

### 8️⃣ Reset When Object Is Removed

```cpp
if (irValue == HIGH) {
  isOpen = false;
}
```

Allows the system to work again after the hand is removed.

---

## ▶️ How to Upload the Code

1. Open **Arduino IDE**
2. Connect Arduino via USB
3. Select:

   * **Board:** Arduino Uno
   * **Port:** Correct COM Port
4. Paste the code
5. Click **Upload** ⬆️

---

## ✅ Final Output

* 👋 Hand detected → Lid opens
* ⏳ Waits 3 seconds
* 🔒 Lid closes automatically
* 🔁 Ready for next detection

---

## 🚀 Applications

* Smart Dustbin
* Touchless Containers
* Hygiene Automation
* Arduino Learning Projects

---

## 📜 License

This project is open-source and free to use for learning and development purposes.

---