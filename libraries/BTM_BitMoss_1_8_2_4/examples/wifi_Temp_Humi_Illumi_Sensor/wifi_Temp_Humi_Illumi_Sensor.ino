/*
 * 
 *  Project : Tiny Farmer 
 *  SubProject : Bitmoss Sensor
 *  
 *  Since : 2020.05.01
 *  Author : Jang Sun yeon (Mediaflow)
 *  URL : www.tinyfarmer.com  / www.mediaflow.kr
 *  e-mail : iot@mediaflow.kr
 * 
 *  - 8 kinds sensors (more than 8 sensors)
 *  - RF moddule via UART interface
 *  - Energy saving (not yet)
 *  
 * 
 */
#include <ArduinoJson.h>
#include <BTM_BitMoss.h>
#include <DHT.h>
#include <SoftwareSerial.h>

#define DHTPIN 4        // DHT pin number, 온습도 센서 핀 번호
#define DHTTYPE DHT11   // DHT Sensor Type, 온습도 센서 타입

SoftwareSerial mySerial(10, 12);

DHT dht(DHTPIN, DHTTYPE); 

#define NODE_ID  11

// ****** 1. define varialbes
BTM_PROTOCOL _proto;
BTM_COMMUNICATION _comm(WIFI);

String  inputString = "";          // a string to hold incoming data
boolean stringComplete = false;    // whether the string is complete
String  receivedData = "";
long lastMsg = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  
  digitalWrite(LED2,HIGH);
  digitalWrite(LED3,HIGH);
  
  //dht.begin();

  // 1.1  Serial & HC12 setup, 시리얼 통신 및 HC12 모듈 설정
  _comm.Init();
  
  //mySerial.begin(9600);
  
  // 2 ****** sensor value clear
  _proto.clearValues(); // all clear stored data in memory
  _proto.setNodeID(NODE_ID); // all clear stored data in memory

  if (stringComplete == true)
  {   
      Serial.print(receivedData);  
      stringComplete =  false;
      delay(5000);
  }

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
  _proto.setSensorKind(sTEMP); // use Temperature sensor
  _proto.setSensorKind(sHUMI); // use Humidity sensor
  _proto.setSensorKind(sILL);  // use illumination sensor

  
}

// the loop function runs over and over again forever
void loop() {
  //////////////////////////////////////////////////////////////////////////////////////////
  //
  //////////////////////////////////////////////////////////////////////////////////////////
  float hum = dht.readHumidity();    //read humidity,  습도 센서값 읽기 
  float temp = dht.readTemperature();//read temperature,  온도 센서값 읽기 

   if (stringComplete == true)
   {   
      Serial.print(receivedData);
      if(_proto.isCommandJSON(receivedData,String(NODE_ID)))
      {
        _proto.stPROTO.id;
        
        if(_proto.stPROTO.seq == "1")
            digitalWrite(RELAY1,(_proto.stPROTO.val=="1" ? true:false));
        else if(_proto.stPROTO.seq == "2")
            digitalWrite(RELAY2,(_proto.stPROTO.val=="1" ? true:false));
      }

      stringComplete =  false;

      _proto.cmdResponse(); // 결과 반응 신호 전송
  }

  _proto.cmdHeartBeat(); // heart beat
  
  //////////////////////////////////////////////////////////////////////////////////////////
  //
  //////////////////////////////////////////////////////////////////////////////////////////

    // ******  3. assign Sensor value
    _proto.setNodeID(NODE_ID);
    
    _proto.setTotalCount_Packet(4);
    
    _proto.setTemp(temp);
    
    _proto.setHumidity(hum);
    
    _proto.setIllum(10);

    /*
    // Not Used belows
    
    _proto.setCO2(500);
    _proto.setPH(7.6);
    _proto.setEC(8.1);
    _proto.setSoilTemp(0.0);
    _proto.setSoilHumidity(0.0);
    _proto.setWindDir(230);
    _proto.setWindVolume(4.5);
    _proto.setRainFall(55.5);
    _proto.setCustomSensor_1(555.5);
    _proto.setCustomSensor_2(666.6);
    _proto.setCustomSensor_3(777.7);
    _proto.setCustomSensor_4(888.8);

    */
    // ******  4. send the gathered data
    String _data;
    long now = millis();
    
    if (now - lastMsg > 30000) 
    {
        lastMsg = now;          

        _data = _proto.sendSensorPacketJSON();//_data

        digitalWrite(LED3,LOW);
        delay(300);
        digitalWrite(LED3,HIGH);
    }
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
