/*
 * SMCPMessage.hh
 *
 *  Created on: Jun 5, 2011
 *      Author: yuasa
 */

#ifndef SMCPMESSAGE_HH_
#define SMCPMESSAGE_HH_

class SMCPMessageType {
public:
	enum {
		CommandMessage = 0x00, //
		TelemetryMessage = 0x01
	};
};

#include <string>
#include <sstream>
#include <iostream>
#include "SMCPMessageHeader.hh"
#include "SMCPMessageData.hh"

class SMCPMessage {
public:
	SMCPMessageHeader* header;
	SMCPMessageData* data;

public:
	std::vector<unsigned char> getAsByteVector() {
		std::vector<unsigned char> result;
		result.insert(result.end(), header->getAsByteVector().begin(), header->getAsByteVector().end());
		result.insert(result.end(), data->getAsByteVector().begin(), data->getAsByteVector().end());
		return result;
	}

public:
	virtual std::string toString() = 0;

	virtual void dump(std::ostream& os){
		os << this->toString();
	}

	virtual void dump(std::ostream* os){
		*os << this->toString();
	}

	virtual void dumpToScreen(){
		dump(std::cout);
	}
};

#endif /* SMCPMESSAGE_HH_ */
