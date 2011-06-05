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

class SMCPException : public std::exception {
private:
	std::string message;
public:
	SMCPException(std::string message){
		this->message=message;
	}
	virtual ~SMCPException() throw(){

	}
public:
	std::string getMessage(){
		return message;
	}
	const char *what() const throw(){
		return message.c_str();
	}
};

#endif /* SMCPEXCEPTION_HH_ */
