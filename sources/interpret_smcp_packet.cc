/*
 * interpret_smcp_packet.cc
 *
 *  Created on: Jul 24, 2011
 *      Author: yuasa
 */

#include "SMCP.hh"
#include <iostream>
#include <vector>
#include <sstream>

int toInteger(std::string str) {
	using namespace std;
	stringstream ss;
	ss << str;
	int avalue;
	if (str.size() >= 2 && str[0] == '0' && (str[1] == 'X' || str[1] == 'x')) {
		ss >> hex >> avalue;
	} else {
		ss >> avalue;
	}
	return avalue;
}

int main(int argc,char* argv[]){
	using namespace std;

	if(argc==1){
		cerr << "interpret_smcp_packet (byte array)" << endl;
		exit(-1);
	}

	SMCPCommandMessage* packet=new SMCPCommandMessage();
	vector<unsigned char> data;
	for(int i=1;i<argc;i++){
		data.push_back((unsigned char)toInteger(argv[i]));
	}
	packet->interpretAsCommandMessage(data);
	packet->dumpToScreen();
}
