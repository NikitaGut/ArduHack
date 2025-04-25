#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "SPI.h"
#include "MFRC522.h"

// RFID
#define RST_PIN 9 // RES pin
#define SS_PIN  10 // SDA (SS) pin

// RFID
byte readCard[4];
String tagID = "";
MFRC522 mfrc522(SS_PIN, RST_PIN);

//piezo
int piezoPin = 8;

//buttons
const int buttonPin1 = 2;
const int buttonPin2 = 3;
const int selectPin = 4;

int buttonState1 = 0;
int buttonState2 = 0;
int selectState = 0;

int mode = 1;

// Set the LCD address to 0x27 in PCF8574 by NXP and Set to 0x3F in PCF8574A by Ti
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte customChar[] = {
  0x04,
  0x00,
  0x15,
  0x1E,
  0x16,
  0x15,
  0x00,
  0x00
};

byte t[] = {
  0x1F,
  0x04,
  0x04,
  0x04,
  0x04,
  0x00,
  0x00,
  0x00
};

byte m[] = {
  0x11,
  0x1B,
  0x15,
  0x11,
  0x11,
  0x00,
  0x00,
  0x00
};

byte wf[] = {
  0x00,
  0x04,
  0x0E,
  0x11,
  0x04,
  0x0A,
  0x00,
  0x04
};

void end() {
  lcd.clear();
  lcd.home();
  lcd.print("    SET MODE");
}

void setup() {
  SPI.begin();
  mfrc522.PCD_Init();
  
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(selectPin, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  //lcd.begin();
  lcd.createChar(0, customChar);
  lcd.createChar(1, t);
  lcd.createChar(2, m);
  lcd.createChar(3, wf);
  lcd.home();
  lcd.write(0);
  lcd.setCursor(1, 0);
  lcd.print(" NIK COMPANY");
  lcd.write(1);
  lcd.write(2);
  delay(3000);

  lcd.clear();
  lcd.home();
  lcd.print("Hello,");
  lcd.setCursor(0, 1);
  lcd.print("Nikita!");
  delay(3000);

  lcd.clear();
  lcd.home();
  lcd.print("    SET MODE");
}

void loop() { 
  delay(150);
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  selectState = digitalRead(selectPin);

  if(buttonState1 == HIGH) {
    tone(piezoPin, 1000, 100);
    lcd.clear();
    lcd.home();
    lcd.print("    SET MODE");
    mode -= 1;
  }

  if(buttonState2 == HIGH) {
    tone(piezoPin, 1000, 100);
    lcd.clear();
    lcd.home();
    lcd.print("    SET MODE");
    mode += 1;
  }

  switch(mode) {
    case 1: lcd.setCursor(0, 1); lcd.print("Wi-Fi hacking"); lcd.write(3); break;
    case 2: lcd.setCursor(0, 1); lcd.print("RFID hacking"); break;
    case 3: lcd.setCursor(0, 1); lcd.print("Mobile spam"); break;
    case 4: lcd.setCursor(0, 1); lcd.print("2.4G hacking"); break;
    case 5: lcd.setCursor(0, 1); lcd.print("Buzzer test"); break;
  }

  if(mode == 6)
    mode = 1;
  if(mode == 0)
    mode = 5;

  if(selectState == HIGH){
    tone(piezoPin, 1000, 100);
    switch(mode) {
      case 1: {
        for(int k = 0; k < 5; k++){
          lcd.clear();
          lcd.home();
          lcd.print("Scaning");
          delay(1000);
          lcd.print(".");
          delay(1000);
          lcd.print(".");
          delay(1000);
          lcd.print(".");
          delay(1000);
        }
        end();
        break;
      }

      case 2: {
        lcd.clear();
        lcd.home();
        lcd.print("RFID");

        /* lcd.print("RFID is not");
        lcd.setCursor(0, 1);
        lcd.print("connected");
        delay(3000); */

        selectState = LOW;

        while(selectState == LOW) {
          delay(150);
          selectState = digitalRead(selectPin);
          while(getID()) {
            lcd.home();
            tone(piezoPin, 1000, 100);
            lcd.print("RFID");
            lcd.setCursor(0, 1);
            lcd.print("ID: ");
            lcd.print(tagID);
          }
        }
        tone(piezoPin, 1000, 100);
        end();
        delay(150);
        break;
      }
      case 3: {
        lcd.clear();
        lcd.home();
        lcd.print("To use this,");
        lcd.setCursor(0, 1);
        lcd.print("connect Bluetooth");
        delay(4500);
        end();
        break;
      }
      case 5: {
        lcd.clear();
        lcd.home();
        lcd.print("Buzzer test");
        tone(piezoPin, 500);
        delay(1000);
        noTone(piezoPin);
        delay(1000);
        tone(piezoPin, 500);
        delay(1000);
        noTone(piezoPin);
        delay(1000);
        tone(piezoPin, 500);
        delay(1000);
        noTone(piezoPin);
        lcd.clear();
        lcd.home();
        lcd.print("Test finished");
        delay(2000);
        end();
        break;
      }
    }
  }
}

boolean getID() {
  if (! mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }

  if (! mfrc522.PICC_ReadCardSerial()) {
    return false;
  }

  tagID = "";
  
  for (uint8_t i = 0; i < 4; i++) {
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  tagID.toUpperCase();
  mfrc522.PICC_HaltA();
  return true;
}