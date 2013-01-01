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

/** A class that represents SMCP Command Message.
 * Comprises from SMCPCommandMessageHeader and SMCPCommandMessageData.
 */
class SMCPCommandMessage: public SMCPMessage {

public:
	/** Constructor. */
	SMCPCommandMessage() :
		SMCPMessage() {
		header = new SMCPCommandMessageHeader();
		data = new SMCPCommandMessageData();
	}

public:
	/** Denstructor. */
	virtual ~SMCPCommandMessage() {
		delete header;
		delete data;
	}

public:
	/** Returns string dump of this instance.
	 * @returns string dump of this packet.
	 */
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
	/** Interprets an input byte array as SMCPCommandMessage.
	 * @param[in] data a byte array.
	 * @param[in] length of the input data.
	 */
	void interpretAsCommandMessage(uint8_t* data, size_t length) {
		if (length < SMCPCommandMessageHeader::HeaderLength) {
			throw SMCPException("size error");
		}

		header->setMessageHeader(data);
		((SMCPCommandMessageData*)(this->data))->setCommandTypeID(((SMCPCommandMessageHeader*)header)->getCommandTypeID());
		this->data->setMessageData(data + SMCPCommandMessageHeader::HeaderLength, length
				- SMCPCommandMessageHeader::HeaderLength);
	}

public:
	/** Interprets an input byte array as SMCPCommandMessage.
	 * @param[in] data a byte array.
	 */
	void interpretAsCommandMessage(std::vector<uint8_t>& data) {
		if (data.size() != 0) {
			interpretAsCommandMessage(&(data[0]), data.size());
		}
	}

public:
	/** Checks if a provided SMCPCommandMessage instance has the same content
	 * as this instance does.
	 * @param[in] message SMCPCommandMessage instance.
	 */
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
	/** Returns an SMCPCommandMessageHeader instance. */
	SMCPCommandMessageHeader* getMessageHeader() {
		return (SMCPCommandMessageHeader*) header;
	}

public:
	/** Returns an SMCPCommandMessageData instance. */
	SMCPCommandMessageData* getMessageData() {
		return (SMCPCommandMessageData*) data;
	}

public:
	/** Sets an SMCPCommandMessageHeader instance.
	 * Current header instance is deleted, and replaced by the provided one.
	 * @param[in] header new header instance.
	 */
	void setMessageHeader(SMCPCommandMessageHeader* header) {
		delete this->header;
		this->header = header;
	}

public:
	/** Sets an SMCPCommandMessageData instance.
	 * Current header instance is deleted, and replaced by the provided one.
	 * @param[in] data new data instance.
	 */
	void setMessageData(SMCPCommandMessageData* data) {
		delete this->data;
		this ->data = data;
	}

public:
	/** Sets Command Type ID.
	 * -0000b = Action Command.
	 * -0001b = Get Command.
	 * -0100b = Memory Load Command.
	 * -0101b = Mempry Dump Command.
	 * @param[in] commandTypeID command type id.
	 */
	void setCommandTypeID(std::bitset<4> commandTypeID) {
		((SMCPCommandMessageHeader*)header)->setCommandTypeID(commandTypeID);
		((SMCPCommandMessageData*)data)->setCommandTypeID(commandTypeID);
	}

public:
	/** Sets Command Type ID.
	 * -0000b = Action Command.
	 * -0001b = Get Command.
	 * -0100b = Memory Load Command.
	 * -0101b = Mempry Dump Command.
	 * @param[in] commandTypeID command type id.
	 */
	void setCommandTypeID(uint8_t commandTypeID) {
		std::bitset<4> bits((uint32_t)commandTypeID);
		((SMCPCommandMessageHeader*)header)->setCommandTypeID(bits);
		((SMCPCommandMessageData*)data)->setCommandTypeID(bits);
	}

public:
	/** Sets Command Type ID.
	 * -0000b = Action Command.
	 * -0001b = Get Command.
	 * -0100b = Memory Load Command.
	 * -0101b = Mempry Dump Command.
	 * @param[in] commandTypeIDAsString 4-bit Command Type ID like "0010".
	 */
	void setCommandTypeID(std::string commandTypeIDAsString) {
		((SMCPCommandMessageHeader*)header)->setCommandTypeID(commandTypeIDAsString);
		((SMCPCommandMessageData*)data)->setCommandTypeID(commandTypeIDAsString);
	}
};

/** A class that represents SMCP Action Command Message.
 * Extends SMCPCommandMessage.
 */
class SMCPActionCommandMessage: public SMCPCommandMessage {
public:
	/** Constructor. */
	SMCPActionCommandMessage() :
		SMCPCommandMessage() {
		this->setCommandTypeID(SMCPCommandTypeID::ActionCommand);
	}
};

/** A class that represents SMCP Get Command Message.
 * Extends SMCPCommandMessage.
 */
class SMCPGetCommandMessage: public SMCPCommandMessage {
public:
	/** Constructor. */
	SMCPGetCommandMessage() :
		SMCPCommandMessage() {
		this->setCommandTypeID(SMCPCommandTypeID::GetCommand);
	}
};

/** A class that represents SMCP Memory Load Command Message.
 * Extends SMCPCommandMessage.
 */
class SMCPMemoryLoadCommandMessage: public SMCPCommandMessage {
public:
	/** Constructor. */
	SMCPMemoryLoadCommandMessage() :
		SMCPCommandMessage() {
		this->setCommandTypeID(SMCPCommandTypeID::MemoryLoadCommand);
	}
};

/** A class that represents SMCP Memory Dump Command Message.
 * Extends SMCPCommandMessage.
 */
class SMCPMemoryDumpCommandMessage: public SMCPCommandMessage {
public:
	/** Constructor. */
	SMCPMemoryDumpCommandMessage() :
		SMCPCommandMessage() {
		this->setCommandTypeID(SMCPCommandTypeID::MemoryDumpCommand);
	}
};

#endif /* SMCPCOMMANDMESSAGE_HH_ */
