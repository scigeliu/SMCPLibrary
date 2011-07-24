/*
 * SMCPCommandMessage.hh
 *
 *  Created on: Jun 5, 2011
 *      Author: yuasa
 */

#ifndef SMCPCOMMANDMESSAGE_HH_
#define SMCPCOMMANDMESSAGE_HH_

#include "SMCPMessage.hh"
#include "SMCPCommandMessageHeader.hh"
#include "SMCPCommandMessageData.hh"

class SMCPCommandMessage: public SMCPMessage {

public:
	SMCPCommandMessage() :
		SMCPMessage() {
		header = new SMCPCommandMessageHeader();
		data = new SMCPCommandMessageData();
	}

	virtual ~SMCPCommandMessage() {
		delete header;
		delete data;
	}

public:
	std::string toString() {
		std::stringstream ss;
		using std::endl;
		ss << "---------------------------------" << endl;
		ss << "SMCPCommandMessage" << endl;
		ss << "---------------------------------" << endl;
		ss << header->toString();
		ss << data->toString();
		ss << endl;
		return ss.str();
	}

public:
	void interpretAsCommandMessage(unsigned char* data, unsigned int length) {
		if (length < SMCPCommandMessageHeader::HeaderLength) {
			throw SMCPException("size error");
		}

		header->setMessageHeader(data);
		this->data->setMessageData(data + SMCPCommandMessageHeader::HeaderLength, length
				- SMCPCommandMessageHeader::HeaderLength);
	}

	void interpretAsCommandMessage(std::vector<unsigned char>& data) {
		if (data.size() != 0) {
			interpretAsCommandMessage(&(data[0]), data.size());
		}
	}

public:
	bool equals(SMCPCommandMessage& message) {
		if (!header->equals(*message.getMessageHeader())) {
			return false;
		}
		if (!data->equals(*message.getMessageData())) {
			return false;
		}
		return true;
	}

public:
	SMCPCommandMessageHeader* getMessageHeader() {
		return (SMCPCommandMessageHeader*) header;
	}

	SMCPCommandMessageData* getMessageData() {
		return (SMCPCommandMessageData*) data;
	}

	void setMessageHeader(SMCPCommandMessageHeader* header) {
		delete this->header;
		this->header = header;
	}

	void setMessageData(SMCPCommandMessageData* data) {
		delete this->data;
		this ->data = data;
	}
};

class SMCPActionCommandMessage: public SMCPCommandMessage {
public:
	SMCPActionCommandMessage() :
		SMCPCommandMessage() {
		this->getMessageHeader()->setCommandTypeID(SMCPCommandTypeID::ActionCommand);
	}
};

class SMCPGetCommandMessage: public SMCPCommandMessage {
public:
	SMCPGetCommandMessage() :
		SMCPCommandMessage() {
		this->getMessageHeader()->setCommandTypeID(SMCPCommandTypeID::GetCommand);
	}
};

class SMCPMemoryLoadCommandMessage: public SMCPCommandMessage {
public:
	SMCPMemoryLoadCommandMessage() :
		SMCPCommandMessage() {
		this->getMessageHeader()->setCommandTypeID(SMCPCommandTypeID::MemoryLoadCommand);
	}
};

class SMCPMemoryDumpCommandMessage: public SMCPCommandMessage {
public:
	SMCPMemoryDumpCommandMessage() :
		SMCPCommandMessage() {
		this->getMessageHeader()->setCommandTypeID(SMCPCommandTypeID::MemoryDumpCommand);
	}
};

#endif /* SMCPCOMMANDMESSAGE_HH_ */
