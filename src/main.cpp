#include <Arduino.h>
#include <util/delay.h>

#include "ComponentState.h"
#include "AnalogReader.h"

#include "TemperatureReader.h"

#include "PwmOutput.h"
#include "RpmWatchdog.h"
#include "AlarmBuzzer.h"

#include "display/DisplayButton.h"
#include "display/LedArray.h"
#include "display/DisplayController.h"

ComponentState componentState;
AnalogReader analogReader;

TemperatureReader temperatureReader = TemperatureReader(&analogReader, &componentState);
PwmOutput pwmOutput = PwmOutput(&componentState);

RpmWatchdog rpmWatchdog = RpmWatchdog(&componentState);

LedArray ledArray = LedArray(&componentState);

DisplayButton displayButton = DisplayButton(&componentState);
DisplayController displayController = DisplayController(&componentState);

AlarmBuzzer alarmBuzzer = AlarmBuzzer(&componentState);

uint16_t input = 0;

void setup()
{
  // this is only for debugging. disable for production.
  Serial.begin(115200);

  // init the debug output on PE2
  PORTE_DIRSET = (1 << PIN2_bp);

  // LED Array needs to be first, as it shows error states :)
  ledArray.init();

  // init display button
  displayButton.init();

  // create a simple tick timer with 125ms ticks using the Real Time Counter
  // enable the rtc clock
  RTC_CTRLA |= (1 << RTC_RTCEN_bp);

  // setting Period
  RTC_PITCTRLA |= RTC_PERIOD_CYC256_gc | RTC_PITEN_bm;

  // enable periodic interrupt
  RTC_PITINTCTRL |= RTC_PI_bm;

  rpmWatchdog.init();

  analogReader.initReader();
  pwmOutput.initTimer();

  displayController.init();

  _delay_ms(250);

  alarmBuzzer.init();
  sei();
}

void loop()
{
  temperatureReader.update();
  pwmOutput.update();

  displayController.update();
  ledArray.update();

  _delay_ms(100);
}

ISR(ADC0_RESRDY_vect)
{
  PORTE_OUTTGL = PIN2_bm;
  analogReader.handleResult();
}

ISR(PORTF_PORT_vect)
{
  rpmWatchdog.incrementCount();

  // CRITICAL: Clear interrupt flag!
  PORTF_INTFLAGS |= (1 << PIN5_bp);
}

ISR(RTC_PIT_vect)
{
  rpmWatchdog.tick();
  displayButton.tick();

  alarmBuzzer.tick();

  // CRITICAL: Clear interrupt flag!
  RTC_PITINTFLAGS |= (1 << RTC_PI_bp);
}