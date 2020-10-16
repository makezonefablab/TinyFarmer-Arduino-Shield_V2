//
//  btm_wifi.cpp
//  bitMossSensorLib
//
//  Created by jang sunyeon on 2017. 12. 06.
//
//
#include <stdio.h>
#include <stdlib.h>


#include "BTM_WIFI.h"


BTM_WIFI::BTM_WIFI()
{
    //ctor
}

void BTM_WIFI::Init()
{
  // 시리얼 셋팅
  Serial.begin(SERIAL_BAUD);
}

void BTM_WIFI::Init(String _ssid,String _passwd, String _host, int16_t _port)
{
  // 시리얼 셋팅
  Serial.begin(SERIAL_BAUD);
  delay(1000);

  // 와이파이 모듈에 통신 설정값 전달 
  Serial.println("{\"AT\":\"config\",\"ssid\":\""+_ssid
                                +"\",\"passwd\":\""+_passwd
                                +"\",\"host\":\""+_host
                                +"\",\"port\":\""+String(_port)
                                +"\"}");
}


bool BTM_WIFI::sendData(String _str)
{
  Serial.print(_str);

  return true;
}


bool BTM_WIFI::receiveData(String * retValue)
{
  bool ret = false;

  if(Serial.available()) {

    // get the new byte:
    char inChar = (char)Serial.read();

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

