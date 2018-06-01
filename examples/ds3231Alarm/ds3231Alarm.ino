// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;


char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  Serial.begin(9600);

  delay(3000); // wait for console opening

  if ( rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  pinMode (2, INPUT_PULLUP); // interrupt of RTC
  pinMode (13, OUTPUT); //Arduino Led indicator

  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  DateTime nowAlarm1;
  nowAlarm1 = DateTime (F(__DATE__), F(__TIME__));
  rtc.disableAlarm1();
  rtc.clearFlagAlarm1();
  /*
  DS3231_ALARM_TYPES_t :
    DS3231_ALM1_EVERY_SECOND      //every_second
    DS3231_ALM1_MATCH_SECONDS     //match seconds
    DS3231_ALM1_MATCH_MINUTES     //match minutes *and* seconds
    DS3231_ALM1_MATCH_HOURS       //match hours *and* minutes, seconds
    DS3231_ALM1_MATCH_DATE        //match date *and* hours, minutes, seconds
    DS3231_ALM1_MATCH_DAY         //match day *and* hours, minutes, seconds
  */
  rtc.adjustAlarm1(nowAlarm1, DS3231_ALM1_EVERY_SECOND);
  rtc.enableAlarm1();

  Serial.print("Alarme type: ");
  Serial.print("DS3231_ALM1_EVERY_SECOND");
  Serial.print(nowAlarm1.year(), DEC);
  Serial.print('/');
  Serial.print(nowAlarm1.month(), DEC);
  Serial.print('/');
  Serial.print(nowAlarm1.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[nowAlarm1.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(nowAlarm1.hour(), DEC);
  Serial.print(':');
  Serial.print(nowAlarm1.minute(), DEC);
  Serial.print(':');
  Serial.print(nowAlarm1.second(), DEC);
  Serial.println();
}

void loop () {
  if (!digitalRead(2)) {
    rtc.clearFlagAlarm1();
    digitalWrite(13,(!digitalRead(13)));

    DateTime now = rtc.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");
    
    // calculate a date which is 7 days and 30 seconds into the future
    DateTime future (now + TimeSpan(7,12,30,6));
    
    Serial.print(" now + 7d + 30s: ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);
    Serial.println();
    
    Serial.println();
    
    }
}
