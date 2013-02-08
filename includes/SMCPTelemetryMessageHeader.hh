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

/** A class that represents SMCP Telemetry Message Header.
 * Extends SMCPMessageHeader.
 */
class SMCPTelemetryMessageHeader: public SMCPMessageHeader {
private:
	//Telemetry Message Header
	std::bitset<2> reserved; //2 bit
	std::bitset<4> telemetryTypeID; //4 bit
	uint8_t messageLength[3]; //3 octets
	uint8_t lowerFOID; //1 octet

public:
	static const size_t HeaderLength = 0x07;

public:
	static const size_t ReservedFieldValue = 0x00;

public:
	/** Constructor. */
	SMCPTelemetryMessageHeader() {
		this->setDefaultValues();
		lowerFOID = 0x00;
		messageLength[0] = 0x00;
		messageLength[1] = 0x00;
		messageLength[2] = 0x00;
		reserved.reset();
	}

public:
	/** Destructor. */
	virtual ~SMCPTelemetryMessageHeader() {

	}

public:
	/** Returns string dump of this instance.
	 * @returns string dump of this packet.
	 */
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

		ss << "MessageLength      = "
				<< (uint32_t) (messageLength[2] * 0x10000 + messageLength[1] * 0x100 + messageLength[0])
				<< " bytes (decial)" << std::right << endl;

		ss << "lowerFOID          = 0x" << hex << setw(2) << setfill('0') << (uint32_t) lowerFOID << dec << endl;
		return ss.str();
	}

public:
	/** Returns packet content as a vector of uint8_t.
	 * Packet content will be dynamically generated every time
	 * when this method is invoked.
	 * @return a uint8_t vector that contains packet content
	 */
	std::vector<uint8_t> getAsByteVector() {
		std::vector<uint8_t> result;
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
	/** Sets header field values based on a provided uint8_t array.
	 * @param[in] data uint8_t array which contains Telemetry Message Header.
	 */
	void setMessageHeader(uint8_t* data) {
		size_t index = 0;
		reserved.set(1, (data[0] & 0x80) >> 8 /* 1000 0000 */);
		reserved.set(0, (data[0] & 0x40) >> 6 /* 0100 0000 */);
		smcpVersion.set(1, (data[0] & 0x20) >> 5 /* 0010 0000 */);
		smcpVersion.set(0, (data[0] & 0x10) >> 4 /* 0001 0000 */);
		telemetryTypeID.set(3, (data[0] & 0x08) >> 3 /* 0000 1000 */);
		telemetryTypeID.set(2, (data[0] & 0x04) >> 2 /* 0000 0100 */);
		telemetryTypeID.set(1, (data[0] & 0x02) >> 1 /* 0000 0010 */);
		telemetryTypeID.set(0, data[0] & 0x01 /* 0000 0001 */);
		index += 1;
		for (size_t i = 2; index < 4; index++, i--) {
			messageLength[i] = data[index];
		}
		lowerFOID = data[index];
	}

public:
	/** Sets header field values based on a provided uint8_t vector.
	 * @param[in] data uint8_t vector which contains Telemetry Message Header.
	 */
	void setMessageHeader(std::vector<uint8_t>& data) throw (SMCPException) {
		if (data.size() == HeaderLength) {
			setMessageHeader(&(data[0]));
		} else {
			throw SMCPException("size error");
		}
	}

public:
	/** Checks if a provided SMCPMessageHeader instance has the same content
	 * as this instance does.
	 * @param[in] message SMCPMessageHeader instance.
	 */
	bool equals(SMCPMessageHeader* header) {
		if (header->getSMCPMessageType() == SMCPMessageType::TelemetryMessage) {
			return equals(*(SMCPTelemetryMessageHeader*) header);
		} else {
			return false;
		}
	}

public:
	/** Checks if a provided SMCPMessageHeader instance has the same content
	 * as this instance does.
	 * @param[in] message SMCPMessageHeader instance.
	 */
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
		uint8_t* pointer = header.getMessageLengthAsPointer();
		if (messageLength[0] != pointer[0] || messageLength[1] != pointer[1] || messageLength[2] != pointer[2]) {
			return false;
		}
		if (lowerFOID != header.getLowerFOID()) {
			return false;
		}
		return true;
	}

public:
	uint8_t getLowerFOID() const {
		return lowerFOID;
	}

public:
	uint8_t* getMessageLengthAsPointer() const {
		return (uint8_t*) messageLength;
	}

public:
	std::vector<uint8_t> getMessageLength() const {
		std::vector<uint8_t> messageLength;
		messageLength.push_back(this->messageLength[0]);
		messageLength.push_back(this->messageLength[1]);
		messageLength.push_back(this->messageLength[2]);
		return messageLength;
	}

public:
	std::bitset<2> getReserved() const {
		return reserved;
	}

public:
	std::bitset<2> getSMCPVersion() const {
		return smcpVersion;
	}

public:
	std::bitset<4> getTelemetryTypeID() const {
		return telemetryTypeID;
	}

public:
	void setLowerFOID(uint8_t lowerFOID) {
		this->lowerFOID = lowerFOID;
	}

public:
	void setMessageLength(std::vector<uint8_t>& messageLength) {
		if (messageLength.size()) {
			this->messageLength[0] = messageLength[0];
			this->messageLength[1] = messageLength[1];
			this->messageLength[2] = messageLength[2];
		}
	}

public:
	void setMessageLength(uint8_t* messageLength) {
		this->messageLength[0] = messageLength[0];
		this->messageLength[1] = messageLength[1];
		this->messageLength[2] = messageLength[2];
	}

public:
	void setReserved(std::bitset<2>& reserved) {
		this->reserved = reserved;
	}

public:
	void setSMCPVersion(std::bitset<2>& smcpVersion) {
		this->smcpVersion = smcpVersion;
	}

public:
	void setTelemetryTypeID(std::bitset<4>& telemetryTypeID) {
		this->telemetryTypeID = telemetryTypeID;
	}

public:
	void setTelemetryTypeID(uint8_t telemetryTypeID) {
		std::bitset<4> bits((uint32_t)telemetryTypeID);
		this->telemetryTypeID = bits;
	}
};

#endif /* SMCPTELEMETRYMESSAGEHEADER_HH_ */
