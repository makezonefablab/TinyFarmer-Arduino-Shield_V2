//
//  comm.h
//  bitMossSensorLib
//
//  Created by jang sunyeon on 2017. 1. 10..
//
//

#ifndef BTM_COMM_h
#define BTM_COMM_h
#endif /* comm_hpp */

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "HardWareserial.h"
#define SERIAL_BAUD   9600

class BTM_COMM
{
    public:
    //comm(int _mdType);
    virtual void Init(int setPIO, int _baudRate, int _channel,int _nodeID, HardwareSerial* port);
    virtual void Init();
    virtual void Init(String _ssid,String _passwd, String _host, int16_t _port);
    virtual bool sendData(String _str);
    virtual bool receiveData(String * retValue);

    protected:

    private:

};
