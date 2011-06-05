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

class SMCPMessageData {
private:
	int smcpMessageType;

protected:
	unsigned int maximumDumpLength;

public:
	virtual std::string toString() = 0;

public:
	virtual std::vector<unsigned char> getAsByteVector() = 0;

	virtual void setMessageData(unsigned char* data, unsigned int length) throw (SMCPException) = 0;

	virtual void setMessageData(std::vector<unsigned char> data) throw (SMCPException) =0;

	virtual bool equals(SMCPMessageData* data) =0;

	virtual bool equals(SMCPMessageData& data){
		equals(&data);
	}

	int getSMCPMessageType() const {
		return smcpMessageType;
	}

	void setSMCPMessageType(int smcpMessageType) {
		this->smcpMessageType = smcpMessageType;
	}

	unsigned int getMaximumDumpLength() const {
		return maximumDumpLength;
	}

	void setMaximumDumpLength(unsigned int maximumDumpLength){
		this->maximumDumpLength=maximumDumpLength;
	}

};

#endif /* SMCPMESSAGEDATA_HH_ */
