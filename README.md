# smartDustbin


## 🗑️ Smart Dustbin using Arduino

This project is a simple **Smart Dustbin** system built using an Arduino microcontroller and an ultrasonic sensor. It automates the opening of a dustbin lid when a hand or object is detected near it, providing a touch-free and hygienic way of disposing of waste.

### 📸 Features

- Automatic lid opening using ultrasonic sensing
- Non-contact operation to maintain hygiene
- Power efficient — only activates when needed
- Compact and easy to install

---

### 🛠️ Hardware Components

- Arduino Uno or any compatible board
- Ultrasonic Sensor (HC-SR04)
- Servo Motor (SG90 recommended)
- Jumper Wires
- Breadboard (optional)
- Power supply (USB or battery)

---

### 🔌 Circuit Diagram

| Component      | Arduino Pin |
|----------------|-------------|
| Ultrasonic TRIG | Digital 9   |
| Ultrasonic ECHO | Digital 8   |
| Servo Motor     | Digital 7   |
| VCC             | 5V          |
| GND             | GND         |

> Ensure the ultrasonic sensor is mounted such that it can detect an approaching hand or object within ~20cm range.

---

### 💾 Installation & Upload

1. Open the Arduino IDE.
2. Connect your Arduino board to your computer via USB.
3. Open the `smart_dustbin.ino` file.
4. Select the correct board and COM port from **Tools**.
5. Click on **Upload**.

---

### ⚙️ How It Works

- The ultrasonic sensor continuously monitors the distance in front of the dustbin.
- When an object is detected within 20 cm, the servo motor rotates to open the lid.
- After a short delay (usually 2 seconds), the lid automatically closes.

---

### 📋 Sample Code Snippet

```cpp
if (distance < 20) {
  servo.write(90);  // Open lid
  delay(2000);
  servo.write(0);   // Close lid
}
```

---

### 🧠 Applications

- Household waste bins
- Office or public smart bins
- Hospitals or labs for hygienic waste disposal

---

### 📝 License

This project is open-source and available for modification or distribution under the [MIT License](https://opensource.org/licenses/MIT).
