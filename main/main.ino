#include <ADS1X15.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define DECIMAL_PLACES 5
#define TIME_FACTOR 1000.0
#define FLOW_FACTOR 4.5
#define INTERRUPT_PIN 3
#define TEMPERATURE_PIN 7

/* ============= Global Variables ============= */

volatile int pulses = 1;
unsigned currentTime, startTime;
double controlVoltage, capacitorVoltage, volumetricFlowRate, temperature=1, concentration, density, massFlowRate;
String output;
ADS1115 ads(0x48);
OneWire DS18B20(TEMPERATURE_PIN);
DallasTemperature temperaturSensor(&DS18B20);

/* ============= Functions ============= */

void pulseIncrement()
{
  pulses++;
}

String makeDataPackage()
{
  String data 
     = "{\"capacitorVoltage\":" + String(capacitorVoltage, DECIMAL_PLACES)
     + ",\"temperature\":" + String(temperature, DECIMAL_PLACES) 
     + ",\"concentration\": " + String(concentration,DECIMAL_PLACES)
     + ",\"density\": " + String(density,DECIMAL_PLACES)
     + ",\"volumetricFlowRate\": " + String(volumetricFlowRate,DECIMAL_PLACES)
     + ",\"massFlowRate\":" + String(massFlowRate, DECIMAL_PLACES)
     + "}";
  return data;
}

double volumetricFlowRateCalc(){
  return ((TIME_FACTOR / (currentTime - startTime)) * pulses) / FLOW_FACTOR; // [L/M]
}

double concentrationCalc()
{
  return 13929.68 - 9718.35*capacitorVoltage - 548.74*temperature + 384.38*capacitorVoltage*temperature;
}

double densityCalc()
{
  return 1.70 + 0.0182*concentration - 0.1237*capacitorVoltage - 0.054*temperature + 0.00065*pow(temperature,2);
}

double massFlowRateCalc(){
  return density*volumetricFlowRate;
}


/* ============= Main Code ============= */

void setup()
{
  Serial.begin(9600);
  pinMode(TEMPERATURE_PIN, INPUT);
  pinMode(INTERRUPT_PIN, INPUT);
  ads.begin();
  ads.setGain(2);
  temperaturSensor.begin();
  startTime = millis();
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), pulseIncrement, FALLING);
}

void loop()
{
  currentTime = millis();
  if (currentTime - startTime >= TIME_FACTOR)
  {
    noInterrupts();
    temperaturSensor.requestTemperatures();  
    temperature = temperaturSensor.getTempCByIndex(0);
    volumetricFlowRate = volumetricFlowRateCalc(); 
    capacitorVoltage = ads.toVoltage(ads.readADC(0));
    controlVoltage = ads.toVoltage(ads.readADC(1));
    concentration = concentrationCalc();
    density = densityCalc();
     massFlowRate = massFlowRateCalc();
    output = makeDataPackage();
    Serial.println(output);
    startTime = millis();
    pulses = 0;
    interrupts();
  }
}

/* ============= End ============= */
