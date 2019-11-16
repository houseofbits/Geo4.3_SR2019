#include "Geo4.h"

CLASS_DECLARATION(BasicSerial);

BasicSerial::BasicSerial() : Entity(),
CSerialEx(),
EventHandler(),
lightDataPacket("SR2019LT", PacketClassEnumerator::LIGHT_DATA),
moverDataPacket("SR2019LT", PacketClassEnumerator::MOVER_DATA),
wingLightDataPacket("SR2019LT", PacketClassEnumerator::WING_LIGHT_DATA)
{
	memset(outputBuffer, 0, MAX_PAYLOAD_SIZE);
}


BasicSerial::~BasicSerial()
{
}
void BasicSerial::Initialise(EventManager*const eventManager, ResourceManager*const resourceManager)
{
	eventManager->RegisterEventHandler(this);
	eventManager->RegisterEventReceiver(this, &BasicSerial::OnWindowEvent);
	eventManager->RegisterEventReceiver(this, &BasicSerial::OnGUIInputEvent);
}

void BasicSerial::Deserialize(CFONode* node)
{
	Entity::Deserialize(node);

	node->getValueString("serial_port", configPortName);
	node->getValueString("baud_rate", configBaudRate);
	node->getValueString("stop_bits", configStopBits);
	node->getValueString("parity", configParity);

	OpenPort(configPortName, configBaudRate, configStopBits, configParity);
}

bool BasicSerial::OnWindowEvent(WindowEvent*const event) 
{


	return 1;
}

#define DATA_SIZE 252

void BasicSerial::updateLightData(WingsKeyframe& k)
{

	for (unsigned int a = 0; a < 12; a++)
	{
		for (unsigned int i = 0; i < 7; i++)
		{
			lightDataOut.wingData[a].colors[i].r = (unsigned char)(k.colors[a][i].x * 255.0f);
			lightDataOut.wingData[a].colors[i].g = (unsigned char)(k.colors[a][i].y * 255.0f);
			lightDataOut.wingData[a].colors[i].b = (unsigned char)(k.colors[a][i].z * 255.0f);
		}
	}

	outputBuffer[0] = (unsigned char)83;
	outputBuffer[1] = (unsigned char)82;
	outputBuffer[2] = (unsigned char)49;
	outputBuffer[3] = (unsigned char)57;

	unsigned char* dta = (unsigned char*)&lightDataOut.wingData;

	if (IsOpen()) {

		Write(outputBuffer, 4, 0, 0, 10);

		Write(dta, DATA_SIZE, 0, 0, 10);

		outputBuffer[0] = CRC8(dta, DATA_SIZE);

		Write(outputBuffer, 1, 0, 0, 10);
	}
}




bool BasicSerial::OnGUIInputEvent(GUIInputEvent*const event)
{	
	
	LightDataOutStructure ldo;

	for (unsigned int a = 0; a < 12; a++)
	{
		for (unsigned int i = 0; i < 7; i++)
		{
			ldo.wingData[a].colors[i].r = (unsigned char)Math::RangeRandom(0, 255);
			ldo.wingData[a].colors[i].g = (unsigned char)Math::RangeRandom(0, 255);
			ldo.wingData[a].colors[i].b = (unsigned char)Math::RangeRandom(0, 255);
		}
	}
	/*
	ldo.wingData[0].colors[0].r = 0;
	ldo.wingData[0].colors[0].g = 255;
	ldo.wingData[0].colors[0].b = 0;

	ldo.wingData[0].colors[1].r = 0;
	ldo.wingData[0].colors[1].g = 0;
	ldo.wingData[0].colors[1].b = 0;
	*/
	

	/*
	ldo.wingData[0].colors[1].r = 0;
	ldo.wingData[0].colors[1].g = 0;
	ldo.wingData[0].colors[1].b = 255;
	LightWingData lwd;
	lwd.colors[1].r = (unsigned char)Math::RangeRandom(0, 255);
	lwd.colors[1].g = (unsigned char)Math::RangeRandom(0, 255);
	lwd.colors[1].b = (unsigned char)Math::RangeRandom(0, 255);
	lwd.wingId = 0;
	*/

	switch (event->type) {
	case GUIInputEvent::EventType::KEYUP:
		//cout << event->keyCode << endl;
		//'Q'
		if (event->keyCode == 20) {
			
			//cout << "----------------------------------------" << endl;

			memset(&outputBuffer, 0, MAX_PAYLOAD_SIZE);

			outputBuffer[0] = (unsigned char)83;
			outputBuffer[1] = (unsigned char)82;
			outputBuffer[2] = (unsigned char)49;
			outputBuffer[3] = (unsigned char)57;

			unsigned char* dta = (unsigned char*)&ldo.wingData;

			/*
			outputBuffer[4] = (unsigned char)1;
			outputBuffer[5] = (unsigned char)2;
			outputBuffer[6] = (unsigned char)3;
			outputBuffer[7] = (unsigned char)4;

			outputBuffer[8] = (unsigned char)5;

			outputBuffer[9] = (unsigned char)6;
			outputBuffer[10] = (unsigned char)7;

			unsigned char crc = CRC8(&outputBuffer[4], DATA_SIZE);
			outputBuffer[4 + DATA_SIZE] = crc;
			*/

			


			if (IsOpen()) {

				Write(outputBuffer, 4, 0, 0, 10);

				Write(dta, DATA_SIZE, 0, 0, 10);

				outputBuffer[0] = CRC8(dta, DATA_SIZE);

				Write(outputBuffer, 1, 0, 0, 10);
			}

			//lightDataPacket.setPayload(lightDataOut);
			//WritePacket(&lightDataPacket);	

			//wingLightDataPacket.setPayload(lwd);
			//WritePacket(&wingLightDataPacket);
		}
		//'W'
		if (event->keyCode == 26) {
		//	moverDataPacket.setPayload(MoverDataOutStructure());
		//	WritePacket(&moverDataPacket);
		//	WritePacket(&moverDataPacket);
		//	WritePacket(&moverDataPacket);
		}
		break;

	}
	return true;
}

bool BasicSerial::OpenPort(string name, string baudStr, string stop, string parityStr) {

	if (IsOpen())Close();

	LONG    lLastError = ERROR_SUCCESS;
	std::wstring ws;
	ws.assign(name.begin(), name.end());
	lLastError = Open(&ws[0]);
	if (lLastError != ERROR_SUCCESS) {
		cout << "Unable to open COM-port: " << name << endl;
		return false;
	}
	else {
		cout << "Port is open " << name << endl;
	}
	CSerial::EBaudrate baud = CSerial::EBaud9600;
	CSerial::EStopBits stopb = CSerial::EStop2;
	CSerial::EParity parity = CSerial::EParNone;
	CSerial::EDataBits datab = CSerial::EData8;

	if (baudStr.length() > 0) {
		if (baudStr == "110")baud = CSerial::EBaud110;
		else if (baudStr == "300")baud = CSerial::EBaud300;
		else if (baudStr == "600")baud = CSerial::EBaud600;
		else if (baudStr == "1200")baud = CSerial::EBaud1200;
		else if (baudStr == "2400")baud = CSerial::EBaud2400;
		else if (baudStr == "4800")baud = CSerial::EBaud4800;
		else if (baudStr == "9600")baud = CSerial::EBaud9600;
		else if (baudStr == "14400")baud = CSerial::EBaud14400;
		else if (baudStr == "19200")baud = CSerial::EBaud19200;
		else if (baudStr == "38400")baud = CSerial::EBaud38400;
		else if (baudStr == "56000")baud = CSerial::EBaud56000;
		else if (baudStr == "57600")baud = CSerial::EBaud57600;
		else if (baudStr == "115200")baud = CSerial::EBaud115200;
		else if (baudStr == "128000")baud = CSerial::EBaud128000;
		else if (baudStr == "256000")baud = CSerial::EBaud256000;
	}
	if (stop.length() > 0) {
		if (stop == "1")stopb = CSerial::EStop1;
		else if (stop == "2")stopb = CSerial::EStop2;
	}
	if (parityStr.length() > 0) {
		if (parityStr == "none")parity = CSerial::EParNone;
		else if (parityStr == "odd")parity = CSerial::EParOdd;
		else if (parityStr == "even")parity = CSerial::EParEven;
		else if (parityStr == "mark")parity = CSerial::EParMark;
		else if (parityStr == "space")parity = CSerial::EParSpace;
	}
	Setup(baud, datab, parity, stopb);
	SetupHandshaking(CSerial::EHandshakeOff);
	SetMask(CSerial::EEventBreak |
		CSerial::EEventCTS |
		CSerial::EEventDSR |
		CSerial::EEventError |
		CSerial::EEventRing |
		CSerial::EEventRLSD |
		CSerial::EEventRecv);

	StartListener();

	return true;
}

void BasicSerial::OnSerialEvent(EEvent eEvent, EError eError) {
	
	if (eEvent & CSerial::EEventRecv) {
		DWORD dwBytesRead = 0;
		char szBuffer[101];
		do {
			Read(szBuffer, sizeof(szBuffer) - 1, &dwBytesRead);

			if (dwBytesRead > 0) {
				szBuffer[dwBytesRead] = '\0';
			}
			
			cout << szBuffer;

		} while (dwBytesRead == sizeof(szBuffer) - 1);
		/*
		std::lock_guard<std::mutex> lock(*mtx);
		for (unsigned int i = 0; i < dwBytesRead; i++) {
			inputBuffer.push(szBuffer[i]);
		}*/
	}
	
}

void BasicSerial::WritePacket(BaseDataPacket* packet) {

	//	WritePacketToFile(packet, "status_packet.bin");
	//	return;

	size = packet->getSize();
	cout << "Send packet of size: " << size << endl;
	packet->toBytes(outputBuffer, size);
	if (IsOpen())Write(outputBuffer, size, 0, 0, 10);
}

void BasicSerial::WritePacketToFile(BaseDataPacket* packet, string filename) {
	size = packet->getSize();
	packet->toBytes(outputBuffer, size);
	FILE* pFile;
	fopen_s(&pFile, filename.c_str(), "wb");
	fwrite(outputBuffer, 1, size, pFile);
	fclose(pFile);
	cout << "Packet written to file '" << filename.c_str() << "'" << endl;
}