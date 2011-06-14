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

class SMCPAcknowledgeRequest {
public:
	enum {
		NoAcknowledgeTelemetry = 0x00, //00b
		RequestAcknowledgeTelemetry = 0x01, //01b
		Undefined = 0xffff
	};
};

class SMCPCommandTypeID {
public:
	enum {
		ActionCommand = 0x00, //0000b
		GetCommand = 0x01, //0001b
		MemoryLoadCommand = 0x04, //0100b
		MemoryDumpCommand = 0x05, //0101b
		Undefined = 0xffff
	};
};
class SMCPCommandMessageHeader: public SMCPMessageHeader {
private:
	//Command Message Header section
	std::bitset<2> acknowledgeRequest;//2 bit
	std::bitset<4> commandTypeID; //4 bit
	unsigned char lowerFOID; //1 octet

public:
	static const unsigned int HeaderLength = 0x02; //2bytes

public:
	SMCPCommandMessageHeader() {
		this->setDefaultValues();
	}

	virtual ~SMCPCommandMessageHeader() {

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
	std::vector<unsigned char> getAsByteVector() {
		std::vector<unsigned char> result;
		result.push_back( //
				((unsigned char) acknowledgeRequest.to_ulong()) * 0x40 //
						+ ((unsigned char) smcpVersion.to_ulong()) * 0x10 //
						+ (unsigned char) (commandTypeID.to_ulong()) //
		);
		result.push_back(lowerFOID);
		return result;
	}

public:
	void setMessageHeader(unsigned char* data) {
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

	void setMessageHeader(std::vector<unsigned char> data) throw (SMCPException) {
		if (data.size() == HeaderLength) {
			setMessageHeader(&(data[0]));
		} else {
			throw SMCPException("size error");
		}
	}

public:
	bool equals(SMCPMessageHeader* header) {
		if (header->getSMCPMessageType() == SMCPMessageType::CommandMessage) {
			equals(*(SMCPCommandMessageHeader*) header);
		} else {
			return false;
		}
	}

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
	std::bitset<2> getAcknowledgeRequest() const {
		return acknowledgeRequest;
	}

	std::bitset<4> getCommandTypeID() const {
		return commandTypeID;
	}

	unsigned char getLowerFOID() const {
		return lowerFOID;
	}

	std::bitset<2> getSMCPVersion() const {
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

	void setLowerFOID(unsigned char lowerFOID) {
		this->lowerFOID = lowerFOID;
	}

	void setSMCPVersion(std::bitset<2> smcpVersion) {
		this->smcpVersion = smcpVersion;
	}

};
#endif /* SMCPCOMMANDMESSAGEHEADER_HH_ */
