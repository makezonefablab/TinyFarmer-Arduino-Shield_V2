/*
 *
 *  Project : Tiny Farmer
 *  SubProject : Bitmoss Sensor
 *
 *  Since : 2015.11.01
 *  Author : Jang Sun yeon (Mediaflow)
 *  URL : www.tinyfarmer.com  / www.mediaflow.kr
 *  e-mail : iot@mediaflow.kr
 *
 *
 *
 *  ################ related Libraries #################
 *  https://github.com/LowPowerLab/RFM69
 *  https://github.com/LowPowerLab/SPIFlash
 *
 *
 *
 *  - 8 kinds sensors (more than 8 sensors)
 *  - RF moddule via UART interface
 *  - Energy saving (not yet)
 *
 *
 *
 *  modification (2016.12.26)
 *  - 3 user-defined sensors added
 *
 *  modification (2017.01.11)
 *  - hc12 RF module added
 *  - rfm69 added
 *  - virtualize hc12 , rfm69 classes as BTM_COMM
 *  - unify all comm module to BTM_COMMINICATION
 *
 *  2017.12.06
 *  - ESP8266 wifi module added
 *  - control protocol added
 *
 *  2018.01.12
 *  - RFM69 removed
 * 
 *  2018.03.29
 *  - add getNodeID() in BTM_PROTOCOL class
 *  - problem , parsing error of Json in BTM_PROTOCOL class because of String class
 *
 *  2018.06.29
 *  - PROTOCOL modified, remove "" infront of number and etc
 * 
 *  2019.01.04
 *  - sw reset 
 * 
 *  2019.02.01
 *  - added getSeverPacketJSON() in BTM_PROTOCOL.cpp
 * 
 *  2019.05.21
 *  - ArduinoJson Updated
 * 
 *  2020.07.30
 *  - 제어 신호 수신 함수 추가 
 *  - util.h -> receivedCommandJSON
 * 
 */


//
//  BTM_BitMoss.h
//  bitMossSensorLib
//
//  Created by jang sunyeon on 2017. 1. 10..
//
//  * related Libraries
//  
//  https://github.com/LowPowerLab/SPIFlash
//
//
//

#ifndef BTM_BitMoss_h
#define BTM_BitMoss_h


#endif /* BitMoss_h */

#ifndef BTM_COMM_H
#define BTM_COMM_H
#include "BTM_COMM.h"
#endif

#include "BTM_PROTOCOL.h"

#ifndef BTM_HC12_H
#define BTM_HC12_H
  #include "BTM_HC12.h"
#endif

#ifndef BTM_WIFI_H
#define BTM_WIFI_H
  #include "BTM_WIFI.h"
#endif


// ********* Communication *********
#define NETWORKID     100  // The same on all nodes that talk to each other
#define NODEID        12    // The unique identifier of this node
#define GATEWAYID     1    // The recipient of packets

#define RFM69M        1   // RFM69 Module
#define HC12M         2   // Hc12 Module

#define SERIAL_BAUD   9600

// ********* Shield IO *********
#if defined(ESP32)
  #define LED2          A2
  #define LED3          A3
  #define RELAY1        25
  #define RELAY2        12
#else
  #define LED2          A2
  #define LED3          A3
  #define RELAY1        3
  #define RELAY2        8
#endif

// ********* ETC *********
#define ACK_TIME      30  // # of ms to wait for an ack
#define JSON_BUFFER_SIZE              300 /* JSON 버퍼 사이즈 */


bool checkReset(String _data)
{
    bool ret = false;

    if(_data.indexOf("reset") > 0)
    {
        asm volatile ("  jmp 0");
    }
    return ret;
}