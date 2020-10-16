//
//  protocolS.cpp
//  bitMossSensorLib
//
//  Created by jang sunyeon on 2017. 1. 10..
//
//
#include <stdio.h>
#include <stdlib.h>


#include "BTM_PROTOCOL.h"



#define BUFF_LEN           60
#define JSON_BUFFER_SIZE   300   /* JSON 버퍼 사이즈 */


bool bUSE_temp = false;
bool bUSE_hum = false;
bool bUSE_ill = false;
bool bUSE_co2 = false;
bool bUSE_ph = false;
bool bUSE_ec = false;
bool bUSE_soil_temp = false;
bool bUSE_soil_hum = false;
bool bUSE_wind_dir = false;
bool bUSE_wind_vol = false;
bool bUSE_rainfall = false;
bool bUSE_custom1 = false;
bool bUSE_custom2 = false;
bool bUSE_custom3 = false;
bool bUSE_custom4 = false;

String relayStatus = "00000000";   //릴레이 상태 저장


BTM_PROTOCOL::BTM_PROTOCOL()
{
    //ctor
    clearValues();
}

void BTM_PROTOCOL::clearValues()
{
    nodeId = -1; //store this nodeId
    total = 0;
    count = 0;
    temp = 0.0;   //temperature maybe?
    hum =0.0;
    ill =0;
    co2 =0;
    ph = 0.0;
    ec = 0.0;
    soil_temp = 0.0;
    soil_hum =0.0;
    wind_dir = 0;
    wind_vol = 0.0;
    rainfall = 0.0;
    custom1 = 0.0;
    custom2 = 0.0;
    custom3 = 0.0;
    custom4 = 0.0;
    sleep = 1;
}


void BTM_PROTOCOL::parseProtocol()
{

}

void BTM_PROTOCOL::getProtocol()
{

}


void BTM_PROTOCOL::setSensorKind(int _kind)
{

    if(_kind ==  sTEMP)
    {
        bUSE_temp = true;

    }
    else if(_kind == sHUMI)
    {
        bUSE_hum = true;
    }
    else if(_kind == sCO2)
    {
        bUSE_co2 = true;
    }
    else if(_kind == sILL)
    {
        bUSE_ill = true;
    }
    else if(_kind == sPH)
    {
        bUSE_ph = true;
    }
    else if(_kind == sEC)
    {
        bUSE_ec = true;
    }
    else if(_kind == sSOIL_TEMP)
    {
        bUSE_soil_temp = true;
    }
    else if(_kind == sSOIL_HUMI)
    {
        bUSE_soil_hum = true;
    }
    else if(_kind == sWIND_DIR)
    {
        bUSE_wind_dir = true;
    }
    else if(_kind == sWIND_VOL)
    {
        bUSE_wind_vol = true;
    }
    else if(_kind == sRAINFALL)
    {
        bUSE_rainfall = true;
    }
    else if(_kind == sCUSTOM1)
    {
        bUSE_custom1 = true;
    }
    else if(_kind == sCUSTOM2)
    {
        bUSE_custom2 = true;
    }
    else if(_kind == sCUSTOM3)
    {
        bUSE_custom3 = true;
    }
    else if(_kind == sCUSTOM4)
    {
        bUSE_custom4 = true;
    }


}

bool BTM_PROTOCOL::isRequestToMe(int _nodeID, String _data)
{
//    StaticJsonBuffer<200> jsonBuffer;
//    JsonObject& root = jsonBuffer.parseObject(_data);
//
//    String ch = "";
//    String id = "";
//    String req = "";
//
//    bool ret = false;
//
//    // {"ch":"2","id":"1","req":"1"}
//
//    if (root.success())
//    {
//      String _ch = root["ch"];
//      String _id = root["id"];
//      String _req = root["req"];
//
//      ch = _ch;
//      id = _id;
//      req = _req;
//
//      // 수신된 ID와 센서 ID 비교
//      if (id == String(_nodeID) && req == "1")
//      {
//        ret = true;
//      }
//    }

    return true;
}

void BTM_PROTOCOL::setNodeID(int _nodeID)
{
    nodeId = _nodeID;
}

int  BTM_PROTOCOL::getNodeID()
{
    return nodeId;
}

void BTM_PROTOCOL::setTotalCount_Packet(int _count)
{
    total = _count;
}

void BTM_PROTOCOL::setCount_Packet(int _count)
{
    count =_count;
}

void BTM_PROTOCOL::setTemp(float _value)
{
    temp =_value;
}

void BTM_PROTOCOL::setHumidity(float _value)
{
    hum =_value;
}

void BTM_PROTOCOL::setIllum(long _value)
{
    ill = _value;
}

void BTM_PROTOCOL::setCO2(int _value)
{
    co2 =_value;
}

void BTM_PROTOCOL::setPH(float _value)
{
    ph =_value;
}

void BTM_PROTOCOL::setEC(float _value)
{
    ec =_value;
}

void BTM_PROTOCOL::setSoilTemp(float _value)
{
    soil_temp =_value;
}

void BTM_PROTOCOL::setSoilHumidity(float _value)
{
    soil_hum =_value;
}

void BTM_PROTOCOL::setWindDir(int _dgree)
{
    wind_dir =_dgree;
}

void BTM_PROTOCOL::setWindVolume(float _value)
{
    wind_vol =_value;
}

void BTM_PROTOCOL::setRainFall(float _value)
{
    rainfall =_value;
}

void BTM_PROTOCOL::setCustomSensor_1(float _value)
{
    custom1 =_value;
}

void BTM_PROTOCOL::setCustomSensor_2(float _value)
{
    custom2 =_value;
}

void BTM_PROTOCOL::setCustomSensor_3(float _value)
{
    custom3 =_value;
}

void BTM_PROTOCOL::setCustomSensor_4(float _value)
{
    custom4 =_value;
}


char * BTM_PROTOCOL::getSeverPacketBuf(int _number)
{
    static char jsonString[BUFF_LEN];

    char str_temp[6];

    if(_number == 1)
    {
        char str_temp[6];
        char str_hum[6];

        /* 4 is mininum width, 2 is precision; float value is copied onto str_temp*/
        dtostrf(temp, 4, 2, str_temp);
        dtostrf(hum, 4, 2, str_hum);

        sprintf(jsonString,"%d,%d,%d,%s,%s,%l,%l,",nodeId,total,count,str_temp,str_hum,ill,co2);
    }
    else if(_number == 2)
    {
        char str_ph[6];
        char str_ec[6];
        char str_soil_temp[6];
        char str_soil_hum[6];

        /* 4 is mininum width, 2 is precision; float value is copied onto str_temp*/
        dtostrf(ph, 4, 2, str_ph);
        dtostrf(ec, 4, 2, str_ec);
        dtostrf(soil_temp, 4, 2, str_soil_temp);
        dtostrf(soil_hum, 4, 2, str_soil_hum);
        sprintf(jsonString,"%d,%d,%d,%s,%s,%s,%s,",nodeId,total,count,str_ph,str_ec,str_soil_temp,str_soil_hum);
    }
    else if(_number == 3)
    {
        char str_wind_vol[6];
        char str_rainfall[6];
        char str_soil_custom1[6];

        /* 4 is mininum width, 2 is precision; float value is copied onto str_temp*/
        dtostrf(wind_vol, 4, 2, str_wind_vol);
        dtostrf(rainfall, 4, 2, str_rainfall);
        sprintf(jsonString,"%d,%d,%d,%d,%s,%s,%l,",nodeId,total,count,wind_dir,str_wind_vol,str_rainfall,custom1);
    }
    else if(_number == 4)
    {
        sprintf(jsonString,"%d,%d,%d,%l,%l,%l,%d,",nodeId,total,count,custom2,custom3,custom4,sleep);
    }

    return jsonString;

}

String BTM_PROTOCOL::getSeverPacketString(int _number)
{
    String jsonString;

    if(_number == 1)
    {
        jsonString += String(nodeId);
        jsonString += String(","+String(total));
        jsonString += String(","+String(_number));
        jsonString += String(","+String(temp));
        jsonString += String(","+String(hum));
        jsonString += String(","+String(ill));
        jsonString += String(","+String(co2)+",");
    }
    else if(_number == 2)
    {
        jsonString += String(nodeId);
        jsonString += String(","+String(total));
        jsonString += String(","+String(_number));
        jsonString += String(","+String(ph));
        jsonString += String(","+String(ec));
        jsonString += String(","+String(soil_temp));//+String(ill));
        jsonString += String(","+String(soil_hum)+",");
    }
    else if(_number == 3)
    {
        jsonString += String(nodeId);
        jsonString += String(","+String(total));
        jsonString += String(","+String(_number));
        jsonString += String(","+String(wind_dir));
        jsonString += String(","+String(wind_vol));
        jsonString += String(","+String(rainfall));//+String(ill));
        jsonString += String(","+String(custom1)+",");
    }
    else if(_number == 4)
    {
        jsonString += String(nodeId);
        jsonString += String(","+String(total));
        jsonString += String(","+String(_number));
        jsonString += String(","+String(custom2));
        jsonString += String(","+String(custom3));
        jsonString += String(","+String(custom4));
        jsonString += String(","+String(sleep)+",");
    }


    return jsonString;

}

String BTM_PROTOCOL::getSeverPacketJSON()
{
    static char jsonBuf[JSON_BUFFER_SIZE];

    String jsonString = "{\"ptCd\":\"06\",\"id\":"+String(nodeId)+"";

    if(bUSE_temp == true)
    {
        jsonString += ",\"temp\":"+String(temp)+"";
    }

    if(bUSE_hum == true)
    {
        jsonString += ",\"hum\":"+String(hum)+"";
    }

    if(bUSE_co2 == true)
    {
        jsonString += ",\"co2\":"+String(co2)+"";
    }

    if(bUSE_ill == true)
    {

        jsonString += ",\"ill\":"+String(ill)+"";
    }

    if(bUSE_ph == true)
    {

        jsonString += ",\"ph\":"+String(ph)+"";
    }

    if(bUSE_ec == true)
    {

        jsonString += ",\"ec\":"+String(ec)+"";
    }

    if(bUSE_soil_temp == true)
    {

        jsonString += ",\"soilTemp\":"+String(soil_temp)+"";
    }

    if(bUSE_soil_hum == true)
    {

        jsonString += ",\"soilHum\":"+String(soil_hum)+"";
    }

    if(bUSE_wind_dir == true)
    {

        jsonString += ",\"windDir\":"+String(wind_dir)+"";
    }

    if(bUSE_wind_vol == true)
    {

        jsonString += ",\"windVol\":"+String(wind_vol)+"";
    }

    if(bUSE_rainfall == true)
    {

        jsonString += ",\"rainfall\":"+String(rainfall)+"";
    }

    if(bUSE_custom1 == true)
    {
        jsonString += ",\"custom1\":"+String(custom1)+"";
    }

    if(bUSE_custom2 == true)
    {
        jsonString += ",\"custom2\":"+String(custom2)+"";
    }

    if(bUSE_custom3 == true)
    {
        jsonString += ",\"custom3\":"+String(custom3)+"";
    }

    if(bUSE_custom4 == true)
    {
        jsonString += ",\"custom4\":"+String(custom4)+"";
    }

    jsonString += "}";

    return jsonString;
}


String BTM_PROTOCOL::sendSensorPacketJSON()
{
    static char jsonBuf[JSON_BUFFER_SIZE];

    String jsonString = "{\"ptCd\":\"06\",\"id\":"+String(nodeId)+"";

    if(bUSE_temp == true)
    {
        jsonString += ",\"temp\":"+String(temp)+"";
    }

    if(bUSE_hum == true)
    {
        jsonString += ",\"hum\":"+String(hum)+"";
    }

    if(bUSE_co2 == true)
    {
        jsonString += ",\"co2\":"+String(co2)+"";
    }

    if(bUSE_ill == true)
    {

        jsonString += ",\"ill\":"+String(ill)+"";
    }

    if(bUSE_ph == true)
    {

        jsonString += ",\"ph\":"+String(ph)+"";
    }

    if(bUSE_ec == true)
    {

        jsonString += ",\"ec\":"+String(ec)+"";
    }

    if(bUSE_soil_temp == true)
    {

        jsonString += ",\"soilTemp\":"+String(soil_temp)+"";
    }

    if(bUSE_soil_hum == true)
    {

        jsonString += ",\"soilHum\":"+String(soil_hum)+"";
    }

    if(bUSE_wind_dir == true)
    {

        jsonString += ",\"windDir\":"+String(wind_dir)+"";
    }

    if(bUSE_wind_vol == true)
    {

        jsonString += ",\"windVol\":"+String(wind_vol)+"";
    }

    if(bUSE_rainfall == true)
    {

        jsonString += ",\"rainfall\":"+String(rainfall)+"";
    }

    if(bUSE_custom1 == true)
    {
        jsonString += ",\"custom1\":"+String(custom1)+"";
    }

    if(bUSE_custom2 == true)
    {
        jsonString += ",\"custom2\":"+String(custom2)+"";
    }

    if(bUSE_custom3 == true)
    {
        jsonString += ",\"custom3\":"+String(custom3)+"";
    }

    if(bUSE_custom4 == true)
    {
        jsonString += ",\"custom4\":"+String(custom4)+"";
    }

    jsonString += "}";

    Serial.println(jsonString);

    return jsonString;
}


bool BTM_PROTOCOL::isCommandJSON(String _comm, String _nodeID)
{
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

    //String jsonString = _comm;
    bool ret = false;

    StaticJsonDocument<200> jsonBuffer;

    DeserializationError error = deserializeJson(jsonBuffer, _comm);
    Serial.println(_comm);

    if (error == false)
    {
        String _ptCd = jsonBuffer["ptCd"];
        String _relayHistoryId = jsonBuffer["relayHistoryId"];
        String _id = jsonBuffer["id"];
        String _motorCtrlYn = jsonBuffer["motorCtrlYn"];
        //String _operTime = root["operTime"];
        String _seq = jsonBuffer["seq"];
        String _val = jsonBuffer["val"];

        stPROTO.ptCd = _ptCd;
        stPROTO.relayHistoryId = _relayHistoryId;
        stPROTO.id = _id;
        stPROTO.motorCtrlYn = _motorCtrlYn;
        //stPROTO.operTime = _operTime;
        stPROTO.seq = _seq;
        stPROTO.val = _val;
        
        if(_id == _nodeID)
        {
            relayStatus[8-_seq.toInt()] = (_val=="1" ? '1':'0'); // 릴레이 상태 저장
        }
        
        ret = true;
    }


    return ret;

}

void BTM_PROTOCOL::setResult(String _seq, String _val)
{
    relayStatus[8-_seq.toInt()] = (_val=="1" ? '1':'0'); // 릴레이 상태 저장
}

String BTM_PROTOCOL::checkERROR(String _comm)
{
    String jsonString = _comm;
    String ret = "0";

    StaticJsonDocument<200> jsonBuffer;
    DeserializationError error = deserializeJson(jsonBuffer, _comm);

    // {"Err":"1"}

    if (error == false)
    {
        String _Err = jsonBuffer["Err"];
		stPROTO.Err = _Err;
    }

    return ret;
}

void BTM_PROTOCOL::cmdHeartBeat()
{
//  heartbeatTimer
    //if ((unsigned long)(millis() - heartbeatTimer) > HEARTBEAT_TIMER)
    //{

    //    heartbeatTimer = millis();

        //to do
        
	cmdStatusSend(nodeId,0);

    //}
}

void BTM_PROTOCOL::cmdResponse()
{
	bmTCPSend(nodeId,stPROTO);
}


String BTM_PROTOCOL::getCommandID()
{

}


String BTM_PROTOCOL::getCommandSEQ()
{

}


String BTM_PROTOCOL::getCommandVAL()
{

}


// 상태 (허트비트)
/////////////////////////////////////////////////////////////////////////////////
void BTM_PROTOCOL::cmdStatusSend(int _nodeID, int _seq) {

    String msg = "";
    StaticJsonDocument<200> jsonBuffer;

    jsonBuffer["ptCd"] = "02";
    jsonBuffer["dtCd"] = "03";
    jsonBuffer["id"] = String(_nodeID);
    jsonBuffer["status"] = relayStatus;//현재 모든 릴레이의 상태를 보냄

#if defined(__AVR_ATmega128__) ||defined(__AVR_ATmega1280__)
    //Serial1.println(_buff);
    serializeJson(jsonBuffer, Serial);
    Serial.println();
#else
    serializeJson(jsonBuffer, Serial);
    Serial.println();
#endif

}


/////////////////////////////////////////////////////////////////////////////////
// 수동 응답 프로토콜
/////////////////////////////////////////////////////////////////////////////////
void BTM_PROTOCOL::bmTCPSend(int _nodeID, _bmProtocolST _st) {
  
  String msg = "";
  StaticJsonDocument<200> jsonBuffer;

  jsonBuffer["ptCd"]  = "08";
  jsonBuffer["relayHistoryId"] = _st.relayHistoryId;
  jsonBuffer["id"]    = String(_nodeID);
  jsonBuffer["val"]   = relayStatus;//"00000000";
  jsonBuffer["resCd"] = "00";   // 00:정상, 99:패킷수신실패


#if defined(__AVR_ATmega128__) ||defined(__AVR_ATmega1280__)
    serializeJson(jsonBuffer, Serial);
    Serial.println();
#else
    serializeJson(jsonBuffer, Serial);
    Serial.println();
#endif

}