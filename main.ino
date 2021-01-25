//Preto - terra, vermelho - controle, branco - capacitor

#include <ADS1X15.h>
#include <OneWire.h>
#include <DallasTemperature.h>


/* Global Variables */ 

volatile int pulses;
int currentTime, lastTime;
double control, capacitor, flowRate, temperature;
ADS1115 ads(0x48);
OneWire DS18B20(tempPin);
DallasTemperature tempSensor(&DS18B20);

/* Global Constants */

const double timeFactor=100, flowFactor=4.5;
const byte interruptPin=3,tempPin=7,stdPin=A0;

void setup() {
  pinMode(tempPin,INPUT);
  pinMode(interruptPin,INPUT);
  pinMode(stdPin,INPUT);
  Serial.begin(9600);
  ads.begin();
  tempSensor.begin();
  pulses = 0;
  lastTime = millis();
}

void loop(){
  ads.setGain(2);
  currentTime = millis();
  if(currentTime-lastTime>=timeFactor){
    detachInterrupt(interruptPin);
    tempSensor.requestTemperatures();
    temperature = tempSensor.getTempCByIndex(0);
    flowRate = ((timeFactor / (currentTime - lastTime)) * pulses)/flowFactor;
    capacitor = ads.toVoltage(ads.readADC(0)); 
    control = ads.toVoltage(ads.readADC(1));      
    Serial.print(makeJson());
    attachInterrupt(digitalPinToInterrupt(interruptPin),pulseCounter,FALLING);
    lastTime = millis();
    pulses = 0;
  }  
}

/* Functions */

String makeJson(){
  String json;
  json+="{\"capacitor\":" + String(capacitor,5);
  json+=",\"control\":" + String(control,5);
  json+=",\"flowRate\":" + String(flowRate);
  json+=",\"temperature\":" + String(temperature) + "}\n";
  return json;
}

void pulseCounter(){
  pulses++;
}