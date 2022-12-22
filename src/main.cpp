#include <Arduino.h>
#include <AnalogReader.h>
#include "Timer.h"
#include <util/delay.h>

#include "ResistanceTemperatureConverter.h"

AnalogReader analogReader;
Timer timer;

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
  timer.initTimer();
  sei();

  _delay_ms(500);
}

void loop()
{
  referenceVoltage = analogReader.getVoltage(0);
  temperatureVoltage = analogReader.getVoltage(1);
  thirdValue = analogReader.getVoltage(2);
  temperatureResistance = ((referenceVoltage - temperatureVoltage) * 10000) / (temperatureVoltage);
  temperature = interpolateTempFromResistance(temperatureResistance);
  Serial.println(temperature);
  Serial.println("###");

  _delay_ms(500);
}

ISR(ADC0_RESRDY_vect)
{
  analogReader.handleResult();
}