//
//  hc12_m.h
//  bitMossSensorLib
//
//  Created by jang sunyeon on 2017. 1. 10..
//
//

#ifndef hc12_m_hpp
#define hc12_m_hpp
#endif /* hc12_m_hpp */

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#ifndef BTM_COMM_H
#define BTM_COMM_H
  #include "BTM_COMM.h"
#endif
#include "HardWareserial.h"



#define HC12_SET  7

class BTM_HC12 : public BTM_COMM
{
  
    public:
    BTM_HC12();
    void Init(int setPIO, int _baudRate, int _channel,int _nodeID, HardwareSerial* port);
    bool sendData(String _str);
    bool receiveData(String * retValue);
    
    protected:
    
    private:
    HardwareSerial* pSerialPort;
    String inputString = "";         // a string to hold incoming data
    boolean stringComplete = false;  // whether the string is complete
    String receivedData = "";

};
