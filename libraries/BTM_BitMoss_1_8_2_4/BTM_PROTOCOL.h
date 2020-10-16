//
//  protocolS.hpp
//  bitMossSensorLib
//
//  Created by jang sunyeon on 2017. 1. 10..
//
//

#ifndef protocolS_hpp
#define protocolS_hpp


#endif /* protocolS_hpp */


#include "HardWareserial.h"

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <ArduinoJson.h>
#include "BTM_COMMUNICATION.h"

#define sTEMP      1
#define sHUMI      2
#define sCO2       3
#define sILL       4
#define sPH        5
#define sEC        6
#define sSOIL_TEMP 7
#define sSOIL_HUMI 8
#define sWIND_DIR  9
#define sWIND_VOL  10
#define sRAINFALL  11
#define sCUSTOM1   12
#define sCUSTOM2   13
#define sCUSTOM3   14
#define sCUSTOM4   15


///////////// ERROR /////////////
#define NORMAL       "0"
#define DISCONNECTED "1"




class BTM_PROTOCOL
{
    public:

    //manual control protocol
    typedef struct protocolSt {
      String ptCd;
      String relayHistoryId;
      String id;
      String motorCtrlYn;
      String kind;
      String operTime;
      String seq;
      String val;
      String resCd;
	  String Err;

    } _bmProtocolST;

    _bmProtocolST stPROTO;

    //heartbeat setting
    #define HEARTBEAT_TIMER 30000
    uint32_t heartbeatTimer;



    BTM_PROTOCOL();
    void parseProtocol();
    void getProtocol();
    bool isRequestToMe(int _nodeID, String _data);
    void setNodeID(int _nodeID);
    int  getNodeID();
    void setTotalCount_Packet(int _count);
    void setCount_Packet(int _count);
    void setTemp(float _value);
    void setHumidity(float _value);
    void setIllum(long _value);
    void setCO2(int _value);
    void setPH(float _value);
    void setEC(float _value);
    void setSoilTemp(float _value);
    void setSoilHumidity(float _value);
    void setWindDir(int _dgree);
    void setWindVolume(float _value);
    void setRainFall(float _value);
    void setCustomSensor_1(float _value);
    void setCustomSensor_2(float _value);
    void setCustomSensor_3(float _value);
    void setCustomSensor_4(float _value);
    char * getSeverPacketBuf(int _number);
    String getSeverPacketString(int _number);
    String getSeverPacketJSON();
    String sendSensorPacketJSON();
    void setSensorKind(int _kind);
    bool isCommandJSON(String _comm, String _nodeID);
    void setResult(String _seq, String _val);
    String checkERROR(String _comm);
    String getCommandID();
    String getCommandSEQ();
    String getCommandVAL();
    void cmdHeartBeat();
    void cmdResponse();

    void clearValues();
    void cmdStatusSend(int _nodeID,int _seq); // heart beat for command node
    void bmTCPSend(int _nodeID, _bmProtocolST _st);

    protected:

    private:

    int           nodeId; //store this nodeId
    int           total;
    int           count;
    float         temp;   //temperature maybe?
    float         hum;
    long          ill;
    long          co2;
    float         ph;
    float         ec;
    float         soil_temp;
    float         soil_hum;
    long          wind_dir;
    long          wind_vol;
    float         rainfall;
    float         custom1;
    float         custom2;
    float         custom3;
    float         custom4;
    int           sleep;
    //float         custom5;

    
    
};
