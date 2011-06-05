/*
 * SMCPTelemetryMessageData.hh
 *
 *  Created on: Jun 5, 2011
 *      Author: yuasa
 */

#ifndef SMCPTELEMETRYMESSAGEDATA_HH_
#define SMCPTELEMETRYMESSAGEDATA_HH_

#include <vector>
#include "SMCPMessageData.hh"
#include "SMCPException.hh"

class SMCPTelemetryMessageData: public SMCPMessageData {
private:
	unsigned char attributeID[2]; //2 octets
	std::vector<unsigned char> attributeValues; //1 octet - 16M octets
	std::vector<unsigned char> attachment;

public:
	static const unsigned int DefaultMaximumDumpLength = 0x08;

public:
	SMCPTelemetryMessageData() {
		this->setSMCPMessageType(SMCPMessageType::TelemetryMessage);
		this->setMaximumDumpLength(DefaultMaximumDumpLength);
	}

	virtual ~SMCPTelemetryMessageData() {

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
		ss << "AttributeID        = " << hex << setw(4) << setfill('0') << (unsigned int) (attributeID[0] * 0x100
				+ attributeID[1]) << dec << endl;
		ss << "Attr.Val. Length   = " << dec << attributeValues.size() << " bytes (decimal)" << endl;
		for (unsigned int i = 0; i < this->getMaximumDumpLength(); i++) {
			if (i < attributeValues.size()) {
				ss << "Attr.Val.[" << setw(8) << setfill('0') << i << "]= 0x" << hex << setw(2) << setfill('0')
						<< (unsigned int) attributeValues[i] << dec << endl;
			} else {
				break;
			}
		}

		if (this->getMaximumDumpLength() < attributeValues.size()) {
			ss << "The AttributeValues field continue... (total size = " << attributeValues.size() << " bytes)" << endl;
		}
		return ss.str();
	}

public:
	std::vector<unsigned char> getAsByteVector() {
		std::vector<unsigned char> result;
		result.push_back(attributeID[0]);
		result.push_back(attributeID[1]);
		result.insert(result.end(), attributeValues.begin(), attributeValues.end());
		result.insert(result.end(), attachment.begin(), attachment.end());
		return result;
	}

public:
	void setMessageData(unsigned char* data, unsigned int length) throw (SMCPException) {
		if (length < 3) {
			throw SMCPException("size error");
		}
		attributeID[0] = data[0];
		attributeID[1] = data[1];
		attributeValues.clear();
		for (unsigned int i = 2; i < length; i++) {
			attributeValues.push_back(data[i]);
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
		if (data->getSMCPMessageType() == SMCPMessageType::TelemetryMessage) {
			equals(*(SMCPTelemetryMessageData*) data);
		} else {
			return false;
		}
	}

	bool equals(SMCPTelemetryMessageData& data) {
		if (attributeID[0] != data.getAttributeIDAsPointer()[0] || attributeID[1] != data.getAttributeIDAsPointer()[1]) {
			return false;
		}
		if (attributeValues != data.getAttributeValues()) {
			return false;
		}
		return true;
	}

public:
	unsigned int getLength() {
		return 2 + attributeValues.size() + attachment.size();
	}

public:
	std::vector<unsigned char> getAttachment() const {
		return attachment;
	}

	unsigned char* getAttributeIDAsPointer() const {
		return (unsigned char*) attributeID;
	}

	std::vector<unsigned char> getAttributeIDAsPointer() {
		std::vector<unsigned char> attributeID;
		attributeID.push_back(this->attachment[0]);
		attributeID.push_back(this->attachment[1]);
		return attributeID;
	}

	std::vector<unsigned char> getAttributeValues() const {
		return attributeValues;
	}

	void setAttachment(std::vector<unsigned char> attachment) {
		this->attachment = attachment;
	}

	void setAttributeID(std::vector<unsigned char> attributeID) throw(SMCPException){
		if (attributeID.size() == 2) {
			this->attributeID[0] = attributeID[0];
			this->attributeID[1] = attributeID[1];
		}else{
			throw SMCPException("size error");
		}
	}

	void setAttributeID(unsigned int attributeID) {
		this->attributeID[0] = attributeID/0x100;
		this->attributeID[1] = attributeID%0x100;
	}

	void setAttributeID(unsigned char* attributeID) {
		this->attributeID[0] = attributeID[0];
		this->attributeID[1] = attributeID[1];
	}

	void setAttributeValues(std::vector<unsigned char> attributeValues) {
		this->attributeValues = attributeValues;
	}

	void setAttributeValues(unsigned char* attributeValues, unsigned int length) {
		this->attributeValues.clear();
		for (int i = 0; i < length; i++) {
			this->attributeValues.push_back(attributeValues[i]);
		}
	}

};
#endif /* SMCPTELEMETRYMESSAGEDATA_HH_ */
