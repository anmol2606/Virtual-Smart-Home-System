#include <Servo.h>
#include <Keypad.h>

Servo doorServo;

const int lightPin = 3;
const int fanPin = 5;
const int tempPin = A0;
const int buttonPin = 2;
int buttonState = 0;

const String password = "1234";
String input = "";

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13, 12, 11, 10}; 
byte colPins[COLS] = {9, 8, 7, 6}; 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  pinMode(lightPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  doorServo.attach(4);
  doorServo.write(0);  // door locked
  Serial.begin(9600);
}

void loop() {
  // Light Control
  int buttonState = digitalRead(buttonPin); // HIGH = not pressed, LOW = pressed

  if (buttonState == LOW) {
    digitalWrite(lightPin, HIGH); // LED ON
  } else {
    digitalWrite(lightPin, LOW);  // LED OFF
  }

  // Fan Control
  int tempValue = analogRead(tempPin);
  float voltage = tempValue * (5.0 / 1023.0);
  float temperature = (voltage - 0.5) * 100;  // For TMP36
  Serial.print("Temp: ");
  Serial.println(temperature);
  if (temperature > 30) {
    digitalWrite(fanPin, HIGH);
  } else {
    digitalWrite(fanPin, LOW);
  }

  // Door Lock
  char key = keypad.getKey();
  if (key) {
    Serial.print("Key pressed: ");
    Serial.println(key);
    if (key == '#') {
      if (input == password) {
        Serial.println("Access Granted");
        doorServo.write(90); // unlock
        delay(3000);
        doorServo.write(0);  // lock again
      } else {
        Serial.println("Wrong Password");
      }
      input = "";  // reset
    } else if (key == '*') {
      input = "";  // clear
    } else {
      input += key;
    }
  }

  delay(100);
}
