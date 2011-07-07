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
private:
	//Command Message Data section
	unsigned char operationID[2]; //2 octets
	std::vector<unsigned char> parameters; //0-1006 octets

public:
	static const unsigned int DefaultMaximumDumpLength = 0x08;

public:
	SMCPCommandMessageData() {
		this->setSMCPMessageType(SMCPMessageType::CommandMessage);
		this->setMaximumDumpLength(DefaultMaximumDumpLength);
	}

	virtual ~SMCPCommandMessageData() {

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

		ss << "=== Command Message Data ===" << endl;
		ss << "OperationID        = " << hex << setw(4) << setfill('0') << (unsigned int) (operationID[0] * 0x100
				+ operationID[1]) << dec << endl;
		ss << "Parameter Length   = " << dec << parameters.size() << " bytes (decimal)" << endl;
		for (unsigned int i = 0; i < this->getMaximumDumpLength(); i++) {
			if (i < parameters.size()) {
				ss << "Parameter[" << setw(4) << setfill('0') << i << "]    = 0x" << hex << setw(2) << setfill('0')
						<< (unsigned int) parameters[i] << dec << endl;
			} else {
				break;
			}
		}

		if (this->getMaximumDumpLength() < parameters.size()) {
			ss << "Parameter continues... (total size = " << parameters.size() << " bytes)" << endl;
		}
		return ss.str();
	}

public:
	std::vector<unsigned char> getAsByteVector() {
		std::vector<unsigned char> result;
		result.push_back(operationID[0]);
		result.push_back(operationID[1]);
		result.insert(result.end(), parameters.begin(), parameters.end());
		return result;
	}

public:
	void setMessageData(unsigned char* data, unsigned int length) throw (SMCPException) {
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

	void setMessageData(std::vector<unsigned char> data) throw (SMCPException) {
		if (data.size() != 0) {
			setMessageData(&(data[0]), data.size());
		} else {
			throw SMCPException("size error");
		}
	}

public:
	bool equals(SMCPMessageData* data) {
		if (data->getSMCPMessageType() == SMCPMessageType::CommandMessage) {
			equals(*(SMCPCommandMessageData*) data);
		} else {
			return false;
		}
	}

	bool equals(SMCPCommandMessageData& data) {
		if (operationID[0] != data.getOperationIDAsPointer()[0] || operationID[1] != data.getOperationIDAsPointer()[1]) {
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
};

#endif /* SMCPCOMMANDMESSAGEDATA_HH_ */
