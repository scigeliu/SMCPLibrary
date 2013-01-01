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

/** An abstract class that represents SMCP Message Header.
 * Used by SMCPMessage.
 * @see SMCPCommandMessageHeader, SMCPTelemetryMessageHeader for implementations.
 */
class SMCPMessageHeader {
public:
	/** Constructor. */
	SMCPMessageHeader() {
		this->setDefaultValues();
	}

public:
	/** Destructor. */
	virtual ~SMCPMessageHeader() {
	}

private:
	int smcpMessageType;

protected:
	std::bitset<2> smcpVersion; //2 bit

public:
	/** Sets default values to member variables. */
	void setDefaultValues() {
		//smcpVersion = 0x01 (00b) fix
		smcpVersion.set(1, 0);
		smcpVersion.set(0, 1);
		smcpMessageType = 0x00;
	}

public:
	virtual std::string toString() = 0;

public:
	virtual std::vector<uint8_t> getAsByteVector() = 0;

public:
	virtual void setMessageHeader(uint8_t* data) = 0;

public:
	virtual void setMessageHeader(std::vector<uint8_t>& data) throw (SMCPException) = 0;

public:
	virtual bool equals(SMCPMessageHeader* header) =0;

public:
	virtual bool equals(SMCPMessageHeader& header) {
		return equals(&header);
	}

public:
	/** Returns SMCP Message Type. */
	int getSMCPMessageType() {
		return smcpMessageType;
	}

public:
	/** Sets SMCP Message Type.
	 * Valid values are SMCPMessageType::CommandMessage and
	 * SMCPMessageType::TelemetryMessage.
	 * @param[in] smcpMessageType SMCP Message Type.
	 */
	void setSMCPMessageType(int smcpMessageType) {
		this->smcpMessageType = smcpMessageType;
	}

};

#endif /* SMCPMESSAGEHEADER_HH_ */
