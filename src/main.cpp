#include <Arduino.h>
#include <AnalogReader.h>
#include <util/delay.h>

#include "PwmOutput.h"
#include "ResistanceTemperatureConverter.h"
#include "Display.h"
#include "LedOut.h"

AnalogReader analogReader;
PwmOutput pwmOutput;
Display upperDisplay;
Display lowerDisplay;

uint16_t input = 0;
float referenceVolts = 0.0f;
float waterTempVolts = 0.0f;
float airTempVolts = 0.0f;

float waterTemperature = 0.0f;
float airTemperature = 0.0f;

float delta_T = 0.0f;

uint16_t waterTempResistance = 0;
uint16_t airTempResistance = 0;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  init_led_output();

  upperDisplay.setPortData(
      &PORTB_OUTSET,
      &PORTB_OUTCLR,
      &PORTB_DIRSET,
      &PORTB_DIRCLR,
      &PORTB_PIN0CTRL,
      &PORTB_IN,
      1,
      0);

  lowerDisplay.setPortData(
      &PORTE_OUTSET,
      &PORTE_OUTCLR,
      &PORTE_DIRSET,
      &PORTE_DIRCLR,
      &PORTE_PIN0CTRL,
      &PORTE_IN,
      1,
      0);

  analogReader.initReader();
  pwmOutput.initTimer();
  upperDisplay.initDisplay();
  lowerDisplay.initDisplay();

  sei();

  _delay_ms(1000);
}

void loop()
{
  airTempVolts = analogReader.getVoltage(2);
  waterTempVolts = analogReader.getVoltage(1);
  referenceVolts = analogReader.getVoltage(0);

  Serial.println(referenceVolts);
  Serial.println(airTempVolts);
  Serial.println(waterTempVolts);
  Serial.println("---");

  waterTempResistance = ((referenceVolts - waterTempVolts) * 10000) / (waterTempVolts);
  airTempResistance = ((referenceVolts - airTempVolts) * 10000) / (airTempVolts);

  waterTemperature = interpolateTempFromResistance(waterTempResistance);
  airTemperature = interpolateTempFromResistance(airTempResistance);

  upperDisplay.updateNumber(waterTemperature);
  lowerDisplay.updateNumber(airTemperature);

  delta_T = waterTemperature - airTemperature;

  applyDeltaT(delta_T);
  pwmOutput.updateTemperature(waterTemperature);

  _delay_ms(200);
}

ISR(ADC0_RESRDY_vect)
{
  analogReader.handleResult();
}