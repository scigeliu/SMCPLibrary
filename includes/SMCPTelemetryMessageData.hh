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

/** A class that represents SMCP Telemetry Message Data.
 */
class SMCPTelemetryMessageData: public SMCPMessageData {
private:
	uint8_t attributeID[2]; //2 octets
	std::vector<uint8_t> attributeValues; //1 octet - 16M octets
	std::vector<uint8_t> attachment;

public:
	static const size_t DefaultMaximumDumpLength = 0x08;

public:
	/** Constructor. */
	SMCPTelemetryMessageData() {
		this->setSMCPMessageType(SMCPMessageType::TelemetryMessage);
		this->setMaximumDumpLength(DefaultMaximumDumpLength);
	}

public:
	/** Destructor. */
	virtual ~SMCPTelemetryMessageData() {

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

		ss << "=== Command Message Data ===" << endl;
		ss << "AttributeID        = " << hex << setw(4) << setfill('0') << (unsigned int) (attributeID[0] * 0x100
				+ attributeID[1]) << dec << endl;
		ss << "Attr.Val. Length   = " << dec << attributeValues.size() << " bytes (decimal)" << endl;
		for (size_t i = 0; i < this->getMaximumDumpLength(); i++) {
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
	/** Returns packet content as a vector of uint8_t.
	 * Packet content will be dynamically generated every time
	 * when this method is invoked.
	 * @return a uint8_t vector that contains packet content
	 */
	std::vector<uint8_t> getAsByteVector() {
		std::vector<uint8_t> result;
		result.push_back(attributeID[0]);
		result.push_back(attributeID[1]);
		result.insert(result.end(), attributeValues.begin(), attributeValues.end());
		result.insert(result.end(), attachment.begin(), attachment.end());
		return result;
	}

public:
	/** Sets Message Data based on a provided uint8_t array.
	 * @param[in] data uint8_t array which contains Message Data.
	 * @param[in] length the length of the array.
	 */
	void setMessageData(uint8_t* data, size_t length) throw (SMCPException) {
		if (length < 3) {
			throw SMCPException("size error");
		}
		attributeID[0] = data[0];
		attributeID[1] = data[1];
		attributeValues.clear();
		for (size_t i = 2; i < length; i++) {
			attributeValues.push_back(data[i]);
		}
	}

public:
	/** Sets Message Data based on a provided uint8_t vector.
	 * @param[in] data uint8_t vector which contains Message Data.
	 */
	void setMessageData(std::vector<uint8_t>& data) throw (SMCPException) {
		if (data.size() != 0) {
			setMessageData(&(data[0]), data.size());
		} else {
			throw SMCPException("size error");
		}
	}

public:
	/** Checks if a provided SMCPMessageData instance has the same content
	 * as this instance does.
	 * @param[in] message SMCPMessageData instance.
	 */
	bool equals(SMCPMessageData* data) {
		if (data->getSMCPMessageType() == SMCPMessageType::TelemetryMessage) {
			return equals(*(SMCPTelemetryMessageData*) data);
		} else {
			return false;
		}
	}

public:
	/** Checks if a provided SMCPMessageData instance has the same content
	 * as this instance does.
	 * @param[in] message SMCPMessageData instance.
	 */
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
	/** Returns the size of SMCP Telemetry Message Data part.
	 * Calculation based on
	 * "2 + attributeValues.size() + attachment.size()"
	 */
	size_t getLength() {
		return 2 + attributeValues.size() + attachment.size();
	}

public:
	std::vector<uint8_t> getAttachment() const {
		return attachment;
	}

public:
	uint8_t* getAttributeIDAsPointer() const {
		return (uint8_t*) attributeID;
	}

public:
	std::vector<uint8_t> getAttributeIDAsPointer() {
		std::vector<uint8_t> attributeID;
		attributeID.push_back(this->attachment[0]);
		attributeID.push_back(this->attachment[1]);
		return attributeID;
	}

public:
	std::vector<uint8_t> getAttributeValues() const {
		return attributeValues;
	}

public:
	void setAttachment(std::vector<uint8_t>& attachment) {
		this->attachment = attachment;
	}

public:
	void setAttributeID(std::vector<uint8_t>& attributeID) throw (SMCPException) {
		if (attributeID.size() == 2) {
			this->attributeID[0] = attributeID[0];
			this->attributeID[1] = attributeID[1];
		} else {
			throw SMCPException("size error");
		}
	}

public:
	void setAttributeID(uint16_t attributeID) {
		this->attributeID[0] = attributeID / 0x100;
		this->attributeID[1] = attributeID % 0x100;
	}

public:
	void setAttributeID(uint8_t* attributeID) {
		this->attributeID[0] = attributeID[0];
		this->attributeID[1] = attributeID[1];
	}

public:
	void setAttributeValues(std::vector<uint8_t>& attributeValues) {
		this->attributeValues = attributeValues;
	}

public:
	void setAttributeValues(uint8_t* attributeValues, size_t length) {
		this->attributeValues.clear();
		for (int i = 0; i < length; i++) {
			this->attributeValues.push_back(attributeValues[i]);
		}
	}

};
#endif /* SMCPTELEMETRYMESSAGEDATA_HH_ */
