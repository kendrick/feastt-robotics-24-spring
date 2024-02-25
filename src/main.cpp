#include <Arduino.h>
#include <Wire.h>
#include <Helpers.h>

#include <BlockNot.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

const int pinButtonMinutes = 2;
const int pinButtonHours = 3;
const int pinBuzzer = 13;

Adafruit_7segment matrix = Adafruit_7segment();
BlockNot rtcTimer(1, SECONDS);

extern const char *song;
const long alarmInterval = 5000;

bool isAlarmActive = true;
bool isAlarmRinging = false;
bool isRTCPresent = true;

// previousMillis[...] store the last time each time was updated
static unsigned long previousMillisAlarm = 0;

void setup() {
  Serial.begin(9600);

  // Initialize the HT16K33 LED display
  matrix.begin(0x70);

  // Initialize outputs
  pinMode(pinBuzzer, OUTPUT);

  // Initialize inputs
  pinMode(pinButtonMinutes, INPUT_PULLUP);
  pinMode(pinButtonHours, INPUT_PULLUP);

  setSyncProvider(RTC.get);
}

void loop() {
  if (rtcTimer.TRIGGERED) {
    if (timeStatus() == timeSet) {
      setClockDisplay(matrix);
    }
  }

  unsigned long currentMillis = millis();

  static unsigned long previousFlashMillis = 0;
  const long flashInterval = 1000;

  if (!isRTCPresent && currentMillis - previousFlashMillis >= flashInterval) {
    previousFlashMillis = currentMillis; // Save the last flash time

    // Toggle display on and off
    static bool displayOn = false;
    if (displayOn) {
      matrix.clear();
      matrix.writeDisplay();
      displayOn = false;
    } else {
      // Assuming you want to light up all segments,
      // this might need adjusting based on your specific display
      for(int i = 0; i <= 4; ++i) {
        matrix.writeDigitRaw(i, 0xFF);
      }
      matrix.writeDisplay();
      displayOn = true;
    }
  }

  if (isAlarmRinging) {
    if (!anyrtttl::nonblocking::isPlaying()) {
      anyrtttl::nonblocking::begin(pinBuzzer, song);
    }

    anyrtttl::nonblocking::play();
  }

  // Replace the check on `alarmInterval` with a comparison
  // between the RTC and the alarm time
  if (currentMillis - previousMillisAlarm >= alarmInterval) {
    Serial.println("alarm interval");
    previousMillisAlarm = currentMillis;

    if (isAlarmActive && !isAlarmRinging) {
      Serial.println("alarm ringing");
      // isAlarmRinging = true;
    }
  }

  int buttonMinutesState = digitalRead(pinButtonMinutes);
  int buttonHoursState = digitalRead(pinButtonHours);

  if (buttonMinutesState == LOW) {
    isAlarmActive = false;
    anyrtttl::nonblocking::stop();
  } else {
    // No-op
  }

  if (buttonHoursState == LOW) {
    isAlarmActive = true;
    anyrtttl::nonblocking::stop();
  } else {
    // No-op
  }

  if (isAlarmRinging) {
    if ((buttonHoursState == LOW || buttonMinutesState == LOW)) {
      previousMillisAlarm = currentMillis;
      isAlarmRinging = false;
    }
  }
}
