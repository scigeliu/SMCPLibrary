/*
 * SMCPCommandMessageHeader.hh
 *
 *  Created on: Jun 5, 2011
 *      Author: yuasa
 */

#ifndef SMCPCOMMANDMESSAGEHEADER_HH_
#define SMCPCOMMANDMESSAGEHEADER_HH_

#include <bitset>
#include <vector>
#include <iomanip>
#include "SMCPMessageHeader.hh"
#include "SMCPException.hh"
#include "SMCPUtility.hh"

/** A class that represents SMCP Command Message Header.
 * Extends SMCPMessageHeader.
 */
class SMCPCommandMessageHeader: public SMCPMessageHeader {
private:
	//Command Message Header section
	std::bitset<2> acknowledgeRequest;//2 bit
	std::bitset<4> commandTypeID; //4 bit
	uint8_t lowerFOID; //1 octet

public:
	static const unsigned int HeaderLength = 0x02; //2bytes

public:
	/** Constructor. */
	SMCPCommandMessageHeader() {
		this->setDefaultValues();
	}

public:
	/** Denstructor. */
	virtual ~SMCPCommandMessageHeader() {

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

		ss << "AcknowledgeRequest = " << acknowledgeRequest.to_string() << "b ";
		if (acknowledgeRequest.to_ulong() == SMCPAcknowledgeRequest::NoAcknowledgeTelemetry) {
			ss << "(NoAcknowledgeTelemetry)" << endl;
		} else if (acknowledgeRequest.to_ulong() == SMCPAcknowledgeRequest::RequestAcknowledgeTelemetry) {
			ss << "(AcknowledgeTelemetry is requested)" << endl;
		} else {
			ss << "(Undefined value)" << endl;
		}

		ss << "SMCP Version       = " << smcpVersion.to_string() << "b" << endl;

		ss << "CommandTypeID      = " << commandTypeID.to_string() << "b ";
		switch (commandTypeID.to_ulong()) {
		case SMCPCommandTypeID::ActionCommand:
			ss << "(ActionCommand)" << endl;
			break;
		case SMCPCommandTypeID::GetCommand:
			ss << "(GetCommand)" << endl;
			break;
		case SMCPCommandTypeID::MemoryLoadCommand:
			ss << "(MemoryLoadCommand)" << endl;
			break;
		case SMCPCommandTypeID::MemoryDumpCommand:
			ss << "(MemoryDumpCommand)" << endl;
			break;
		default:
			ss << "(Undefined value)" << endl;
			break;
		}

		ss << "lowerFOID          = 0x" << hex << setw(2) << setfill('0') << (unsigned int) lowerFOID << dec << endl;

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
				((uint8_t) acknowledgeRequest.to_ulong()) * 0x40 //
						+ ((uint8_t) smcpVersion.to_ulong()) * 0x10 //
						+ (uint8_t) (commandTypeID.to_ulong()) //
		);
		result.push_back(lowerFOID);
		return result;
	}

public:
	/** Sets header field values based on a provided uint8_t array.
	 * @param[in] data uint8_t array which contains 2-byte Messaeg Header.
	 */
	void setMessageHeader(uint8_t* data) {
		acknowledgeRequest.set(1, (data[0] & 0x80) >> 8 /* 1000 0000 */);
		acknowledgeRequest.set(0, (data[0] & 0x40) >> 6 /* 0100 0000 */);
		smcpVersion.set(1, (data[0] & 0x20) >> 5 /* 0010 0000 */);
		smcpVersion.set(0, (data[0] & 0x10) >> 4 /* 0001 0000 */);
		commandTypeID.set(3, (data[0] & 0x08) >> 3 /* 0000 1000 */);
		commandTypeID.set(2, (data[0] & 0x04) >> 2 /* 0000 0100 */);
		commandTypeID.set(1, (data[0] & 0x02) >> 1 /* 0000 0010 */);
		commandTypeID.set(0, data[0] & 0x01 /* 0000 0001 */);
		lowerFOID = data[1];
	}

public:
	/** Sets header field values based on a provided uint8_t array.
	 * @param[in] data uint8_t vector which contains 2-byte Messaeg Header.
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
		if (header->getSMCPMessageType() == SMCPMessageType::CommandMessage) {
			return equals(*(SMCPCommandMessageHeader*) header);
		} else {
			return false;
		}
	}

public:
	/** Checks if a provided SMCPMessageHeader instance has the same content
	 * as this instance does.
	 * @param[in] message SMCPMessageHeader instance.
	 */
	bool equals(SMCPCommandMessageHeader& header) {
		if (acknowledgeRequest != header.getAcknowledgeRequest()) {
			return false;
		}
		if (smcpVersion != header.getSMCPVersion()) {
			return false;
		}
		if (commandTypeID != header.getCommandTypeID()) {
			return false;
		}
		if (lowerFOID != header.getLowerFOID()) {
			return false;
		}
		return true;
	}

public:
	std::bitset<2>& getAcknowledgeRequest() {
		return acknowledgeRequest;
	}

	std::bitset<4>& getCommandTypeID() {
		return commandTypeID;
	}

	uint8_t getLowerFOID() {
		return lowerFOID;
	}

	std::bitset<2> getSMCPVersion() {
		return smcpVersion;
	}

	void setAcknowledgeRequest(std::bitset<2> acknowledgeRequest) {
		this->acknowledgeRequest = acknowledgeRequest;
	}

	void setAcknowledgeRequest(std::string value) {
		this->acknowledgeRequest = SMCPUtility::createBitset<2>(value);
	}

	void setCommandTypeID(std::bitset<4> commandTypeID) {
		this->commandTypeID = commandTypeID;
	}

	void setCommandTypeID(std::string value) {
		this->commandTypeID = SMCPUtility::createBitset<4>(value);
	}

	void setLowerFOID(uint8_t lowerFOID) {
		this->lowerFOID = lowerFOID;
	}

	void setSMCPVersion(std::bitset<2> smcpVersion) {
		this->smcpVersion = smcpVersion;
	}

};
#endif /* SMCPCOMMANDMESSAGEHEADER_HH_ */
