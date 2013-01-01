/*
 * SMCPMessage.hh
 *
 *  Created on: Jun 5, 2011
 *      Author: yuasa
 */

#ifndef SMCPMESSAGE_HH_
#define SMCPMESSAGE_HH_

#include <string>
#include <sstream>
#include <iostream>
#include "SMCPTypeClasses.hh"
#include "SMCPMessageHeader.hh"
#include "SMCPMessageData.hh"

/** A class that represents SMCP Message.
 * Comprises of instances of SMCPMessageHeader and SMCPMessageData, or derived classes of them.
 */
class SMCPMessage {
public:
	SMCPMessageHeader* header;
	SMCPMessageData* data;

public:
	/** Constructor. */
	SMCPMessage() {
		header = NULL;
		data = NULL;
	}

public:
	/** Destructor. */
	virtual ~SMCPMessage() {
	}

public:
	/** Returns packet content as a vector of uint8_t.
	 * Packet content will be dynamically generated every time
	 * when this method is invoked.
	 * @return a uint8_t vector that contains packet content
	 */
	std::vector<unsigned char> getAsByteVector() {
		std::vector<unsigned char> result;
		std::vector<unsigned char> headerVector = header->getAsByteVector();
		std::vector<unsigned char> dataVector = data->getAsByteVector();
		result.insert(result.end(), headerVector.begin(), headerVector.end());
		result.insert(result.end(), dataVector.begin(), dataVector.end());
		return result;
	}

public:
	/** Returns string value of this instance.
	 * Implementations are provided in derived classes.
	 */
	virtual std::string toString() = 0;

public:
	/** Dumps content to a provided output stream.
	 * @param[in] os output stream instance.
	 */
	virtual void dump(std::ostream& os) {
		os << this->toString();
	}

public:
	/** Dumps content to a provided output stream.
	 * @param[in] os output stream instance.
	 */
	virtual void dump(std::ostream* os) {
		*os << this->toString();
	}

public:
	/** Dumps content to the screen.
	 */
	virtual void dumpToScreen() {
		dump(std::cout);
	}
};

#endif /* SMCPMESSAGE_HH_ */
