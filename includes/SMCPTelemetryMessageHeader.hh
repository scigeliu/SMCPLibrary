/*
 * SMCPTelemetryMessageHeader.hh
 *
 *  Created on: Jun 5, 2011
 *      Author: yuasa
 */

#ifndef SMCPTELEMETRYMESSAGEHEADER_HH_
#define SMCPTELEMETRYMESSAGEHEADER_HH_

#include <bitset>
#include "SMCPMessageHeader.hh"
#include "SMCPException.hh"

class SMCPTelemetryTypeID {
public:
	enum {
		ValueTelemetry = 0x00, //0000b
		NotificationTelemetry = 0x01, //0001b
		AcknowledgeTelemetry = 0x02, //0010b
		MemoryDumpTelemetry = 0x05, //0101b
		Undefined = 0xffff
	};
};

class SMCPTelemetryMessageHeader: public SMCPMessageHeader {
private:
	//Telemetry Message Header
	std::bitset<2> reserved;//2 bit
	std::bitset<4> telemetryTypeID; //4 bit
	unsigned char messageLength[3]; //3 octets
	unsigned char lowerFOID; //1 octet

public:
	static const unsigned int HeaderLength = 0x07;

public:
	static const unsigned int ReservedFieldValue = 0x00;

public:
	SMCPTelemetryMessageHeader() {
		this->setDefaultValues();
		reserved.reset();
	}

	virtual ~SMCPTelemetryMessageHeader() {

	}

public:
	std::string toString() {
		std::stringstream ss;
		using std::endl;
		using std::hex;
		using std::dec;
		using std::setw;
		using std::setfill;

		ss << dec;

		ss << "=== Command Message Header ===" << endl;

		ss << "Reserved           = " << reserved.to_string() << "b ";
		if (reserved.to_ulong() != ReservedFieldValue) {
			ss << "(Undefined value)" << endl;
		} else {
			ss << "(Reserved)" << endl;
		}

		ss << "SMCP Version       = " << smcpVersion.to_string() << "b" << endl;

		ss << "TelemetryTypeID    = " << telemetryTypeID.to_string() << "b ";
		switch (telemetryTypeID.to_ulong()) {
		case SMCPTelemetryTypeID::ValueTelemetry:
			ss << "(ValueTelemetry)" << endl;
			break;
		case SMCPTelemetryTypeID::NotificationTelemetry:
			ss << "(NotificationTelemetry)" << endl;
			break;
		case SMCPTelemetryTypeID::AcknowledgeTelemetry:
			ss << "(AcknowledgeTelemetry)" << endl;
			break;
		case SMCPTelemetryTypeID::MemoryDumpTelemetry:
			ss << "(MemoryDumpTelemetry)" << endl;
			break;
		default:
			ss << "(Undefined value)" << endl;
			break;
		}

		ss << "MessageLength      = " << (unsigned int) (messageLength[2] * 0x10000 + messageLength[1] * 0x100
				+ messageLength[0]) << " bytes (decial)" << std::right << endl;

		ss << "lowerFOID          = 0x" << hex << setw(2) << setfill('0') << (unsigned int) lowerFOID << dec << endl;
		return ss.str();
	}

public:
	std::vector<unsigned char> getAsByteVector() {
		std::vector<unsigned char> result;
		result.push_back( //
				reserved.to_ulong() * 0x40 //
						+ smcpVersion.to_ulong() * 0x10 //
						+ telemetryTypeID.to_ulong() //
		);
		result.push_back(messageLength[0]);
		result.push_back(messageLength[1]);
		result.push_back(messageLength[2]);
		result.push_back(lowerFOID);
		return result;
	}

public:
	/** param[in] data a byte array.
	 */
	void setMessageHeader(unsigned char* data) {
		unsigned int index = 0;
		reserved.set(1, (data[0] & 0x80) >> 8 /* 1000 0000 */);
		reserved.set(0, (data[0] & 0x40) >> 6 /* 0100 0000 */);
		smcpVersion.set(1, (data[0] & 0x20) >> 5 /* 0010 0000 */);
		smcpVersion.set(0, (data[0] & 0x10) >> 4 /* 0001 0000 */);
		telemetryTypeID.set(3, (data[0] & 0x08) >> 3 /* 0000 1000 */);
		telemetryTypeID.set(2, (data[0] & 0x04) >> 2 /* 0000 0100 */);
		telemetryTypeID.set(1, (data[0] & 0x02) >> 1 /* 0000 0010 */);
		telemetryTypeID.set(0, data[0] & 0x01 /* 0000 0001 */);
		index += 1;
		for (unsigned int i = 0; index < 4; index++, i++) {
			messageLength[i] = data[index];
		}
		lowerFOID = data[index];
	}

	void setMessageHeader(std::vector<unsigned char> data) throw (SMCPException) {
		if (data.size() == HeaderLength) {
			setMessageHeader(&(data[0]));
		} else {
			throw SMCPException("size error");
		}
	}

public:
	bool equals(SMCPMessageHeader* header) {
		if (header->getSMCPMessageType() == SMCPMessageType::TelemetryMessage) {
			equals(*(SMCPTelemetryMessageHeader*) header);
		} else {
			return false;
		}
	}

	bool equals(SMCPTelemetryMessageHeader& header) {
		if (reserved != header.getReserved()) {
			return false;
		}
		if (smcpVersion != header.getSMCPVersion()) {
			return false;
		}
		if (telemetryTypeID != header.getTelemetryTypeID()) {
			return false;
		}
		unsigned char* pointer = header.getMessageLengthAsPointer();
		if (messageLength[0] != pointer[0] || messageLength[1] != pointer[1] || messageLength[2] != pointer[2]) {
			return false;
		}
		if (lowerFOID != header.getLowerFOID()) {
			return false;
		}
		return true;
	}

public:
	unsigned char getLowerFOID() const {
		return lowerFOID;
	}

	unsigned char* getMessageLengthAsPointer() const {
		return (unsigned char*) messageLength;
	}

	std::vector<unsigned char> getMessageLength() const {
		std::vector<unsigned char> messageLength;
		messageLength.push_back(this->messageLength[0]);
		messageLength.push_back(this->messageLength[1]);
		messageLength.push_back(this->messageLength[2]);
		return messageLength;
	}

	std::bitset<2> getReserved() const {
		return reserved;
	}

	std::bitset<2> getSMCPVersion() const {
		return smcpVersion;
	}

	std::bitset<4> getTelemetryTypeID() const {
		return telemetryTypeID;
	}

	void setLowerFOID(unsigned char lowerFOID) {
		this->lowerFOID = lowerFOID;
	}

	void setMessageLength(std::vector<unsigned char> messageLength) {
		if (messageLength.size()) {
			this->messageLength[0] = messageLength[0];
			this->messageLength[1] = messageLength[1];
			this->messageLength[2] = messageLength[2];
		}
	}

	void setMessageLength(unsigned char* messageLength) {
		this->messageLength[0] = messageLength[0];
		this->messageLength[1] = messageLength[1];
		this->messageLength[2] = messageLength[2];
	}

	void setReserved(std::bitset<2> reserved) {
		this->reserved = reserved;
	}

	void setSMCPVersion(std::bitset<2> smcpVersion) {
		this->smcpVersion = smcpVersion;
	}

	void setTelemetryTypeID(std::bitset<4> telemetryTypeID) {
		this->telemetryTypeID = telemetryTypeID;
	}
};

#endif /* SMCPTELEMETRYMESSAGEHEADER_HH_ */
