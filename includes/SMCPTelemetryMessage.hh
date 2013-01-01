/*
 * SMCPTelemetryMessage.hh
 *
 *  Created on: Jun 5, 2011
 *      Author: yuasa
 */

#ifndef SMCPTELEMETRYMESSAGE_HH_
#define SMCPTELEMETRYMESSAGE_HH_

#include "SMCPTypeClasses.hh"
#include "SMCPMessage.hh"
#include "SMCPTelemetryMessageHeader.hh"
#include "SMCPTelemetryMessageData.hh"
#include "SMCPException.hh"

/** A class which represents an SMCP Telemetry Message.
 * Fields (see details for SMCP09 or ASTH-111):
 * - Telemetry Message Header
 *  - [Reserved 2bits][SMCP Version 2bits][Telemetry Type ID 4bits]
 *  - [Message Length 3octets]
 *  - [Lower FOID 1octet]
 * - Telemetry Message Data
 *  - [Attribute ID 2octets]
 *  - [Attribute Value *octets]
 */
class SMCPTelemetryMessage: public SMCPMessage {
public:
	/** Constructor. */
	SMCPTelemetryMessage() :
		SMCPMessage() {
		header = new SMCPTelemetryMessageHeader();
		data = new SMCPTelemetryMessageData();
	}

public:
	/** Denstructor. */
	virtual ~SMCPTelemetryMessage() {
		delete header;
		delete data;
	}

public:
	/** Returns string dump of this packet.
	 * @returns string dump of this packet.
	 */
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
	/** Interprets an input byte array as SMCPTelemetryMessage.
	 * @param[in] data a byte array.
	 * @param[in] length of the input data.
	 */
	void interpretAsTelemetryMessage(uint8_t* data, size_t length) {
		if (length < SMCPTelemetryMessageHeader::HeaderLength + 1) {
			throw SMCPException("size error");
		}

		header->setMessageHeader(data);
		this->data->setMessageData(data + SMCPTelemetryMessageHeader::HeaderLength, length
				- SMCPTelemetryMessageHeader::HeaderLength);
	}

public:
	/** Interprets an input byte array as SMCPTelemetryMessage.
	 * @param[in] data a byte array.
	 * @param[in] length of the input data.
	 */
	void interpretAsTelemetryMessage(std::vector<uint8_t> data) {
		if (data.size() != 0) {
			interpretAsTelemetryMessage(&(data[0]), data.size());
		}
	}

public:
	/** Checks if a provided SMCPTelemetryMessage instance has the same content
	 * as this instance does.
	 * @param[in] message SMCPCommandMessage instance.
	 */
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
	/** Sets the Message Length field with an appropriate value
	 * calculated from the SMCPTelemetryMessageData part.
	 */
	void setMessageLengthAuto() {
		uint8_t messageLength[3];
		size_t length = 4; //header
		length += ((SMCPTelemetryMessageData*) data)->getLength();
		messageLength[0] = (length % 0x1000000) / 0x10000;
		messageLength[1] = (length % 0x10000) / 0x100;
		messageLength[2] = (length % 0x100) / 0x1;
		((SMCPTelemetryMessageHeader*) header)->setMessageLength(messageLength);
	}

public:
	/** Returns an SMCPTelemetryMessageHeader instance.
	 */
	SMCPTelemetryMessageHeader* getMessageHeader() const {
		return (SMCPTelemetryMessageHeader*) header;
	}


public:
	/** Returns an SMCPTelemetryMessageData instance.
	 */
	SMCPTelemetryMessageData* getMessageData() const {
		return (SMCPTelemetryMessageData*) data;
	}

public:
	/** Sets an SMCPTelemetryMessageHeader instance.
	 * Current header instance is deleted, and replaced by the provided one.
	 * @param[in] header new header instance.
	 */
	void setMessageHeader(SMCPTelemetryMessageHeader* header) {
		delete this->header;
		this->header = header;
	}

public:
	/** Sets an SMCPTelemetryMessageData instance.
	 * Current header instance is deleted, and replaced by the provided one.
	 * @param[in] data new data instance.
	 */
	void setMessageData(SMCPTelemetryMessageData* data) {
		delete this->data;
		this->data = data;
	}

public:
	void setTelemetryTypeID(std::bitset<4>& telemetryTypeID) {
		((SMCPTelemetryMessageHeader*)this->header)->setTelemetryTypeID(telemetryTypeID);
	}

public:
	void setTelemetryTypeID(uint8_t telemetryTypeID) {
		std::bitset<4> bits((uint32_t)telemetryTypeID);
		((SMCPTelemetryMessageHeader*)this->header)->setTelemetryTypeID(bits);
	}

};

/** A class that represents SMCP Value Telemetry Message.
 * Extends SMCPTelemetryMessage.
 */
class SMCPValueTelemetryMessage: public SMCPTelemetryMessage {
public:
	SMCPValueTelemetryMessage() :
		SMCPTelemetryMessage() {
		((SMCPTelemetryMessageHeader*)(this->getMessageHeader()))->setTelemetryTypeID(SMCPTelemetryTypeID::ValueTelemetry);
	}
};

/** A class that represents SMCP Notification Telemetry Message.
 * Extends SMCPTelemetryMessage.
 */
class SMCPNotificationTelemetryMessage: public SMCPTelemetryMessage {
public:
	SMCPNotificationTelemetryMessage() :
		SMCPTelemetryMessage() {
		this->getMessageHeader()->setTelemetryTypeID(SMCPTelemetryTypeID::NotificationTelemetry);
	}
};

/** A class that represents SMCP Acknowledge Telemetry Message.
 * Extends SMCPTelemetryMessage.
 */
class SMCPAcknowledgeTelemetryMessage: public SMCPTelemetryMessage {
public:
	SMCPAcknowledgeTelemetryMessage() :
		SMCPTelemetryMessage() {
		this->getMessageHeader()->setTelemetryTypeID(SMCPTelemetryTypeID::AcknowledgeTelemetry);
	}
};

/** A class that represents SMCP Memory Dump Telemetry Message.
 * Extends SMCPTelemetryMessage.
 */
class SMCPMemoryDumpTelemetryMessage: public SMCPTelemetryMessage {
public:
	SMCPMemoryDumpTelemetryMessage() :
		SMCPTelemetryMessage() {
		this->getMessageHeader()->setTelemetryTypeID(SMCPTelemetryTypeID::MemoryDumpTelemetry);
	}
};

#endif /* SMCPTELEMETRYMESSAGE_HH_ */
