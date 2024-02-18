#ifndef Helpers_h
#define Helpers_h

#include <Arduino.h>
#include <RTClib.h>
#include <Music.h>

void initRTC(RTC_DS3231 rtc) {
  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC.");
    while (1)
      ;
  }

  // Check if the RTC lost power and if so, set the time to the time this sketch was compiled
  if (rtc.lostPower())
  {
    Serial.println("RTC lost power; setting its time to the sketch compilation time.");
    // The following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

#endif
