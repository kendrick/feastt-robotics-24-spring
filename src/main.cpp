#include <Arduino.h>
#include <RTClib.h>
#include <ezBuzzer.h>
#include <Helpers.h>
// #include <avr8-stub.h>

extern int noteLength;
extern int melody[];
extern int noteDurations[];

const int pinLedMinutes = 8;
const int pinLedHours = 9;
const int pinButtonMinutes = 2;
const int pinButtonHours = 3;
const int pinBuzzer = 13;

RTC_DS3231 rtc;
ezBuzzer buzzer(pinBuzzer);

const long alarmInterval = 5000;

bool isAlarmActive = true;
bool isAlarmRinging = false;

// previousMillis[...] store the last time each time was updated
static unsigned long previousMillisAlarm = 0;
static unsigned long previousMillisClock = 0;

void setup() {
  // debug_init();

  Serial.begin(9600);
  noteLength = sizeof(noteDurations) / sizeof(int);

  // Initialize the LED pins as output
  pinMode(pinLedMinutes, OUTPUT);
  pinMode(pinLedHours, OUTPUT);

  // Initialize the button pins as input
  pinMode(pinButtonMinutes, INPUT_PULLUP);
  pinMode(pinButtonHours, INPUT_PULLUP);

  initRTC(rtc);
}

void loop() {
  unsigned long currentMillis = millis();

  buzzer.loop();

  if (currentMillis - previousMillisAlarm >= alarmInterval) {
    Serial.println("alarm interval");
    previousMillisAlarm = currentMillis;

    if (isAlarmActive && !isAlarmRinging) {
      Serial.println("alarm ringing");
      isAlarmRinging = true;
      buzzer.playMelody(melody, noteDurations, noteLength);
    }
  }

  int buttonMinutesState = digitalRead(pinButtonMinutes);
  int buttonHoursState = digitalRead(pinButtonHours);

  if (buttonMinutesState == LOW) {
    isAlarmActive = false;
    // Button 1 is pressed; turn on LED 1
    digitalWrite(pinLedMinutes, HIGH);
  } else {
    // Button 1 not pressed; turn off LED 1
    digitalWrite(pinLedMinutes, LOW);
  }

  if (buttonHoursState == LOW) {
    isAlarmActive = true;
    // Button 2 is pressed; turn on LED 2
    digitalWrite(pinLedHours, HIGH);
  } else {
    // Button 2 not pressed; turn off LED 2
    digitalWrite(pinLedHours, LOW);
  }

  if (isAlarmRinging) {
    if ((buttonHoursState == LOW || buttonMinutesState == LOW)) {
      previousMillisAlarm = currentMillis;

      digitalWrite(pinLedHours, LOW);
      digitalWrite(pinLedMinutes, LOW);
      buzzer.stop();

      isAlarmRinging = false;

    }
  }
}

void printTime() {
  DateTime time = rtc.now(); // Get the current date and time

  // Print the current time to the Serial monitor
  Serial.print(time.year(), DEC);
  Serial.print('/');
  Serial.print(time.month(), DEC);
  Serial.print('/');
  Serial.print(time.day(), DEC);
  Serial.print(" ");
  Serial.print(time.hour(), DEC);
  Serial.print(':');
  Serial.print(time.minute(), DEC);
  Serial.print(':');
  Serial.print(time.second(), DEC);
  Serial.println();
}

// void initRTC() {
//   if (!rtc.begin())
//   {
//     Serial.println("Couldn't find RTC.");
//     while (1)
//       ;
//   }

//   // Check if the RTC lost power and if so, set the time to the time this sketch was compiled
//   if (rtc.lostPower())
//   {
//     Serial.println("RTC lost power; setting its time to the sketch compilation time.");
//     // The following line sets the RTC to the date & time this sketch was compiled
//     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//   }
// }
