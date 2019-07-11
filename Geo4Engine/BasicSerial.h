#pragma once

#include "Geo4.h"
#include "Serial.h"
#include "DataPacketRequest.h"

enum PacketClassEnumerator : unsigned short
{
	HEADER = 0,
	LIGHT_DATA,
	MOVER_DATA,
};

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
}LightColor;
typedef struct {
	LightColor colors[7];
}LightWing;
struct LightDataOutStructure {
	LightWing wingData[12];
};
struct MoverDataOutStructure {
	unsigned char data[12];
};

typedef DataPacket<LightDataOutStructure, PacketClassEnumerator> LightDataPacket;
typedef DataPacket<MoverDataOutStructure, PacketClassEnumerator> MoverDataPacket;

class BasicSerial : public Entity, public CSerialEx
{
CLASS_PROTOTYPE(BasicSerial);
public:
	BasicSerial();
	~BasicSerial();

	void	Initialise(EventManager*const, ResourceManager*const);
	void	Deinitialise(EventManager*const, ResourceManager*const) {};

	void	Serialize(CFONode*) {}
	void	Deserialize(CFONode*);

	bool	OnWindowEvent(WindowEvent*const);

	bool	OpenPort(string name, string baud, string stop, string parity);

	void	OnSerialEvent(EEvent eEvent, EError eError);

	bool	isRenderable() { return false; }

	void	WritePacket(BaseDataPacket* packet);
	void	WritePacketToFile(BaseDataPacket* packet, string filename);

	unsigned char	outputBuffer[MAX_PAYLOAD_SIZE];
	unsigned int	size;

	string	configPortName;
	string	configBaudRate;
	string	configStopBits;
	string	configParity;
};

