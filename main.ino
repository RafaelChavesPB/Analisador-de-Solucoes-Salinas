#include <ADS1X15.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/*  ============= Global Constants ============= */

const double timeFactor = 100, flowFactor = 4.5;
const byte interruptPin = 3, temperaturePin = 7;

/* ============= Global Variables ============= */

volatile int pulses = 0;
int currentTime, startTime;
double controlVoltage, capacitorVoltage, volumetricFlowRate, temperature, concentration, density, massFlowRate;
ADS1115 ads(0x48);
OneWire DS18B20(temperaturePin);
DallasTemperature temperaturSensor(&DS18B20);

/* ============= Functions ============= */

void pulseIncrement()
{
  pulses++;
}

String makeDataPackage()
{
  String json =
      "{\"capacitorVoltage\":" + String(capacitorVoltage, 5) 
    + ",\"controlVoltage\":" + String(controlVoltage, 5) 
    + ",\"volumetricFlowRate\":" + String(volumetricFlowRate) 
    + ",\"temperature\":" + String(temperature) + "}\n";
  return json;
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
  pinMode(temperaturePin, INPUT);
  pinMode(interruptPin, INPUT);
  ads.begin();
  temperaturSensor.begin();
  startTime = millis();
}

void loop()
{
  ads.setGain(2);
  currentTime = millis();
  if (currentTime - startTime >= timeFactor)
  {
    detachInterrupt(interruptPin);
    temperaturSensor.requestTemperatures();  
    temperature = temperaturSensor.getTempCByIndex(0);
    volumetricFlowRate = ((timeFactor / (currentTime - startTime)) * pulses) / flowFactor; //volumetricFlowRate [L/M]
    capacitorVoltage = ads.toVoltage(ads.readADC(0));
    controlVoltage = ads.toVoltage(ads.readADC(1));
    concentration = concentrationCalc();
    density = densityCalc();
    massFlowRate = massFlowRateCalc();
    Serial.print(makeDataPackage());
    attachInterrupt(digitalPinToInterrupt(interruptPin), pulseIncrement, FALLING);
    startTime = millis();
    pulses = 0;
  }
}

/* ============= End ============= */
