#include <Arduino.h>
#include <AnalogReader.h>
#include <util/delay.h>

#include "PwmOutput.h"
#include "ResistanceTemperatureConverter.h"
#include "Display.h"

AnalogReader analogReader;
PwmOutput pwmOutput;
Display display;

uint16_t input = 0;
float referenceVoltage = 0.0f;
float temperatureVoltage = 0.0f;
float thirdValue = 0.0f;
float temperature = 0.0f;

uint16_t temperatureResistance = 0;



void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  analogReader.initReader();
  pwmOutput.initTimer();
  display.initDisplay();
  sei();

  _delay_ms(500);
}

void loop()
{
  temperatureVoltage = analogReader.getVoltage(1);
  referenceVoltage = analogReader.getVoltage(0);
  temperatureResistance = ((referenceVoltage - temperatureVoltage) * 10000) / (temperatureVoltage);
  temperature = interpolateTempFromResistance(temperatureResistance);

  pwmOutput.updateTemperature(temperature);

  // Serial.println(temperature);
  // Serial.println("--");

  display.updateNumber(temperature);

  _delay_ms(250);
}

ISR(ADC0_RESRDY_vect)
{
  analogReader.handleResult();
}