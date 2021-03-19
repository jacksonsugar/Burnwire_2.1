//Burn Wire 2.0 -- Jackson Sugar //
#include <Wire.h>
#include "RTClib.h"
#include "LowPower.h"

const int LED = 7;
const int BURN = 3;
const int STROBE = 4;
unsigned long start;
unsigned long burn;
int i;

RTC_DS3231 rtc;

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// THIS IS THE TEST SCRIPT//////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

//Demonstrates operation via one minute "mission" to test timing, strobe and burnwire functions

//Be sure to input zero for unused variables
//////////////////////////////////////////////////////////////////////////////////////
//Set time delay from power-on to wire burn
int days = 0;
int hours = 0;
int minutes = 1;
int seconds = 0;
//Set time of burn before strobe flash (in hours)
float noStrobe = 0; 
//////////////////////////////////////////////////////////////////////////////////////
const long deploy = (days * 86400)+ (hours * 3600) + (60 * minutes) + seconds;
const long burnTime = (noStrobe * 3600);

void soft_pwm(void)
{
  for(int idx=0; idx < 1000; idx++)
  {
    digitalWrite(STROBE, HIGH);
    delayMicroseconds(1);
    digitalWrite(STROBE, LOW);
    delayMicroseconds(100);
  }
}

void hard_pwm(void)
{
  for(int idx=0; idx < 100000; idx++)
  {
    digitalWrite(STROBE, HIGH);
    delayMicroseconds(1);
    digitalWrite(STROBE, LOW);
    delayMicroseconds(1);
  }
}

void setup () {

  //Set outputs
  pinMode(BURN, OUTPUT);
  pinMode(STROBE, OUTPUT);
  digitalWrite(BURN, LOW);
  digitalWrite(STROBE, LOW);

  while (!Serial) {
    delay(100);  // for Leonardo/Micro/Zero
  }

  Serial.begin(9600);

  if (! rtc.begin()) {
    while (1);
  }

  //Set new time based on computer clock
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  DateTime now = rtc.now();
  start = now.unixtime();
  burn = start + deploy;

  delay(2000);

  for (int j = 1; j <= 3; j++) {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(200);
  }


  for (int jj = 1; jj <= 3; jj++) {
    digitalWrite(LED, HIGH);
    soft_pwm();
    delay(300);
    digitalWrite(LED, LOW);
  }

}

void loop () {

  DateTime now = rtc.now();

  if (now.unixtime() <= burn) {

    digitalWrite(BURN, LOW);
    digitalWrite(STROBE, LOW);
    delay(100);

    for (int i = 1; i <= 15; i++){
      LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    }

  }
  else {
    if(now.unixtime() <= burn + burnTime) {
      digitalWrite(BURN, HIGH);
      for (int i = 1; i <= 15; i++){
        LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
      }

    }
    else {
      digitalWrite(BURN, HIGH);
      soft_pwm();
      delay(random(10, 300));
      soft_pwm();
      LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
      delay(10);
    }
  }
}

