//
//  BTM_COMMUNICATION.cpp
//  bitMossSensorLib
//
//  Created by jang sunyeon on 2017. 1. 10..
//
//
#include <stdio.h>
#include <stdlib.h>

#include "BTM_COMMUNICATION.h"

//

BTM_COMMUNICATION::BTM_COMMUNICATION(int _mdType)
{
    //ctor
    if(_mdType ==  HC12M)
    {
      comm = &hc12;
    }
    else if(_mdType ==  WIFI)
    {
      comm = &wifi;
    }
}

// for 433MHz module
void BTM_COMMUNICATION::Init(int setPIO, int _baudRate, int _channel,int _nodeID, HardwareSerial* port)
{
    comm->Init(setPIO, _baudRate,_channel,_nodeID,port);
}

void BTM_COMMUNICATION::Init()
{
    comm->Init();
}

// for WIFI
void BTM_COMMUNICATION::Init(String _ssid,String _passwd, String _host, int16_t _port)
{
    comm->Init(_ssid,_passwd, _host, _port);
}


void BTM_COMMUNICATION::sendData(String _str)
{
    comm->sendData(_str);
}


bool BTM_COMMUNICATION::receiveData(String * _rcv)
{
    bool ret = false;

    ret = comm->receiveData(_rcv);

    return ret;

}

