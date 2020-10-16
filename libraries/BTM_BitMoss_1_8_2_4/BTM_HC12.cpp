//
//  hc12_m.cpp
//  bitMossSensorLib
//
//  Created by jang sunyeon on 2017. 1. 10..
//
//
#include <stdio.h>
#include <stdlib.h>


#include "BTM_HC12.h"


BTM_HC12::BTM_HC12()
{
    //ctor
}

void BTM_HC12::Init(int setPIO, int _baudRate, int _channel,int _nodeID, HardwareSerial* port)
{

  pSerialPort = port;
  
  // 시리얼 셋팅
  pSerialPort->begin(_baudRate);

  String chString = "AT+C00" + String(_channel) +"\r\n";
  
  
  // 초기화 (HC-12)
  pinMode(setPIO, OUTPUT);
  digitalWrite(setPIO, LOW);  // AT 설정 모드 진입
  delay(200);

  pSerialPort->print(F("AT+B9600\r\n")); // 속도  설정
  delay(200);

  pSerialPort->print(chString); // 채널설정
  delay(200);
    
  pSerialPort->print(F("AT+P8\r\n")); // 전송 파워 설정 , 1~20dB = P1 ~ P8
  delay(200);

  digitalWrite(setPIO, HIGH); // AT 설정 모드 빠져나옴
}


bool BTM_HC12::sendData(String _str)
{
  pSerialPort->print(_str);

  return true;
}


bool BTM_HC12::receiveData(String * retValue)
{
  bool ret = false;
  
  if(pSerialPort->available()) {
    
    // get the new byte:
    char inChar = (char)pSerialPort->read();
    
    // add it to the inputString:
    inputString += inChar;

    if (inChar == '}') {
      receivedData = inputString;
      
      retValue = &inputString;
      
      inputString ="";
      stringComplete = true;
      ret = true;
    }
  }

  return ret;
}

