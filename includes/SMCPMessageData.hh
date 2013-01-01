/*
 * SMCPMessageData.hh
 *
 *  Created on: Jun 5, 2011
 *      Author: yuasa
 */

#ifndef SMCPMESSAGEDATA_HH_
#define SMCPMESSAGEDATA_HH_

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "SMCPMessage.hh"
#include "SMCPException.hh"

/** A class that represents SMCP Command Message Data.
 * Extends SMCPMessageData.
 */
class SMCPMessageData {
private:
	uint32_t smcpMessageType;

protected:
	size_t maximumDumpLength;

public:
	SMCPMessageData(){}

public:
	virtual ~SMCPMessageData(){}

public:
	virtual std::string toString() = 0;

public:
	virtual std::vector<uint8_t> getAsByteVector() = 0;

public:
	virtual void setMessageData(uint8_t* data, size_t length) throw (SMCPException) = 0;

public:
	virtual void setMessageData(std::vector<uint8_t>& data) throw (SMCPException) =0;

public:
	virtual bool equals(SMCPMessageData* data) =0;

public:
	virtual bool equals(SMCPMessageData& data) {
		return equals(&data);
	}

public:
	/** Returns SMCP Message Type. */
	int getSMCPMessageType() const {
		return smcpMessageType;
	}

public:
	/** Sets SMCP Message Type.
	 * @see SMCPMessageType.
	 */
	void setSMCPMessageType(int smcpMessageType) {
		this->smcpMessageType = smcpMessageType;
	}

public:
	/** Returns the maximum dump length used when dumping
	 * packet content to the screen.
	 */
	size_t getMaximumDumpLength() const {
		return maximumDumpLength;
	}

public:
	/** Sets the maximum dump length used when dumping
	 * packet content to the screen.
	 */
	void setMaximumDumpLength(size_t maximumDumpLength) {
		this->maximumDumpLength = maximumDumpLength;
	}

};

#endif /* SMCPMESSAGEDATA_HH_ */
