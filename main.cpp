#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// LED Pins
const int greenLED = 18;
const int yellowLED = 7;
const int redLED = 19;

// Button Pins
const int button1 = 0;  // Change from GPIO 0 to GPIO 4
const int button2 = 1;  // Change from GPIO 1 to GPIO 5

bool pedestrianWaiting = false;
unsigned long lastPressTime = 0;

void setup() {
    Serial.begin(115200);
    Wire.begin(9, 8);

    pinMode(greenLED, OUTPUT);
    pinMode(yellowLED, OUTPUT);
    pinMode(redLED, OUTPUT);
    pinMode(button1, INPUT_PULLUP);
    pinMode(button2, INPUT_PULLUP);

    Serial.println("We are ready");

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 10);
    display.println("Traffic light ready");
    display.display();
}

// Traffic Light Function
void TrafficLightSys() {
    if (pedestrianWaiting) {
        Serial.println("Pedestrian button pressed! Switching to red.");
        digitalWrite(greenLED, LOW);
        digitalWrite(yellowLED, LOW);
        digitalWrite(redLED, HIGH);
        delay(3000);  // Pedestrian crossing time

        pedestrianWaiting = false;  // Reset pedestrian request
    }

    Serial.println("Normal traffic cycle running...");
    digitalWrite(redLED, HIGH);
    delay(3000);
    
    digitalWrite(yellowLED, HIGH);
    digitalWrite(redLED, LOW);
    delay(1500);

    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, HIGH);
    delay(3000);

    digitalWrite(yellowLED, HIGH);
    digitalWrite(greenLED, LOW);
    delay(1500);

    digitalWrite(yellowLED, LOW);
}

void loop() {
    if (digitalRead(button1) == LOW) {
        if (millis() - lastPressTime > 300) {  // Debounce logic
            lastPressTime = millis();
            Serial.println("Button Press Detected!");
            pedestrianWaiting = true;
        }
    }

    TrafficLightSys();
}
