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
 *  - mblock Interface File
 *
 */

String  inputString = "";          // a string to hold incoming data
boolean stringComplete = false;    // whether the string is complete
String  receivedData = "";

int ret = 0;

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

String _relayStatus = "00000000";   //릴레이 상태 저장




double currentTime1 = 0;
double lastTime1 = 0;

double currentTime2 = 0;
double lastTime2 = 0;

double getLastTime1() {
	return currentTime1 = millis() / 1000.0 - lastTime1;
}

double getLastTime2() {
	return currentTime2 = millis() / 1000.0 - lastTime2;
}





// *********** Don't edit below codes *********** 
void serialEvent() {
	while (Serial.available()) {

		// get the new byte:
		char inChar = (char)Serial.read();

		// add it to the inputString:
		inputString += inChar;

		if (inChar == '}') {
			receivedData = inputString;
			inputString = "";
			stringComplete = true;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////
//  상태전송
/////////////////////////////////////////////////////////////////////////////////
void cmdStatusSend(int _nodeID, int _seq) {

	String msg = "";

	StaticJsonDocument<200> jsonBuffer;

	jsonBuffer["ptCd"] = "02";
	jsonBuffer["dtCd"] = "03";
	jsonBuffer["id"] = String(_nodeID);
	jsonBuffer["status"] = _relayStatus;//현재 모든 릴레이의 상태를 보냄

#if defined(__AVR_ATmega128__) ||defined(__AVR_ATmega1280__)
	serializeJson(jsonBuffer, Serial);
	Serial.println();
#else
	serializeJson(jsonBuffer, Serial);
	Serial.println();
#endif

}

/////////////////////////////////////////////////////////////////////////////////
//  정해진 시간에 상태값 전송 (30초)
/////////////////////////////////////////////////////////////////////////////////
void sendHeartBeat(int _nodeID) {

	if ((30) < (getLastTime1()))
	{
		cmdStatusSend(_nodeID, 0);
		lastTime1 = millis() / 1000.0;
	}

}


/////////////////////////////////////////////////////////////////////////////////
// 수동 응답 프로토콜
/////////////////////////////////////////////////////////////////////////////////
void bmTCPSend(int _nodeID, _bmProtocolST _st) {

	String msg = "";

	StaticJsonDocument<200> jsonBuffer;

	jsonBuffer["ptCd"] = "08";
	jsonBuffer["relayHistoryId"] = _st.relayHistoryId;
	jsonBuffer["id"] = String(_nodeID);
	jsonBuffer["val"] = _relayStatus;//"00000000";
	jsonBuffer["resCd"] = "00";   // 00:정상, 99:패킷수신실패

#if defined(__AVR_ATmega128__) ||defined(__AVR_ATmega1280__)
	serializeJson(jsonBuffer, Serial);
	Serial.println();
#else
	serializeJson(jsonBuffer, Serial);
	Serial.println();
#endif

}

/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
bool isCommandJSON(String _comm, String _nodeID)
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

	 //{"ptCd":"07","relayHistoryId":"1","id":"1","val":"1","seq":"3","motorCtrlYn":"N"}

	String jsonString = _comm;
	bool ret = false;

	StaticJsonDocument<200> jsonBuffer;
	DeserializationError error = deserializeJson(jsonBuffer, _comm);

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

		if (_id == _nodeID)
		{
			_relayStatus[8 - _seq.toInt()] = (_val == "1" ? '1' : '0'); // 릴레이 상태 저장
		}

		ret = true;
	}


	return ret;

}

/////////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////////
void checkCommand(String _cmd, int _id)
{
	// {"ptCd":"07","relayHistoryId":"3888","id":"15","operTime":"0","seq":"1","val":"1","motorCtrlYn":"N"}
	String _id_str = "\"id\":\"" + String(_id) + "\"";

	if (_cmd.indexOf(_id_str) > 1)
	{
		if (_cmd.indexOf("\"seq\":\"1\"") > 1)
		{
			if (_cmd.indexOf("\"val\":\"1\"") > 1)
				digitalWrite(RELAY1, HIGH);
			if (_cmd.indexOf("\"val\":\"0\"") > 1)
				digitalWrite(RELAY1, LOW);
		}
		else if (_cmd.indexOf("\"seq\":\"2\"") > 1)
		{
			if (_cmd.indexOf("\"val\":\"1\"") > 1)
				digitalWrite(RELAY2, HIGH);
			if (_cmd.indexOf("\"val\":\"0\"") > 1)
				digitalWrite(RELAY2, LOW);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////
//  
/////////////////////////////////////////////////////////////////////////////////
void checkCommandJSON(String _cmd, int _id)
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

	if (stringComplete == true)
	{
		if (isCommandJSON(_cmd, String(_id)))
		{
			if (stPROTO.id == String(_id)) //ID 값 비교
			{
				// 릴레이 제어 명령일때
				if (stPROTO.motorCtrlYn == "N")
				{
					if (stPROTO.seq == "1")
					{
						digitalWrite(RELAY1, (stPROTO.val == "1" ? true : false));

					}

					else if (stPROTO.seq == "2")
					{
						digitalWrite(RELAY2, (stPROTO.val == "1" ? true : false));

					}
				}
				// 모터 제어 명령일때
				else
				{

				}
			}
		}
		stringComplete = false;
		bmTCPSend(_id, stPROTO); // 결과 반응 신호 전송
	}
}

int receivedCommandJSON(String _cmd, int _id)
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


	if (stringComplete == true)
	{
		if (isCommandJSON(_cmd, String(_id)))
		{
			if (stPROTO.id == String(_id)) //ID 값 비교
			{
				// 릴레이 제어 명령일때
				if (stPROTO.motorCtrlYn == "N")
				{
					if (stPROTO.seq == "1")
					{
						ret = (stPROTO.val == "1" ? 11 : 10);
					}

					else if (stPROTO.seq == "2")
					{
						ret = (stPROTO.val == "1" ? 21 : 20);
					}
					else
					{
						ret = 0;
					}

				}
				// 모터 제어 명령일때
				else
				{

				}
			}
		}
		stringComplete = false;
		bmTCPSend(_id, stPROTO); // 결과 반응 신호 전송
	}

	return ret;
}