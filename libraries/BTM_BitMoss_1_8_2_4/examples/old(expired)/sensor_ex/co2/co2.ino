#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <OneWire.h>

#include <ArduinoJson.h>
#include "BTM_BitMoss.h"
#include "DHT.h"
#include "math.h"
#include "MAX44009.h"

String sensorstring = "";                   /* 센서 데이터 저장을 위한 버퍼 */
boolean sensor_string_complete = false;     /* 센서로부터 데이터 전부 수신 했는지 여부 값 */
long CO2;                       

#define RX                 6     /* RX 핀 번호 */
#define TX                 5     /* TX 핀 번호 */
SoftwareSerial mySerial(RX, TX);
unsigned char hexdata[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};

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

    mySerial.begin(9600);
    dht.begin();
    Wire.begin();
    light.begin();
   
    _proto.setSensorKind(sTEMP);
    _proto.setSensorKind(sHUMI);
    //_proto.setSensorKind(sILL);
    _proto.setSensorKind(sCO2); 
    //_proto.setSensorKind(sSOIL_TEMP); // use Humidity sensor, 사용할 센서 종류 할당
    // _proto.setSensorKind(sEC);

    lastTime = millis()/1000.0;
}

void loop()
{
    Co2_Sensor();
    
    if((30) < (getLastTime()))
    {
        _proto.setTemp(dht.readTemperature());
        _proto.setHumidity(dht.readHumidity());
        //_proto.setIllum(light.get_lux());
        _proto.setCO2(CO2);
        _proto.sendSensorPacketJSON();
        lastTime = millis()/1000.0;
    }
}

void _delay(float seconds){
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime)_loop();
}

void _loop(){
}

void Co2_Sensor()
{  
      long hi, lo, co2;  
      mySerial.write(hexdata,9);
      //Serial1.write(hexdata,9);
      delay(100);
  
      for(int i=0,j=0;i<9;i++)
      {
        if (mySerial.available())
        {    
          int data = mySerial.read();
          //int data = Serial1.read();   
          if(i==2){hi = data;}   //High concentration
          if(i==3){lo = data;}   //Low concentration
          if(i==8)
          {
              co2 = hi * 256 +lo;//CO2 concentration
              CO2 = co2;    
          }  
        }   
      } 
}
