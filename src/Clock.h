#include <Wire.h>;
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

static bool dotsLit = false;

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

int getFixedDigit(int number) {
  const int _0 = 1 + 2 + 4 + 8 + 16 + 32;
  const int _1 = 32 + 16;
  const int _2 = 8 + 16 + 64 + 2 + 1;
  const int _3 = 8 + 16 + 64 + 32 + 1;
  const int _4 = 16 + 64 + 4 + 32;
  const int _5 = 8 + 4 + 64 + 32 + 1;
  const int _6 = 1 + 2 + 32 + 64 + 4 + 8;
  const int _7 = 32 + 16 + 8;
  const int _8 = 1 + 2 + 4 + 8 + 16 + 32 + 64;
  const int _9 = 1 + 32 + 16 + 8 + 4 + 64;

  switch (number) {
    case 0:
      return _0;
      break;

    case 1:
      return _1;
      break;

    case 2:
      return _2;
      break;

    case 3:
      return _3;
      break;

    case 4:
      return _4;
      break;

    case 5:
      return _5;
      break;

    case 6:
      return _6;
      break;

    case 7:
      return _7;
      break;

    case 8:
      return _8;
      break;

    case 9:
      return _9;
      break;

    default:
      return 64;
      break;
  }

  return 64;
}

void setClockDisplay(Adafruit_7segment matrix){
  const int digit4 = getFixedDigit(hour() / 10);
  const int digit3 = getFixedDigit(hour() % 10);
  const int digit1 = getFixedDigit(minute() / 10);
  const int digit0 = getFixedDigit(minute() % 10);

  dotsLit = !dotsLit;

  matrix.clear();
  matrix.writeDigitRaw(4, digit4);
  matrix.writeDigitRaw(3, digit3);
  matrix.drawColon(dotsLit);
  matrix.writeDigitRaw(1, digit1);
  matrix.writeDigitRaw(0, digit0);
  matrix.writeDisplay();

  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println();
}
