#include <Key.h>
#include <Keypad.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define IR_SENSOR_PIN 6
#define IRin 0
#define BUZZER_PIN 7
#define SERVO_PIN 8
#define LDR A0
#define LED1_PIN 9
#define LED2_PIN 10
#define rled 12
#define gled 13
#define TEMP_SENSOR_PIN A1
#define FAN_PIN 11
#define buttonPin A3
bool buzzerMuted = false;
const byte ROWS = 2;
const byte COLS = 2; 
char keys[ROWS][COLS] = {
  {'1', '2'},
  {'4', '5'}
};
byte rowPins[ROWS] = {2, 3};
byte colPins[COLS] = {4, 5};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const String PASS = "2222";
Servo doorServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);


String inputpassword ="";
bool doorOpen = false;

void setup() {
  pinMode(IR_SENSOR_PIN, INPUT);
  pinMode(IRin, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(gled, OUTPUT);
  pinMode(rled, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  lcd.print(" ENG/Dalia Home");
  delay(2000);
  doorServo.attach(SERVO_PIN);
  doorServo.write(0); 
  Serial.begin(9600);
  lcd.setCursor(0, 1);
  digitalWrite(rled, 0);
  digitalWrite(gled, 0);


}

void loop() {
  if(digitalRead(IRin) == 0)
  {
  doorServo.write(180);
  delay(3000);
  doorServo.write(0);
  }
  if (digitalRead(IR_SENSOR_PIN) == 0 ) {
    lcd.clear();
    lcd.print("Enter Password");
    handlePassword();
  }
  if (doorOpen) {
    handleLDR();
    handleTemperature();
  }
}

void handlePassword() {
inputpassword = "";
while(true){  
  char key = keypad.getKey();
  if (key) {
    inputpassword += key;
    Serial.println(key);
    if (inputpassword.length() == 4) {
      if (inputpassword == PASS) {
        digitalWrite(gled, 1);
        lcd.clear();
        lcd.print("Welcome Home");
        Serial.println("Password correct");
        doorServo.write(180);
        delay(3000);
        digitalWrite(gled, 0);
        doorServo.write(0);
        doorOpen = true;
        break;
      } else {
        doorServo.write(0);
        digitalWrite(rled, 1);
        lcd.clear();
        lcd.print("Wrong Password");
        lcd.setCursor(0, 1);
        lcd.print("Re-enter:");
        Serial.println("Password incorrect");
        digitalWrite(BUZZER_PIN, HIGH);
        delay(1000);
        digitalWrite(rled, 0);
        digitalWrite(BUZZER_PIN, 0);
      
      }
      inputpassword = "";
      }
    }
  }
}

void handleLDR() {
  if(digitalRead(IRin) == 0)
  {
  doorServo.write(180);
  digitalWrite(gled, 1);
  delay(2000);
  digitalWrite(gled,0);
  doorServo.write(0);
  }
  int brightness = analogRead(LDR);
  lcd.clear();
  lcd.print("Brightness: ");
  lcd.print(brightness);
  if (brightness < 200) {
    digitalWrite(LED1_PIN, 1);
    digitalWrite(LED2_PIN, 1);
  } else {
    digitalWrite(LED1_PIN, 0);
    digitalWrite(LED2_PIN, 0);
  }
  delay(1000);
}
void handleTemperature() {
  if(digitalRead(IRin) == 0)
  {
    digitalWrite(gled, 1);
    doorServo.write(180);
    delay(1000);
    digitalWrite(gled,0);
    doorServo.write(0);
  }
  int tempReading = analogRead(TEMP_SENSOR_PIN);
  float voltage = tempReading * (5.0 / 1023.0);
  float temperature = voltage * 100.0;
  if (digitalRead(buttonPin) == LOW) {
    buzzerMuted = true; 
  }
  if (temperature < 30) {
    digitalWrite(FAN_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  } else if (temperature >= 30 && temperature <= 45) {
    digitalWrite(FAN_PIN, HIGH);
    digitalWrite(BUZZER_PIN, LOW);
  } else {
    digitalWrite(FAN_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    if (!buzzerMuted) {
      digitalWrite(BUZZER_PIN, HIGH); 
    } else {
      digitalWrite(BUZZER_PIN, LOW);  
    }
  }

  lcd.clear();
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");
  delay(1000);
}
