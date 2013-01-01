/*
 * SMCPException.hh
 *
 *  Created on: Jun 5, 2011
 *      Author: yuasa
 */

#ifndef SMCPEXCEPTION_HH_
#define SMCPEXCEPTION_HH_

#include <exception>
#include <string>

/** An exception class which is used by SMCPLibrary.
 */
class SMCPException: public std::exception {
private:
	std::string message;

public:
	/** Constructor. */
	SMCPException(std::string message) {
		this->message = message;
	}

public:
	/** Destructor. */
	virtual ~SMCPException() throw () {

	}

public:
	/** Returns status string.
	 */
	std::string getMessage() {
		return message;
	}
public:
	/** Returns status string as const char*. */
	const char *what() const throw () {
		return message.c_str();
	}
};

#endif /* SMCPEXCEPTION_HH_ */
