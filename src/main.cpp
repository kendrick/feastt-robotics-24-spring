#include <Arduino.h>
#include <RTClib.h>
#include <Helpers.h>
// #include <avr8-stub.h>

const int pinLedMinutes = 8;
const int pinLedHours = 9;
const int pinButtonMinutes = 2;
const int pinButtonHours = 3;
const int pinBuzzer = 13;

RTC_DS3231 rtc;

extern const char *song;
const long alarmInterval = 5000;

bool isAlarmActive = true;
bool isAlarmRinging = false;

// previousMillis[...] store the last time each time was updated
static unsigned long previousMillisAlarm = 0;
static unsigned long previousMillisClock = 0;

void setup() {
  Serial.begin(9600);

  // Initialize outputs
  pinMode(pinLedMinutes, OUTPUT);
  pinMode(pinLedHours, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);

  // Initialize inputs
  pinMode(pinButtonMinutes, INPUT_PULLUP);
  pinMode(pinButtonHours, INPUT_PULLUP);

  initRTC(rtc);
}

void loop() {
  unsigned long currentMillis = millis();

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
      isAlarmRinging = true;
    }
  }

  int buttonMinutesState = digitalRead(pinButtonMinutes);
  int buttonHoursState = digitalRead(pinButtonHours);

  if (buttonMinutesState == LOW) {
    isAlarmActive = false;
    anyrtttl::nonblocking::stop();
    digitalWrite(pinLedMinutes, HIGH);
  } else {
    digitalWrite(pinLedMinutes, LOW);
  }

  if (buttonHoursState == LOW) {
    isAlarmActive = true;
    anyrtttl::nonblocking::stop();
    digitalWrite(pinLedHours, HIGH);
  } else {
    digitalWrite(pinLedHours, LOW);
  }

  if (isAlarmRinging) {
    if ((buttonHoursState == LOW || buttonMinutesState == LOW)) {
      previousMillisAlarm = currentMillis;

      digitalWrite(pinLedHours, LOW);
      digitalWrite(pinLedMinutes, LOW);

      isAlarmRinging = false;
    }
  }
}
