#include <Wire.h>
#include <Adafruit_SSD1306.h> // For OLED display
#include <Keypad.h>           // For 4x4 keypad

// OLED display configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32 // Modified for 128x32 display
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pin configuration
const int trigPin = 2;     // Ultrasonic sensor trig pin
const int echoPin = 3;     // Ultrasonic sensor echo pin
const int relayPin = 4;    // Relay pin for door lock
const int irSensor1 = 5;   // IR sensor 1
const int irSensor2 = 6;   // IR sensor 2
const int buzzerPin = 7;   // Buzzer pin

// Keypad configuration
const byte ROWS = 4;       // Four rows
const byte COLS = 4;       // Four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {8, 9, 10, 11};  // Connect to the row pinouts of the keypad
byte colPins[COLS] = {12, A0, A1, A2}; // Connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Variables
const int predefinedDistance = 10; // Distance in cm to lock the bin
const int irThreshold = 20;        // Distance in cm for IR sensors
String password = "1234";          // Predefined password
String inputPassword = "";
bool isLocked = true;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Initialize OLED
  if (!oled.begin(SSD1306_I2C_ADDRESS, 0x3C)) {
    Serial.println("OLED initialization failed!");
    while (1);
  }
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);
  oled.display();

  // Initialize pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(irSensor1, INPUT);
  pinMode(irSensor2, INPUT);
  pinMode(buzzerPin, OUTPUT);

  // Lock the bin initially
  lockBin();
}

void loop() {
  // Measure distance using ultrasonic sensor
  int distance = measureDistance();

  // Display scrolling text on OLED
  testScrollText();

  // Check if the bin is full
  if (distance < predefinedDistance) {
    lockBin();
  }

  // Check if waste is placed beside the bin
  bool wasteBeside = checkWasteBeside();
  if (wasteBeside) {
    soundBuzzer();
  } else {
    noTone(buzzerPin);
  }

  // Handle keypad input
  char key = keypad.getKey();
  if (key) {
    if (key == '#') {
      if (inputPassword == password) {
        unlockBin();
      } else {
        oled.clearDisplay();
        oled.setCursor(0, 0);
        oled.print("Access Denied");
        oled.display();
        delay(2000);
      }
      inputPassword = ""; // Reset password input
    } else if (key == '*') {
      inputPassword = ""; // Clear input
    } else {
      inputPassword += key; // Append key to password
    }
  }

  delay(100);
}

void testScrollText() {
  oled.clearDisplay();
  oled.setTextSize(2); // Set text size
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(10, 0);
  oled.println("Smart Bin");
  oled.display();
  delay(100);

  oled.startscrollright(0x00, 0x0F);
  delay(2000);
  oled.stopscroll();
  delay(1000);
  oled.startscrollleft(0x00, 0x0F);
  delay(2000);
  oled.stopscroll();
  delay(1000);
  oled.startscrolldiagright(0x00, 0x07);
  delay(2000);
  oled.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  oled.stopscroll();
  delay(1000);
}

// Function to measure distance using ultrasonic sensor
int measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  int duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2; // Convert to cm
  return distance;
}

// Function to check waste beside the bin
bool checkWasteBeside() {
  bool sensor1Triggered = digitalRead(irSensor1) == LOW;
  bool sensor2Triggered = digitalRead(irSensor2) == LOW;
  return sensor1Triggered || sensor2Triggered;
}

// Function to lock the bin
void lockBin() {
  digitalWrite(relayPin, HIGH); // Activate relay to lock
  isLocked = true;
}

// Function to unlock the bin
void unlockBin() {
  digitalWrite(relayPin, LOW); // Deactivate relay to unlock
  isLocked = false;
}

// Function to sound the buzzer
void soundBuzzer() {
  tone(buzzerPin, 1000); // Sound the buzzer at 1kHz
}
