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

#define RX                 6      /* RX 핀 번호 */
#define TX                 5      /* TX 핀 번호 */
SoftwareSerial mySerial(RX, TX);
char databuffer[35];

float soil_temp = 0;               // 1byte.1byte
float soil_hum = 0;                // 1byte.1byte
float soil_ec = 0;

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
    _proto.setSensorKind(sILL);
    _proto.setSensorKind(sSOIL_TEMP);
    _proto.setSensorKind(sSOIL_HUMI);
    //_proto.setSensorKind(sEC);

    lastTime = millis()/1000.0;
}

void loop()
{
    WT1000();
    
    if((30) < (getLastTime()))
    {
        _proto.setTemp(dht.readTemperature());
        _proto.setHumidity(dht.readHumidity());
        _proto.setIllum(light.get_lux());
        _proto.setSoilTemp(soil_temp);
        _proto.setSoilHumidity(soil_hum);
        //_proto.setEC(soil_ec);
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

void WT1000()
{ 
    for (int index = 0; index < 18; index ++) 
    {
      if (mySerial.available()) 
      {
        databuffer[index] = mySerial.read();
        
        if (databuffer[0] != 'A') 
        {
          index = -1;
        }

      }
      else
      {
        index --;
      }
    }
    
    soil_temp = t();
    soil_hum = h();
    soil_ec = ec();
    //Serial.println(soil_temp);
    //Serial.println(soil_hum);  
    //Serial.println(soil_ec);  
}

/////////////////////////////////////////////////////////////////////////////////
float transCharToIntEC(char *_buffer, int _start, int _stop)
{
  int _index;
  float result = 0;
  int num = _stop - _start + 1;
  int _temp[num];
  for (_index = _start; _index <= _stop; _index ++)
  {
    _temp[_index - _start] = _buffer[_index] - '0';
    result = 10 * result + _temp[_index - _start];
  }
  return result;
}

float h()                                                                  //Wind Direction
{
  return transCharToIntEC(databuffer, 1, 3) / 10;
}

float ec()                                                                  //Wind Direction
{
  return transCharToIntEC(databuffer, 4, 6) / 100;
}

float t()                                                                  //Wind Direction
{
  return transCharToIntEC(databuffer, 7, 9) / 10;
}
