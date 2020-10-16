/*
 * 
 *  Project : Tiny Farmer 
 *  SubProject : illumination sensor with Bitmoss Sensor
 *  
 *  Since : 2015.11.01
 *  Author : Jang Sun yeon (Mediaflow)
 *  URL : www.tinyfarmer.com  / www.mediaflow.kr
 *  e-mail : iot@mediaflow.kr
 * 
 *  - 8 kinds sensors (more than 8 sensors)
 *  - RF moddule via UART interface
 *  - Energy saving (not yet)
 *  
 *  modification (2016.12.26)
 *  - 3 user-defined sensors added 
 * 
 */
#include <ArduinoJson.h>
#include "BTM_BitMoss.h"
#include <Wire.h>
#include <math.h>

int BH1750address = 0x23;                   /*address sensor's I2C, 조도 I2C 주소 */
byte buff[2];                               /*value buffer,  조도 읽기 위한 버퍼 */

#define NODE_ID  57
#define CHANNEL  7

// ****** 1. define varialbes
BTM_PROTOCOL _proto;
BTM_COMMUNICATION _comm(HC12M);

String  inputString = "";          // a string to hold incoming data
boolean stringComplete = false;    // whether the string is complete
String  receivedData = "";

int setIO = 7; // for shield : 7 , BitmossSensor : 40

// the setup function runs once when you press reset or power the board
void setup() {

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  
  digitalWrite(LED2,HIGH);
  digitalWrite(LED3,HIGH);


  Wire.begin();


  // 1.1  Serial & HC12 setup, 시리얼 통신 및 HC12 모듈 설정
  _comm.Init(setIO, 9600, CHANNEL,NODE_ID, &Serial);

  
  // 2 ****** sensor value clear 
  _proto.clearValues(); // all clear stored data in memory

  // 2.1 ****** choose sensors
  /*
   *  sTEMP      for temperature sensor
      sHUMI      for humidity sensor
      sCO2       for co2 sensor
      sILL       for illumination sensor
      sPH        for PH sensor 
      sEC        for EC sensor 
      sSOIL_TEMP for soil temperature sensor
      sSOIL_HUMI for soil humidity sensor
      sWIND_DIR  for wind direction sensor
      sWIND_VOL  for wind volume sensor
      sRAINFALL  for rainfall sensor
      sCUSTOM1   for custom sensor sensor
      sCUSTOM2   for custom sensor sensor
      sCUSTOM3   for custom sensor sensor
      sCUSTOM4   for custom sensor sensor
   * 
   */
  _proto.setSensorKind(sILL);  // use illumination sensor, 사용할 센서 종류 할당

}

// the loop function runs over and over again forever
void loop() {


  //////////////////////////////////////////////////////////////////////////////////////////
  //
  // 센서 값을 읽어 메시 네트워크로 전송
  //  : 마스터가 해당 ID로 요청
  //  : 요청된 ID 센서노드만 데이터 전송
  //
  //  {"ch":"7","id":"57","req":"1"}
  //
  //////////////////////////////////////////////////////////////////////////////////////////



  // For getting Illumincation sensor data
  // 조도 센서 값 얻어오기
  long ill = 0;
  uint16_t val = 0;
  BH1750_Init(BH1750address);
  delay(200);

  if (2 == BH1750_Read(BH1750address))
  {
    val = ((buff[0] << 8) | buff[1]) / 1.2;
    ill = val;
  }



  
  
  if (stringComplete == true)
  {

      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(receivedData);

      String ch = "";
      String id = "";
      String req = "";


      // Received Json Format
      // {"ch":"5","id":"5","req":"1"}
      
      if (root.success())
      {
        String _ch = root["ch"];
        String _id = root["id"];
        String _req = root["req"];

        ch = _ch;
        id = _id;
        req = _req;

        digitalWrite(LED2,LOW);
        delay(300);
        digitalWrite(LED2,HIGH);

      }

      // 수신된 ID와 센서 ID 비교
      if (id == String(NODE_ID) && req == "1")
      {

        // ******  3. assign Sensor value
        _proto.setNodeID(NODE_ID);
        
        _proto.setIllum(ill);  // assign the data's illumination above, 위에서 취득한 센서 데이터 할당

  
        // ******  4. send the gathered data
        String _data;
        
        _data = _proto.getServerPacketJSON();//_data

        Serial.println(_data);

        digitalWrite(LED3,LOW);
        delay(300);
        digitalWrite(LED3,HIGH);

      }

      stringComplete =  false;
  
  }
  

  delay(1000);                       // wait for a second
}

void BH1750_Init(int address)
{
  Wire.beginTransmission(address);
  Wire.write(0x10);//1lx reolution 120ms
  Wire.endTransmission();
}

int BH1750_Read(int address)
{
  int i = 0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while (Wire.available())
  {
    buff[i] = Wire.read();  // receive one byte
    i++;
  }
  Wire.endTransmission();
  return i;
  // check if returns are valid, if they are NaN (not a number) then something went wrong!
}

// *********** Don't edit below codes *********** 
void serialEvent() {
  while (Serial.available()) {
    
    // get the new byte:
    char inChar = (char)Serial.read();
    
    // add it to the inputString:
    inputString += inChar;

    if (inChar == '}') {
      receivedData = inputString;
      inputString = "";
      stringComplete = true;
    }
  }
}

