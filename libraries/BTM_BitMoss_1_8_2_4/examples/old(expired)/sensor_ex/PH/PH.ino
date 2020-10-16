#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <OneWire.h>

#include <ArduinoJson.h>
#include "BTM_BitMoss.h"
#include "DHT.h"
#include "math.h"
#include "MAX44009.h"

#define SensorPin A1          //pH meter Analog output to Arduino Analog Input 0

unsigned long int avgValue;  //Store the average value o the sensor feedback
float b;
int buf[10],temp;

String sensorstring = "";                   /* 센서 데이터 저장을 위한 버퍼 */
boolean sensor_string_complete = false;     /* 센서로부터 데이터 전부 수신 했는지 여부 값 */
float pH;                                   /* PH 획득 값 변수 */

float s_t;
float s_h;
float s_ec;

BTM_PROTOCOL _proto;
BTM_COMMUNICATION _comm(WIFI);
double currentTime = 0;
double lastTime = 0;
double getLastTime(){
      return currentTime = millis()/1000.0 - lastTime;
}

DHT dht(4, DHT22);
MAX44009 light;

void setup(){
    _comm.Init();
    _proto.clearValues();
    _proto.setNodeID(15);

    dht.begin();
    Wire.begin();
    light.begin();
   
    _proto.setSensorKind(sTEMP);
    _proto.setSensorKind(sHUMI);
    _proto.setSensorKind(sILL);
    _proto.setSensorKind(sPH); 
    //_proto.setSensorKind(sSOIL_TEMP); // use Humidity sensor, 사용할 센서 종류 할당
    // _proto.setSensorKind(sEC);

    lastTime = millis()/1000.0;
}

void loop()
{
    PH_SENSOR();
    
    if((30) < (getLastTime()))
    {
        _proto.setTemp(dht.readTemperature());
        _proto.setHumidity(dht.readHumidity());
        _proto.setIllum(light.get_lux());
        _proto.setPH(pH);
        //_proto.setEC(s_ec);
        //_proto.setSoilTemp(temperature);
        _proto.sendSensorPacketJSON();
        lastTime = millis()/1000.0;
    }
    _loop();
}

void _delay(float seconds){
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime)_loop();
}

void _loop(){
}

void PH_SENSOR()
{
    for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
    { 
      buf[i]=analogRead(SensorPin);
      delay(10);
    }
    
    for(int i=0;i<9;i++)        //sort the analog from small to large
    {
      for(int j=i+1;j<10;j++)
      {
        if(buf[i]>buf[j])
        {
          temp=buf[i];
          buf[i]=buf[j];
          buf[j]=temp;
        }
      }
    }
    
    avgValue=0;
    for(int i=2;i<8;i++)                      //take the average value of 6 center sample
      avgValue+=buf[i];
    float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
    phValue=3.5*phValue;                      //convert the millivolt into pH value
    pH = phValue - 1;

    //save
    _proto.setSensorKind(sPH); // use Humidity sensor
    _proto.setPH(pH);
}
