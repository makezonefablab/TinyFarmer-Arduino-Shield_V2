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
 */
#include <ArduinoJson.h>
#include "BTM_BitMoss.h"
#include <SoftwareSerial.h>

#define NODE_ID  12

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
  
  // 1.1  Serial & HC12 setup, 시리얼 통신 및 HC12 모듈 설정
  _comm.Init();
  
  //mySerial.begin(9600);
  
  // 2 ****** sensor value clear
  _proto.clearValues(); // all clear stored data in memory
  _proto.setNodeID(NODE_ID); // all clear stored data in memory
}

// the loop function runs over and over again forever
void loop() {
  //////////////////////////////////////////////////////////////////////////////////////////
  //
  //////////////////////////////////////////////////////////////////////////////////////////
   _proto.cmdHeartBeat();

   /*
     * {
     *  "ptCd":"07",
     *  "relayHistoryId":"1",
     *  "id":"1",
     *  "operTime":"1",
     *  "val":"1",
     *  "seq":"3",
     *  ""motorCtrlYn":"N"
     *  }
     */
   
   if (stringComplete == true)
   {   
      if(_proto.isCommandJSON(receivedData,String(NODE_ID)))
      {
          
          if(_proto.stPROTO.id == String(_proto.getNodeID())) //ID 값 비교
          {

              // 릴레이 제어 명령일때
              if( _proto.stPROTO.motorCtrlYn == "N")
              { 
                  if(_proto.stPROTO.seq == "1")
                  {
                      digitalWrite(RELAY1,(_proto.stPROTO.val=="1" ? true:false));
                      
                  }
                  
                  else if(_proto.stPROTO.seq == "2")
                  {
                      digitalWrite(RELAY2,(_proto.stPROTO.val=="1" ? true:false));
                      
                  }    
              }
              // 모터 제어 명령일때
              //else
              {
                
              }

              _proto.cmdResponse(); // 결과 반응 신호 전송
          }      
      }
      stringComplete =  false;
      
  }
  //////////////////////////////////////////////////////////////////////////////////////////
  //
  //////////////////////////////////////////////////////////////////////////////////////////
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
