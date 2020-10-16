//
//  BTM_COMMUNICATION.h
//  bitMossSensorLib
//
//  Created by jang sunyeon on 2017. 1. 10..
//
//

#ifndef BTM_COMMUNICATION_h
#define BTM_COMMUNICATION_h
#endif /* comm_hpp */

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "HardWareserial.h"

#ifndef BTM_COMM_H
#define BTM_COMM_H
  #include "BTM_COMM.h"
#endif

#ifndef BTM_HC12_H
#define BTM_HC12_H
  #include "BTM_HC12.h"
#endif

#ifndef BTM_WIFI_H
#define BTM_WIFI_H
  #include "BTM_WIFI.h"
#endif

#define HC12M     2   // Hc12 Module
#define WIFI      3   // ESP8266 Module

class BTM_COMMUNICATION
{
    public:
    BTM_COMMUNICATION(int _mdType);
    void Init(int setPIO, int _baudRate, int _channel,int _nodeID, HardwareSerial* port);
    void Init();
    void Init(String _ssid,String _passwd, String _host, int16_t _port);
    void sendData(String _str);
    bool receiveData(String * _rcv);

    protected:

    private:

    BTM_COMM *   comm;
    BTM_HC12     hc12;
    BTM_WIFI     wifi;

};
