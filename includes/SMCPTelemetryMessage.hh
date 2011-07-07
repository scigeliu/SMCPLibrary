/*
 * SMCPTelemetryMessage.hh
 *
 *  Created on: Jun 5, 2011
 *      Author: yuasa
 */

#ifndef SMCPTELEMETRYMESSAGE_HH_
#define SMCPTELEMETRYMESSAGE_HH_

#include "SMCPMessage.hh"
#include "SMCPTelemetryMessageHeader.hh"
#include "SMCPTelemetryMessageData.hh"
#include "SMCPException.hh"

/* A class which represents an SMCP Telemetry Message.
 * Fields (see details for SMCP09 or ASTH-111):
 * --Telemetry Message Header--
 * [Reserved 2bits][SMCP Version 2bits][Telemetry Type ID 4bits]
 * [Message Length 3octets]
 * [Lower FOID 1octet]
 * --Telemetry Message Data--
 * [Attribute ID 2octets]
 * [Attribute Value *octets]
 */
class SMCPTelemetryMessage: public SMCPMessage {
public:
	SMCPTelemetryMessage() :
		SMCPMessage() {
		header = new SMCPTelemetryMessageHeader();
		data = new SMCPTelemetryMessageData();
	}

	virtual ~SMCPTelemetryMessage() {
		delete header;
		delete data;
	}

public:
	std::string toString() {
		std::stringstream ss;
		using std::endl;
		ss << "---------------------------------" << endl;
		ss << "SMCPTelemetryMessage" << endl;
		ss << "---------------------------------" << endl;
		ss << header->toString();
		ss << data->toString();
		ss << endl;
		return ss.str();
	}

public:
	void interpretAsTelemetryMessage(unsigned char* data, unsigned int length) {
		if (length < SMCPTelemetryMessageHeader::HeaderLength + 1) {
			throw SMCPException("size error");
		}

		header->setMessageHeader(data);
		this->data->setMessageData(data + SMCPTelemetryMessageHeader::HeaderLength, length
				- SMCPTelemetryMessageHeader::HeaderLength);
	}

	void interpretAsTelemetryMessage(std::vector<unsigned char> data) {
		if (data.size() != 0) {
			interpretAsTelemetryMessage(&(data[0]), data.size());
		}
	}

public:
	bool equals(SMCPTelemetryMessage& message) {
		if (!header->equals(*message.getMessageHeader())) {
			return false;
		}
		if (!data->equals(*message.getMessageData())) {
			return false;
		}
		return true;
	}

public:
	void setMessageLengthAuto() {
		unsigned char messageLength[3];
		unsigned int length = 4; //header
		length += ((SMCPTelemetryMessageData*) data)->getLength();
		messageLength[0] = (length % 0x1000000) / 0x10000;
		messageLength[1] = (length % 0x10000) / 0x100;
		messageLength[2] = (length % 0x100) / 0x1;
		((SMCPTelemetryMessageHeader*) header)->setMessageLength(messageLength);
	}

public:
	SMCPTelemetryMessageHeader* getMessageHeader() const {
		return (SMCPTelemetryMessageHeader*) header;
	}

	SMCPTelemetryMessageData* getMessageData() const {
		return (SMCPTelemetryMessageData*) data;
	}

	void setMessageHeader(SMCPTelemetryMessageHeader* header) {
		delete this->header;
		this->header = header;
	}

	void setMessageData(SMCPTelemetryMessageData* data) {
		delete this->data;
		this->data = data;
	}

};

class SMCPValueTelemetryMessage: public SMCPTelemetryMessage {
public:
	SMCPValueTelemetryMessage() :
		SMCPTelemetryMessage() {
		this->getMessageHeader()->setTelemetryTypeID(SMCPTelemetryTypeID::ValueTelemetry);
	}
};

class SMCPNotificationTelemetryMessage: public SMCPTelemetryMessage {
public:
	SMCPNotificationTelemetryMessage() :
		SMCPTelemetryMessage() {
		this->getMessageHeader()->setTelemetryTypeID(SMCPTelemetryTypeID::NotificationTelemetry);
	}
};

class SMCPAcknowledgeTelemetryMessage: public SMCPTelemetryMessage {
public:
	SMCPAcknowledgeTelemetryMessage() :
		SMCPTelemetryMessage() {
		this->getMessageHeader()->setTelemetryTypeID(SMCPTelemetryTypeID::AcknowledgeTelemetry);
	}
};

class SMCPMemoryDumpTelemetryMessage: public SMCPTelemetryMessage {
public:
	SMCPMemoryDumpTelemetryMessage() :
		SMCPTelemetryMessage() {
		this->getMessageHeader()->setTelemetryTypeID(SMCPTelemetryTypeID::MemoryDumpTelemetry);
	}
};

#endif /* SMCPTELEMETRYMESSAGE_HH_ */
