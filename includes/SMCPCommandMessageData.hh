/*
 * SMCPCommandMessageData.hh
 *
 *  Created on: Jun 5, 2011
 *      Author: yuasa
 */

#ifndef SMCPCOMMANDMESSAGEDATA_HH_
#define SMCPCOMMANDMESSAGEDATA_HH_

#include <iomanip>
#include <vector>
#include "SMCPMessageData.hh"
#include "SMCPException.hh"

class SMCPCommandMessageData: public SMCPMessageData {
public:
	static const size_t MaximumParameterLength = 1006;
	static const size_t MaximumLoadDataLength = 1004;

private:
	std::bitset<4> commandTypeID; //4 bit (not appear in the byte field)

private:
	//Command Message Data section for Action Command
	unsigned char operationID[2]; //2 octets
	std::vector<unsigned char> parameters; //0-1006 octets

	//Command Message Data section for Get Command
	unsigned char attributeID[2]; //2octets

	//Command Message Data section for Memory Load or Dump Command
	unsigned char startAddress[4]; //4octets
	std::vector<unsigned char> loadData; //0-1004 octets

	std::bitset<6> reserved; // 000000
	std::bitset<2> nOfDumps; // 00 = 1time, 01 = 2times, 10 = 3times, 11 = 4times
	unsigned char dumpLength[3];

public:
	SMCPCommandMessageData() {
		this->setSMCPMessageType(SMCPMessageType::CommandMessage);
		this->setMaximumDumpLength(DefaultMaximumDumpLength);
		initializeFieldVariables();
	}

	virtual ~SMCPCommandMessageData() {
	}

public:
	void initializeFieldVariables() {
		operationID[0] = 0x00;
		operationID[1] = 0x00;
		parameters.clear();

		attributeID[0] = 0x00;
		attributeID[1] = 0x00;

		startAddress[0] = 0x00;
		startAddress[1] = 0x00;
		startAddress[2] = 0x00;
		startAddress[3] = 0x00;

		loadData.clear();

		nOfDumps[0] = 0;
		nOfDumps[1] = 0;

		dumpLength[0] = 0x00;
		dumpLength[1] = 0x00;
		dumpLength[2] = 0x00;
	}

public:
	std::string toString() {
		switch (commandTypeID.to_ulong()) {
		case SMCPCommandTypeID::ActionCommand:
			return toStringActionCommand();
			break;
		case SMCPCommandTypeID::GetCommand:
			return toStringGetCommand();
			break;
		case SMCPCommandTypeID::MemoryDumpCommand:
			return toStringMemoryDumpCommand();
			break;
		case SMCPCommandTypeID::MemoryLoadCommand:
			return toStringMemoryLoadCommand();
			break;
		default:
			return "";
			break;
		}
		return "";
	}

private:
	std::string toStringActionCommand() {
		std::stringstream ss;
		using std::endl;
		using std::hex;
		using std::dec;
		using std::setw;
		using std::setfill;
		ss << dec;
		ss << "=== Command Message Data ===" << endl;
		ss << "OperationID        = 0x" << hex << setw(4) << setfill('0')
				<< (unsigned int) ((operationID[0] * 0x100 + operationID[1])) << dec << endl;
		ss << "Parameter Length   = " << dec << parameters.size() << " bytes (decimal)" << endl;
		for (unsigned int i = 0; i < this->getMaximumDumpLength(); i++) {
			if (i < parameters.size()) {
				ss << "Parameter[" << setw(4) << setfill('0') << i << "]    = 0x" << hex << setw(2) << setfill('0')
						<< (unsigned int) (parameters[i]) << dec << endl;
			} else {
				break;
			}
		}

		if (this->getMaximumDumpLength() < parameters.size()) {
			ss << "Parameter continues... (total size = " << parameters.size() << " bytes)" << endl;
		}
		return ss.str();
	}

	std::string toStringGetCommand() {
		std::stringstream ss;
		using std::endl;
		using std::hex;
		using std::dec;
		using std::setw;
		using std::setfill;
		ss << dec;
		ss << "=== Command Message Data ===" << endl;
		ss << "AttributeID        = 0x" << hex << setw(4) << setfill('0') << getAttributeID() << dec << endl;
		return ss.str();
	}

	std::string toStringMemoryDumpCommand() {
		std::stringstream ss;
		using std::endl;
		using std::hex;
		using std::dec;
		using std::setw;
		using std::right;
		using std::setfill;
		ss << dec;
		ss << "=== Command Message Data ===" << endl;
		ss << "AttributeID        = 0x" << hex << setw(4) << setfill('0') << getAttributeID() << dec << endl;
		ss << "Number of dumps    = " << dec << getNOfDumps().to_ulong() + 1 << "(" << getNOfDumps().to_string() << ")"
				<< endl;
		ss << "Start Address      = 0x" << hex << right << setw(8) << setfill('0') << (uint32_t) getStartAddress()
				<< endl;
		ss << "Dump Length        = 0x" << hex << right << setw(6) << setfill('0') << (uint32_t) getDumpLength()
				<< endl;
		return ss.str();
	}

	std::string toStringMemoryLoadCommand() {
		std::stringstream ss;
		using std::endl;
		using std::hex;
		using std::dec;
		using std::setw;
		using std::right;
		using std::setfill;
		ss << dec;
		ss << "=== Command Message Data ===" << endl;
		ss << "Start Address      = 0x" << hex << right << setw(8) << setfill('0') << (uint32_t) getStartAddress()
				<< endl;
		ss << "Load Data Length   = " << dec << loadData.size() << " bytes (decimal)" << endl;
		for (unsigned int i = 0; i < this->getMaximumDumpLength(); i++) {
			if (i < loadData.size()) {
				ss << "Load Data[" << setw(4) << setfill('0') << i << "]    = 0x" << hex << setw(2) << setfill('0')
						<< (unsigned int) (loadData[i]) << dec << endl;
			} else {
				break;
			}
		}

		if (this->getMaximumDumpLength() < loadData.size()) {
			ss << "Load Data continues... (total size = " << loadData.size() << " bytes)" << endl;
		}
		return ss.str();
	}

public:
	std::vector<unsigned char> getAsByteVector() {
		switch (commandTypeID.to_ulong()) {
		case SMCPCommandTypeID::ActionCommand:
			return getAsByteVectorActionCommand();
			break;
		case SMCPCommandTypeID::GetCommand:
			return getAsByteVectorGetCommand();
			break;
		case SMCPCommandTypeID::MemoryDumpCommand:
			return getAsByteVectorMemoryDumpCommand();
			break;
		case SMCPCommandTypeID::MemoryLoadCommand:
			return getAsByteVectorMemoryLoadCommand();
			break;
		default:
			return std::vector<unsigned char>();
			break;
		}
		return std::vector<unsigned char>();
	}

private:
	std::vector<unsigned char> getAsByteVectorActionCommand() {
		std::vector<unsigned char> result;
		result.push_back(operationID[0]);
		result.push_back(operationID[1]);
		result.insert(result.end(), parameters.begin(), parameters.end());
		return result;
	}

	std::vector<unsigned char> getAsByteVectorGetCommand() {
		std::vector<unsigned char> result;
		result.push_back(attributeID[0]);
		result.push_back(attributeID[1]);
		return result;
	}

	std::vector<unsigned char> getAsByteVectorMemoryDumpCommand() {
		std::vector<unsigned char> result;
		result.push_back(startAddress[0]);
		result.push_back(startAddress[1]);
		result.push_back(startAddress[2]);
		result.push_back(startAddress[3]);
		result.insert(result.end(), loadData.begin(), loadData.end());
		return result;
	}

	std::vector<unsigned char> getAsByteVectorMemoryLoadCommand() {
		std::vector<unsigned char> result;
		result.push_back((unsigned char) (nOfDumps.to_ulong()));
		result.push_back(startAddress[0]);
		result.push_back(startAddress[1]);
		result.push_back(startAddress[2]);
		result.push_back(startAddress[3]);
		result.push_back(dumpLength[0]);
		result.push_back(dumpLength[1]);
		result.push_back(dumpLength[2]);
		return result;
	}

public:
	void setMessageData(unsigned char *data, unsigned int length) throw (SMCPException) {
		switch (commandTypeID.to_ulong()) {
		case SMCPCommandTypeID::ActionCommand:
			return setMessageDataActionCommand(data, length);
			break;
		case SMCPCommandTypeID::GetCommand:
			return setMessageDataGetCommand(data, length);
			break;
		case SMCPCommandTypeID::MemoryDumpCommand:
			return setMessageDataMemoryDumpCommand(data, length);
			break;
		case SMCPCommandTypeID::MemoryLoadCommand:
			return setMessageDataMemoryLoadCommand(data, length);
			break;
		default:
			return;
			break;
		}
	}

	void setMessageData(std::vector<unsigned char> data) throw (SMCPException) {
		switch (commandTypeID.to_ulong()) {
		case SMCPCommandTypeID::ActionCommand:
			return setMessageDataActionCommand(data);
			break;
		case SMCPCommandTypeID::GetCommand:
			return setMessageDataGetCommand(data);
			break;
		case SMCPCommandTypeID::MemoryDumpCommand:
			return setMessageDataMemoryDumpCommand(data);
			break;
		case SMCPCommandTypeID::MemoryLoadCommand:
			return setMessageDataMemoryLoadCommand(data);
			break;
		default:
			return;
			break;
		}
	}

private:
	void setMessageDataActionCommand(unsigned char *data, unsigned int length) throw (SMCPException) {
		if (length < 2) {
			throw SMCPException("size error");
		}
		operationID[0] = data[0];
		operationID[1] = data[1];
		parameters.clear();
		for (unsigned int i = 2; i < length; i++) {
			parameters.push_back(data[i]);
		}
	}

	void setMessageDataGetCommand(unsigned char *data, unsigned int length) throw (SMCPException) {
		if (length < 2) {
			throw SMCPException("size error");
		}
		attributeID[0] = data[0];
		attributeID[1] = data[1];
	}

	void setMessageDataMemoryDumpCommand(unsigned char *data, unsigned int length) throw (SMCPException) {
		if (length < 8) {
			throw SMCPException("size error");
		}
		nOfDumps = std::bitset<2>(data[0]);
		startAddress[0] = data[1];
		startAddress[1] = data[2];
		startAddress[2] = data[3];
		startAddress[3] = data[4];
		dumpLength[0] = data[5];
		dumpLength[1] = data[6];
		dumpLength[2] = data[7];
	}

	void setMessageDataMemoryLoadCommand(unsigned char *data, unsigned int length) throw (SMCPException) {
		if (length < 5) {
			throw SMCPException("size error");
		}
		startAddress[0] = data[0];
		startAddress[1] = data[1];
		startAddress[2] = data[2];
		startAddress[3] = data[3];
		loadData.clear();
		for (unsigned int i = 4; i < length; i++) {
			loadData.push_back(data[i]);
		}
	}

private:
	void setMessageDataActionCommand(std::vector<unsigned char> data) throw (SMCPException) {
		if (data.size() != 0) {
			setMessageData(&(data[0]), data.size());
		} else {
			throw SMCPException("size error");
		}
	}

	void setMessageDataGetCommand(std::vector<unsigned char> data) throw (SMCPException) {
		if (data.size() != 0) {
			setMessageDataGetCommand(&(data[0]), data.size());
		} else {
			throw SMCPException("size error");
		}
	}

	void setMessageDataMemoryDumpCommand(std::vector<unsigned char> data) throw (SMCPException) {
		if (data.size() != 0) {
			setMessageDataMemoryDumpCommand(&(data[0]), data.size());
		} else {
			throw SMCPException("size error");
		}
	}

	void setMessageDataMemoryLoadCommand(std::vector<unsigned char> data) throw (SMCPException) {
		if (data.size() != 0) {
			setMessageDataMemoryLoadCommand(&(data[0]), data.size());
		} else {
			throw SMCPException("size error");
		}
	}

public:
	bool equals(SMCPMessageData *data) {
		if (data->getSMCPMessageType() == SMCPMessageType::CommandMessage) {
			equals(*(SMCPCommandMessageData*) (data));
			return true;
		} else {
			return false;
		}
	}

	bool equals(SMCPCommandMessageData& data) {
		if (operationID[0] != data.getOperationIDAsPointer()[0]
				|| operationID[1] != data.getOperationIDAsPointer()[1]) {
			return false;
		}
		if (parameters != data.getParameters()) {
			return false;
		}
		return true;
	}

public:
	std::vector<unsigned char> getOperationID() const {
		std::vector<unsigned char> operationID;
		operationID.push_back(this->operationID[0]);
		operationID.push_back(this->operationID[1]);
		return operationID;
	}

	unsigned char* getOperationIDAsPointer() {
		return this->operationID;
	}

	std::vector<unsigned char> getParameters() const {
		return parameters;
	}

	void setOperationID(std::vector<unsigned char> operationID) {
		if (operationID.size() == 2) {
			this->operationID[0] = operationID[0];
			this->operationID[1] = operationID[1];
		}
	}

	void setOperationID(unsigned int operationID) {
		this->operationID[0] = operationID / 0x100;
		this->operationID[1] = operationID % 0x100;
	}

	void setOperationID(unsigned char* operationID) {
		this->operationID[0] = operationID[0];
		this->operationID[1] = operationID[1];
	}

	void setParameters(std::vector<unsigned char> parameters) {
		this->parameters = parameters;
	}

	void setParameters(unsigned char* parameters, unsigned int length) {
		this->parameters.clear();
		for (int i = 0; i < length; i++) {
			this->parameters.push_back(parameters[i]);
		}
	}

public:
	std::bitset<4> getCommandTypeID() const {
		return commandTypeID;
	}

	void setCommandTypeID(std::bitset<4> commandTypeID) {
		this->commandTypeID = commandTypeID;
	}

	void setCommandTypeID(std::string value) {
		this->commandTypeID = SMCPUtility::createBitset<4>(value);
	}

public:
	static const unsigned int DefaultMaximumDumpLength = 0x08;
	unsigned int getAttributeID() {
		return attributeID[0] * 0x0100 + attributeID[1];
	}

	unsigned int getDumpLength() {
		return dumpLength[0] * 0x010000 + dumpLength[1] * 0x0100 + dumpLength[2];
	}

	std::vector<unsigned char> getLoadData() {
		return loadData;
	}

	std::bitset<2> getNOfDumps() const {
		return nOfDumps;
	}

	unsigned int getStartAddress() {
		return startAddress[0] * 0x01000000 + startAddress[1] * 0x00010000 + startAddress[2] * 0x00000100
				+ startAddress[3] * 0x00000001;
	}

	void setAttributeID(unsigned char* attributeId) {
		this->attributeID[0] = attributeId[0];
		this->attributeID[1] = attributeId[1];
	}

	void setAttributeID(unsigned int attributeId) {
		this->attributeID[0] = attributeId % 0x010000 - attributeId / 0x0100;
		this->attributeID[0] = attributeId % 0x0100;
	}

	void setDumpLength(unsigned char* dumpLength) {
		this->dumpLength[0] = dumpLength[0];
		this->dumpLength[1] = dumpLength[1];
		this->dumpLength[2] = dumpLength[2];
	}

	void setDumpLength(unsigned int dumpLength) {
		this->dumpLength[0] = dumpLength % 0x01000000 - dumpLength / 0x010000;
		this->dumpLength[1] = dumpLength % 0x00010000 - dumpLength / 0x000100;
		this->dumpLength[2] = dumpLength % 0x00000100 - dumpLength / 0x000001;
	}

	void setLoadData(std::vector<unsigned char>& loadData) {
		if (MaximumLoadDataLength < loadData.size()) {
			this->loadData.resize(MaximumLoadDataLength);
			for (size_t i = 0; i < MaximumLoadDataLength; i++) {
				this->loadData[i] = loadData[i];
			}
		} else {
			this->loadData = loadData;
		}
	}

	void setNOfDumps(std::bitset<2> nOfDumps) {
		this->nOfDumps = nOfDumps;
	}

	void setNOfDumps(std::string nOfDumps) {
		this->nOfDumps = SMCPUtility::createBitset<2>(nOfDumps);
	}

	void setStartAddress(unsigned char* startAddress) {
		this->startAddress[0] = startAddress[0];
		this->startAddress[1] = startAddress[1];
		this->startAddress[2] = startAddress[2];
		this->startAddress[3] = startAddress[3];
	}

	void setStartAddress(unsigned int startAddress) {
		this->startAddress[0] = startAddress / 0x01000000;
		this->startAddress[1] = (startAddress % 0x01000000 - startAddress / 0x00010000);
		this->startAddress[2] = (startAddress % 0x00010000 - startAddress / 0x00000100);
		this->startAddress[3] = (startAddress % 0x00000100 - startAddress / 0x00000001);
	}

};

#endif /* SMCPCOMMANDMESSAGEDATA_HH_ */
