//
//  btm_wifi.h
//  bitMossSensorLib
//
//  Created by jang sunyeon on 2017. 12. 06.
//
//

#ifndef wifi_m_hpp
#define wifi_m_hpp
#endif /* wifi_m_hpp */

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

class BTM_WIFI : public BTM_COMM
{

    public:
    BTM_WIFI();
    void Init();
    void Init(String _ssid,String _passwd, String _host, int16_t _port);
    bool sendData(String _str);
    bool receiveData(String * retValue);

    protected:

    private:
    HardwareSerial* pSerialPort;
    String inputString = "";         // a string to hold incoming data
    boolean stringComplete = false;  // whether the string is complete
    String receivedData = "";

};
