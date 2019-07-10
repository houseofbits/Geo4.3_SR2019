/*
DataPacketRequestManager<PACKET_TYPE_ENUMERATOR>
	Handles data packet requests - transmission and reception.
*/
#pragma once

#include <chrono>
#include <ctime>
#include "DataPacketParse.h"

class BasePacketRequest {
public:
	BasePacketRequest() : state(), error() {

	}
	enum State {
		FINISHED = 0,
		PENDING_TRANSMISSION,
		PENDING_RESPONSE
	};
	enum Error {
		OK = 0,
		RESPONSE_TIMEOUT,
		PESPONSE_WRONG_CLASS
	};
	void commit() {
		if (state == FINISHED) {
			state = PENDING_TRANSMISSION;
			error = OK;
		}
	}
	void wait() {
		state = PENDING_RESPONSE;
		error = OK;
	}
	void finish(Error err = OK) {
		state = FINISHED;
		error = err;
	}
	bool isCompleted() {
		return (isFinished() && !isError());
	}
	bool isFinished() {
		return state == FINISHED;
	}
	bool isError() {
		return error != OK;
	}
	virtual BaseDataPacket* getTransmitPacket() = 0;
	virtual BaseDataPacket* getResponsePacket() = 0;

	State			state;
	Error			error;
};

template<class TRANSMIT_PACKET_TYPE, class RESPONSE_PACKET_TYPE>
class DataPacketRequest : public BasePacketRequest {
public:
	DataPacketRequest() : BasePacketRequest(),
		transmitPacket(0),
		responsePacket(0){	}
	BaseDataPacket* getTransmitPacket() {
		return transmitPacket;
	}
	BaseDataPacket* getResponsePacket() {
		return responsePacket;
	}
	TRANSMIT_PACKET_TYPE*	transmitPacket;
	RESPONSE_PACKET_TYPE*	responsePacket;
};

template<typename PACKET_TYPE_ENUMERATOR>
class DataPacketRequestManager : public DataPacketParse<PACKET_TYPE_ENUMERATOR> {
public:
	DataPacketRequestManager() : DataPacketParse<PACKET_TYPE_ENUMERATOR>(), 
		requests(), 
		timerStart() {
		timerStart = std::chrono::steady_clock::now();
	}
	DataPacketRequestManager(const char* header) : DataPacketParse<PACKET_TYPE_ENUMERATOR>(header), 
		requests(), 
		timerStart() {
		timerStart = std::chrono::steady_clock::now();
	}

	void RegisterRequest(BasePacketRequest* request) {
		requests.push_back(request);
	}
	//To be implemented in Hardware class
	virtual void			WritePacket(BaseDataPacket*) = 0;
	virtual unsigned char	getInputData() = 0;
	virtual bool			hasInputData() = 0;

	//Implemented from DataPacketParse
	virtual void OnReceivePacket(PACKET_TYPE_ENUMERATOR classType, unsigned char* buffer, unsigned short size) {
		//cout << "receive packet " << classType<<" "<< size << endl;
		//only one request can be pending response at given time
		for (std::vector<BasePacketRequest*>::iterator it = requests.begin(); it != requests.end(); ++it) {
			if ((*it)->state == BasePacketRequest::PENDING_RESPONSE) {
				if ((*it)->getResponsePacket() && classType == (PACKET_TYPE_ENUMERATOR)(*it)->getResponsePacket()->getPacketType() &&
					size == (*it)->getResponsePacket()->getSize()) {
					(*it)->getResponsePacket()->fromBytes(buffer, size);
					(*it)->finish();
					return;
				}
			}
		}
		//no response was handled, set errors
		for (std::vector<BasePacketRequest*>::iterator it = requests.begin(); it != requests.end(); ++it) {
			if ((*it)->state == BasePacketRequest::PENDING_RESPONSE) {
				(*it)->finish(BasePacketRequest::PESPONSE_WRONG_CLASS);
			}
		}
	}
	//Usually run in separate thread
	void processRequests() {

		if (requests.empty())return;
		
		/*
		for (std::vector<BasePacketRequest*>::iterator it = requests.begin(); it != requests.end(); ++it) {
			cout << "request state:" << (*it)->state <<" err:" <<(*it)->error<<" t:"<<
				std::chrono::duration_cast<chrono::milliseconds>(std::chrono::steady_clock::now() - timerStart).count()<<endl;
		}
		*/

		if (std::chrono::duration_cast<chrono::milliseconds>(std::chrono::steady_clock::now() - timerStart).count() > 2000) {
			for (std::vector<BasePacketRequest*>::iterator it = requests.begin(); it != requests.end(); ++it) {
				if ((*it)->state == BasePacketRequest::PENDING_RESPONSE) {
					(*it)->finish(BasePacketRequest::RESPONSE_TIMEOUT);
				}
			}
		}
		//read some data
		while (hasInputData()) {
			readPacketByte(getInputData());
		}
		//check if any one request is pending response, if so, wait some more
		for (std::vector<BasePacketRequest*>::iterator it = requests.begin(); it != requests.end(); ++it) {
			if ((*it)->state == BasePacketRequest::PENDING_RESPONSE)return;
		}
		//cher if any request is pending transmission
		for (std::vector<BasePacketRequest*>::iterator it = requests.begin(); it != requests.end(); ++it) {
			if ((*it)->state == BasePacketRequest::PENDING_TRANSMISSION) {
				if ((*it)->getTransmitPacket()) {
					WritePacket((*it)->getTransmitPacket());
					(*it)->wait();
					timerStart = std::chrono::steady_clock::now();
					return;
				}
			}
		}
		timerStart = std::chrono::steady_clock::now();
	}

	std::vector<BasePacketRequest*>	requests;
	std::chrono::time_point<std::chrono::steady_clock>	timerStart;
};
