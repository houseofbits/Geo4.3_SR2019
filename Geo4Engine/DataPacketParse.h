/*
DataPacketParse<PACKET_TYPE_ENUMERATOR>
	Reads in characters from serial receiver (or any other source) and assembles into data packets, returned to user. 

	Constructor:
		DataPacketParse(name)
			class should be initialized with header name value. Header name is used to address receiver. 
	Functions:
		readPacketByte(c) 
			read in character from data source
		OnReceivePacket(classType, buffer, size)
			virtual function, to be implemented. Handles received data packet
*/
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

static unsigned char CRC8(const unsigned char* data, unsigned short len) {
	unsigned char crc = 0x00;
	while (len--) {
		unsigned char extract = *data++;
		for (unsigned char tempI = 8; tempI; tempI--) {
			unsigned char sum = (crc ^ extract) & 0x01;
			crc >>= 1;
			if (sum) {
				crc ^= 0x8C;
			}
			extract >>= 1;
		}
	}
	return crc;
}

static int memsearch(const char *hay, int haysize, const char *needle, int needlesize) {
	int haypos, needlepos;
	haysize -= needlesize;
	for (haypos = 0; haypos <= haysize; haypos++) {
		for (needlepos = 0; needlepos < needlesize; needlepos++) {
			if (hay[haypos + needlepos] != needle[needlepos]) {
				break;
			}
		}
		if (needlepos == needlesize) {
			return haypos;
		}
	}
	return -1;
}

#define MAX_PAYLOAD_SIZE 512
#define RECEIVE_BUFFER_SIZE 512

class BaseDataPacket {
public:
	BaseDataPacket() {}
	virtual ~BaseDataPacket() {}

	virtual void toBytes(unsigned char* data, unsigned int charSize) {}
	virtual void fromBytes(unsigned char* data, unsigned int charSize) {}
	virtual unsigned int getSize() { return 0; }
	virtual unsigned short getPacketType() = 0;
};

template<typename T, typename PACKET_TYPE_ENUMERATOR>
class DataPacket : public BaseDataPacket {
public:
//	DataPacket() : packet(), packetSize(sizeof(T)){		}
	DataPacket(PACKET_TYPE_ENUMERATOR type) : type(type), packet(), packetSize(sizeof(T)) {	}
	virtual ~DataPacket() {}
	void toBytes(unsigned char* data, unsigned int charSize = sizeof(T)) {
		if (charSize < packetSize)return;
		memcpy(data, &packet.bytes, packetSize);
	}
	void fromBytes(unsigned char* data, unsigned int charSize = sizeof(T)) {
		if (charSize < packetSize)return;
		memcpy(&packet.bytes, data, packetSize);
	}
	unsigned int getSize() { return packetSize; }
	T& getPayload() {
		return this->packet.data;
	}
	unsigned short getPacketType() {
		return (unsigned short)type;
	}
	union {
		T				data;
		unsigned char	bytes[sizeof(T)];
	} packet;
	unsigned int packetSize;
	PACKET_TYPE_ENUMERATOR type;
};

template<typename T>
struct HeaderDataPart {
	char				name[8];
	T					classType;
	unsigned short		payloadSize;
};

template<typename T, typename PACKET_TYPE_ENUMERATOR>
struct CompletePacket {
	HeaderDataPart<PACKET_TYPE_ENUMERATOR>		header;
	T					payload;
	unsigned char		checksum;
};

template<typename PAYLOAD_TYPE, typename PACKET_TYPE_ENUMERATOR>
class CompleteDataPacket : public DataPacket <CompletePacket<PAYLOAD_TYPE, PACKET_TYPE_ENUMERATOR>, PACKET_TYPE_ENUMERATOR> {
public:
	CompleteDataPacket(PACKET_TYPE_ENUMERATOR classType) : DataPacket<CompletePacket<PAYLOAD_TYPE, PACKET_TYPE_ENUMERATOR>, PACKET_TYPE_ENUMERATOR>(classType) {	}
	CompleteDataPacket(const char* header, PACKET_TYPE_ENUMERATOR classType) : CompleteDataPacket(classType) {
		for (unsigned int i = 0; i < 8; i++) {
			this->packet.data.header.name[i] = header[i];
		}
		this->packet.data.header.classType = classType;
		this->packet.data.header.payloadSize = sizeof(PAYLOAD_TYPE);
	}
	CompleteDataPacket(const char* headerName, PACKET_TYPE_ENUMERATOR classType, unsigned short payloadSize) : CompleteDataPacket(headerName, classType) {
		this->packet.data.header.payloadSize = payloadSize;
	}
	void calculateChecksum() {
		this->packet.data.checksum = CRC8((const unsigned char*)&this->packet.data.payload, this->packet.data.header.payloadSize);
	}
	void setPayload(PAYLOAD_TYPE data) {
		this->packet.data.payload = data;
		calculateChecksum();
	}
	PAYLOAD_TYPE& getPayload() {
		return this->packet.data.payload;
	}
};

template<typename PACKET_TYPE_ENUMERATOR>
class DataPacketParse
{
public:
	DataPacketParse() : receiveIndex(0),
		headerData((PACKET_TYPE_ENUMERATOR)0),
		headerStartIndex(0)
	{
		headerSize = sizeof(HeaderDataPart<PACKET_TYPE_ENUMERATOR>);
		_reset();
		memset(headerName, 0, 8);
	}
	DataPacketParse(const char* header) : DataPacketParse() {
		for (unsigned int i = 0; i < 8; i++) {
			headerName[i] = header[i];
		}
	}

	~DataPacketParse() {}

	void readPacketByte(unsigned char c) {
		receiveBuffer[receiveIndex] = c;
		//cout <<"Buff["<<receiveIndex<<"] "<<c<<":"<< receiveBuffer << endl;
		//cout << "Read: "<<state<<" = " << int(receiveBuffer[receiveIndex])<<"("<< receiveBuffer[receiveIndex] <<")" <<" / recindex:"<< receiveIndex<<", headerStart:"<< headerStartIndex << endl;
		switch (state) {
		case WAITING_HEADER_START:
			if (_checkHeaderStart()) {
				state = WAITING_HEADER_END;
			}
		//	break;
		case WAITING_HEADER_END:
			if (_checkHeaderEnd()) {
				state = RECEIVING_DATA;
				//cout << headerData.packet.data.name << " cls:" << headerData.packet.data.classType << " payload size: " << headerData.packet.data.payloadSize << endl;
				receiveIndex = 0;
				return;
			}
			break;
		case RECEIVING_DATA:
			if (_recvData()) {
				state = CHECK_DATA_INTEGRITY;
			}
			break;
		case CHECK_DATA_INTEGRITY:
			unsigned char crc = CRC8((const unsigned char*)&receiveBuffer, headerData.packet.data.payloadSize);
			//cout << "CRC rec:" << (int)receiveBuffer[receiveIndex]<<" calc:"<<(int)crc << " payload size: "<< headerData.packet.data.payloadSize<<endl;
			if (crc == receiveBuffer[receiveIndex]) {
				OnReceivePacket(headerData.packet.data.classType, receiveBuffer, receiveIndex);
			}
			_reset();
			break;
		};
		receiveIndex = (receiveIndex + 1) % RECEIVE_BUFFER_SIZE;
	}
	virtual void OnReceivePacket(PACKET_TYPE_ENUMERATOR classType, unsigned char* buffer, unsigned short size) {	}

private:
	enum State {
		WAITING_HEADER_START = 0,
		WAITING_HEADER_END,
		RECEIVING_DATA,
		CHECK_DATA_INTEGRITY
	};
	void _reset() {
		state = WAITING_HEADER_START;
		receiveIndex = 0;
		headerStartIndex = 0;
		memset(&receiveBuffer[0], 0, RECEIVE_BUFFER_SIZE);
	}
	bool _checkHeaderStart() {
		int found = memsearch((char*)receiveBuffer, 512, headerName, 8);
		if (found >= 0) {
			headerStartIndex = found;
			return true;
		}
		return false;
	}
	bool _checkHeaderEnd() {
		if (receiveIndex - headerStartIndex + 1 == headerSize) {
			headerData.fromBytes(&receiveBuffer[headerStartIndex], headerSize);
			if (headerData.packet.data.payloadSize < (unsigned short)MAX_PAYLOAD_SIZE) {
				return true;
			}
			else _reset();
		}//else check for error
		return false;
	}
	bool _recvData() {
		if (receiveIndex + 1 >= headerData.packet.data.payloadSize) {
			return true;
		}
		return false;
	}

	State						state;
	unsigned char				receiveBuffer[RECEIVE_BUFFER_SIZE];
	DataPacket<HeaderDataPart<PACKET_TYPE_ENUMERATOR>, PACKET_TYPE_ENUMERATOR>	headerData;
	unsigned short				receiveIndex;
	unsigned int				headerStartIndex;
	unsigned short				headerSize;
	char						headerName[8];
};
