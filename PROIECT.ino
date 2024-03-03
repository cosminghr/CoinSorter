#include <Wire.h>

#include <LiquidCrystal_I2C.h>

#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int f1 = 0, f2 = 0, f3 = 0;

int c1 = 0, c2 = 0, c3 = 0;

float suma = 0;

float targetSum = 0;

int servoPin = 7;
Servo Servo1;

int buttonINCPin = 3;
int buttonINCState = HIGH;
int prestateINC = 0;

int buttonDECPin = 2;
int buttonDECState = HIGH;
int prestateDEC = 0;

int buttonUPDATEPin = 4;
int buttonUPDATEState = HIGH;
int prestateUPDATE = 0;


unsigned long lastButtonPressTime = 0;
int revin = 0;

int apas = 0;
enum State {
  SET_TARGET_SUM,
  SORT_COINS,
  UPTATE_SUM
};

State currentState = SET_TARGET_SUM;

void setup()

{

  lcd.init();

  lcd.backlight();

  lcd.setCursor(0, 0);

  lcd.print("  PROIECT PMP");

  lcd.setCursor(0, 1);

  lcd.print("  COIN SORTER       ");

  delay(2000);

  lcd.clear();

  Servo1.attach(servoPin);
  Servo1.write(0);

  pinMode(buttonINCPin, INPUT);
  pinMode(buttonDECPin, INPUT);
  pinMode(buttonUPDATEPin, INPUT);
}

void loop() {
  switch (currentState) {
    case SET_TARGET_SUM:
      setTargetSum();
      break;

    case SORT_COINS:
      sortCoins();
      break;
  }
}

void setTargetSum() {
  if (currentState == SET_TARGET_SUM) {
    buttonINCState = digitalRead(buttonINCPin);
    buttonDECState = digitalRead(buttonDECPin);

    if (revin == 1) {
      lcd.clear();
      lastButtonPressTime = millis();
      revin = 0;
    }

    if (buttonINCState == HIGH && prestateINC == 0) {
      targetSum++;
      prestateINC = 1;
      lastButtonPressTime = millis();
    } else if (buttonINCState == LOW) {
      prestateINC = 0;
    }

    if (buttonDECState == HIGH && prestateDEC == 0 && targetSum > 0.0) {
      targetSum--;
      prestateDEC = 1;
      lastButtonPressTime = millis();
    } else if (buttonDECState == LOW) {
      prestateDEC = 0;
    }

    if (millis() - lastButtonPressTime >= 5000 && targetSum >= 1) {
      lcd.clear();
      currentState = SORT_COINS;
    } else {
      lcd.setCursor(0, 0);
      lcd.print("VREAU SA STRANG:");
      lcd.setCursor(4, 1);
      lcd.print(targetSum);
      lcd.print(" LEI");
    }
  }
}

void updateSum() {
  buttonUPDATEState = digitalRead(buttonUPDATEPin);

  if (buttonUPDATEState == HIGH && prestateUPDATE == 0) {
    lcd.clear();

    currentState = SET_TARGET_SUM;
    prestateUPDATE = 1;
    lastButtonPressTime = millis();
  } else if (buttonUPDATEState == LOW) {
    prestateUPDATE = 0;
  }
}

void sortCoins() {

  int s1 = analogRead(A0);
  int s2 = analogRead(A1);
  int s3 = analogRead(A2);

  printCoins();

  updateSum();

   if (s1 >= 200 && f1 == 0) {
     f1 = 1;
   } else if (s1 < 200 && f1 == 1) {
     f1 = 0;
     c1++;
     countCoins();
     printCoins();
   }
   if (s2 >= 200 && f2 == 0) {
     f2 = 1;
   } else if (s2 < 200 && f2 == 1) {
     f2 = 0;
     c2++;
     countCoins();
     printCoins();
   }
   if (s3 >= 200 && f3 == 0) {
     f3 = 1;
   } else if (s3 < 200 && f3 == 1) {
     f3 = 0;
     c3++;
     countCoins();
     printCoins();
   }

  if (suma >= targetSum) {
    activateServo();
    resetValues();
  }
}

void countCoins() {

  lcd.clear();

  suma = 0.5 * c1 + 0.1 * c2 + 0.05 * c3;

  lcd.setCursor(0, 0);

  lcd.print("  SUMA ADUNATA:");

  lcd.setCursor(3, 1);

  lcd.print(suma);

  lcd.print(" LEI");

  delay(1000);

  lcd.clear();
}


void activateServo() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("BRAVO! AI STRANS:");

  lcd.setCursor(4, 1);
  lcd.print(suma);
  lcd.print(" LEI");

  Servo1.write(380);
  delay(5000);

  Servo1.write(-380);
  delay(1000);

  lcd.clear();
}

void printCoins() {

  lcd.setCursor(0, 0);
  lcd.print("B50 B10 B5 SUMA");

  lcd.setCursor(0, 1);
  lcd.print(c1);

  lcd.setCursor(4, 1);
  lcd.print(c2);

  lcd.setCursor(8, 1);
  lcd.print(c3);

  lcd.setCursor(11, 1);
  lcd.print(suma);

}

void resetValues() {
  targetSum = 0;
  suma = 0;
  c1 = 0;
  c2 = 0;
  c3 = 0;
  revin = 1;
  currentState = SET_TARGET_SUM;
}
