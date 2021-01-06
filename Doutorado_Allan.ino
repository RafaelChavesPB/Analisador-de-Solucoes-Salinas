 #include <ADS1X15.h>
 #include <OneWire.h>
 #include <DallasTemperature.h>
 //Preto - terra, vermelho - controle, branco - capacitor
 double control, voltage,voltageStd, flowRate, temperature, timeFactor=100.0, factor = 7.5;
 volatile int pulses;
 int lastControl, currentTime, lastTime;
 const byte interruptPin=3,stdPin=A0,tempPin=7;

 ADS1115 ads(0x48);
 OneWire DS18B20(tempPin);
 DallasTemperature tempSensor(&DS18B20);

 bool diff(){
   return abs(control-lastControl)<0.0001;
 }

 String makeJson(){
   String json;
   json+='{';
   json+="\"tensao\":";
   json+= String(voltage,5);
   json+=",\"tensaoStd\":";
   json+= String(voltageStd,5);
   json+=",\"controle\":";
   json+= String(control,5);
   json+=",\"vazao\":";
   json+= String(flowRate);
   json+=",\"temperatura\":";
   json+= String(temperature);
   json+='}';
   json+='\n';
   return json;
 }

 void pulseCounter(){
   pulses++;
 }

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
     flowRate = ((timeFactor / (currentTime - lastTime)) * pulses)/factor;
     voltage = ads.toVoltage(ads.readADC(0)); 
     control = ads.toVoltage(ads.readADC(1));      
     voltageStd = 5*analogRead(A0)/1024.0; 
     Serial.print(makeJson());
     attachInterrupt(digitalPinToInterrupt(interruptPin),pulseCounter,FALLING);
     lastControl=control;
     lastTime = millis();
     pulses = 0;
   }  
 }
