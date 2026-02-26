#include <Wire.h>

// -------- ESP32 ADC --------
#define ADC_PIN 32

const int bridgePins[6] = {4, 16, 17, 5, 18, 23};
const float bridgeResistors[6] = {
  820.0,
  3300.0,
  12000.0,
  47000.0,
  200000.0,
  666000.0
};

const int TOTAL_BRIDGES = 6;

// -------------------
// ADC READING (ESP32)
// -------------------

float measureVoltage(int samples)
{
  long summation = 0;

  for (int i = 0; i < samples; i++)
  {
    int adc = analogRead(ADC_PIN);
    summation += adc;
    delay(5);
  }

  float adc_mean = summation / (float)samples;

  // 12-bit ADC (0–4095)
  float voltage = (adc_mean / 4095.0) * 3.3;

  return voltage;
}

// -------------------
// WHICH BRIDGE IS ACTIVE
// -------------------

int activeBridgeIndex()
{
  for (int i = 0; i < TOTAL_BRIDGES; i++)
  {
    if (digitalRead(bridgePins[i]) == HIGH)
      return i;
  }
  return -1;
}

// -------------------
// BRIDGE CONTROL
// -------------------

void adjustBridgeSimple()
{
  for (int i = 0; i < 10; i++)
  {
    float adcVoltage = measureVoltage(70);

    int index = activeBridgeIndex();
    if (index < 0) return;

    if (adcVoltage < 0.90)
    {
      if (index > 0)
      {
        digitalWrite(bridgePins[index], LOW);
        digitalWrite(bridgePins[index - 1], HIGH);
      }
    }
    else if (adcVoltage > 2.30)
    {
      if (index < TOTAL_BRIDGES - 1)
      {
        digitalWrite(bridgePins[index], LOW);
        digitalWrite(bridgePins[index + 1], HIGH);
      }
    }
    else
    {
      // voltage within range, continue
    }

    delay(200);  // allow bridge to settle
  }
}

// -------------------
// SETUP
// -------------------

void setup()
{
  Serial.begin(9600);

  analogReadResolution(12);     // 12 bit
  analogSetAttenuation(ADC_11db);  // 0–3.3V measurement range

  for (int i = 0; i < TOTAL_BRIDGES; i++)
  {
    pinMode(bridgePins[i], OUTPUT);
    digitalWrite(bridgePins[i], LOW);
  }

  digitalWrite(bridgePins[0], HIGH);
  delay(100);
}

// -------------------
// LOOP
// -------------------

void loop()
{
  unsigned long startTime = millis();

  adjustBridgeSimple();

  float finalVoltage = measureVoltage(100);

  int index = activeBridgeIndex();
  float Rfixed = bridgeResistors[index];

  float Rx = Rfixed * finalVoltage / (3 - finalVoltage);

  Serial.print("Bridge: ");
  Serial.print(index);
  Serial.print(" | Voltage: ");
  Serial.print(finalVoltage,3);
  Serial.print(" | Resistance: ");
  Serial.println(Rx,2);

  while (millis() - startTime < 3000);
}
