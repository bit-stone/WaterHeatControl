#include <Arduino.h>
#include <AnalogReader.h>

AnalogReader analogReader;

uint16_t input = 0;
float referenceVoltage = 0.0f;
float temperatureVoltage = 0.0f;
float thirdValue = 0.0f;
float factor = 0.0f;

uint16_t temperatureResistance = 0;



void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  analogReader.initReader();

  delay(1000);
}

void loop()
{
  referenceVoltage = analogReader.getVoltage(0);
  temperatureVoltage = analogReader.getVoltage(1);
  thirdValue = analogReader.getVoltage(2);
  temperatureResistance = ((referenceVoltage - temperatureVoltage) * 10000) / (temperatureVoltage);

  Serial.println(temperatureResistance);

  Serial.println("###");

  delay(500);
}

ISR(ADC0_RESRDY_vect)
{
  analogReader.handleResult();
}