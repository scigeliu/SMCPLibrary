/*
 * SMCPMessageHeader.hh
 *
 *  Created on: Jun 5, 2011
 *      Author: yuasa
 */

#ifndef SMCPMESSAGEHEADER_HH_
#define SMCPMESSAGEHEADER_HH_

#include <string>
#include <sstream>
#include <iostream>
#include <bitset>
#include <vector>
#include "SMCPMessage.hh"
#include "SMCPException.hh"

class SMCPMessageHeader {
private:
	int smcpMessageType;

protected:
	std::bitset<2> smcpVersion; //2 bit

public:
	void setDefaultValues(){
		//smcpVersion = 0x01 (00b) fix
		smcpVersion.set(1,0);
		smcpVersion.set(0,1);
	}

public:
	virtual std::string toString() = 0;

public:
	virtual std::vector<unsigned char> getAsByteVector() = 0;

	virtual void setMessageHeader(unsigned char* data) = 0;

	virtual void setMessageHeader(std::vector<unsigned char> data) throw (SMCPException) = 0;

	virtual bool equals(SMCPMessageHeader* header) =0;

	virtual bool equals(SMCPMessageHeader& header){
		equals(&header);
	}

	int getSMCPMessageType() const {
		return smcpMessageType;
	}

	void setSMCPMessageType(int smcpMessageType) {
		this->smcpMessageType = smcpMessageType;
	}

};

#endif /* SMCPMESSAGEHEADER_HH_ */
