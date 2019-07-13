#pragma once

#include "Geo4.h"
#include "Serial.h"
#include "DataPacketRequest.h"

enum PacketClassEnumerator : unsigned short
{
	HEADER = 0,
	WING_LIGHT_DATA,
	LIGHT_DATA,
	MOVER_DATA,
};

typedef struct {
	unsigned char g;
	unsigned char r;
	unsigned char b;
}LightColor;
typedef struct {
	LightColor colors[7];
}LightWing;
typedef struct {
	LightColor	colors[7];
	uint8_t		wingId;
}LightWingData;

struct LightDataOutStructure {
	LightWing wingData[12];
};
struct MoverDataOutStructure {
	unsigned char data[12];
};

typedef CompleteDataPacket<LightWingData, PacketClassEnumerator> WingLightDataPacket;
typedef CompleteDataPacket<LightDataOutStructure, PacketClassEnumerator> LightDataPacket;
typedef CompleteDataPacket<MoverDataOutStructure, PacketClassEnumerator> MoverDataPacket;

class BasicSerial : public Entity, 
	public CSerialEx,
	public EventHandler
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
	bool	OnGUIInputEvent(GUIInputEvent*const);

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


	WingLightDataPacket wingLightDataPacket;


	LightDataPacket		lightDataPacket;
	MoverDataPacket		moverDataPacket;

	void updateLightData(WingsKeyframe&);

	LightDataOutStructure	lightDataOut;
};

