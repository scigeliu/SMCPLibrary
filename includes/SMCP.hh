/*
 * SMCP.hh
 *
 *  Created on: Jun 5, 2011
 *      Author: yuasa
 */

#ifndef SMCP_HH_
#define SMCP_HH_

/** @mainpage SMCP Library
 * @section intro Overview
 * SMCP Library is a header-only C++ class library
 * that manipulates packets in the SMCP SpacePacket format.
 *
 * See:
 * - http://www.c-soda.isas.jaxa.jp/sog/gstos/file4/SMCP09.pdf
 *
 * Related libraries:
 * - <a href="http://yuasatakayuki.github.com/CCSDSLibrary/">CCSDS SpacePacket Library</a>
 * - <a href="https://yuasatakayuki.github.com/SMCPLibrary">SMCP Library</a>
 *
 * @section install Installation and Build
 * Git repository is hosted on the github.com.
 * To install the latest source tree, execute git clone like below.
 * @code
 * git clone https://github.com/yuasatakayuki/SMCPLibrary
 * @endcode
 *
 * The library does not need to be pre-compiled since this consists
 * only from header files. Just include SDTPLibrary/includes/SMCP.hh
 * and provide a header search path to your compiler.
 *
 * For example, in a source file,
 * @code
 * #include "SMCP.hh"
 * ...your source...
 * @endcode
 * and compile it with a -I flag,
 * @code
 * g++ -I/path/to/SMCPLibrary/includes your_application.cc -o your_application
 * @endcode
 *
 * @section classes Classes
 * Users mainly interface with instances of the SMCPMessage class, or its derived classes
 * SMCPCommandMessage and SMCPTelemetryMessage. The SMCPCommandMessage and SMCPTelemetryMessage classes have concrete classes:
 * - SMCPActionCommandMessage
 * - SMCPGetCommandMessage
 * - SMCPMemoryLoadCommandMessage
 * - SMCPMemoryDumpCommandMessage
 * and
 * - SMCPValueTelemetryMessage
 * - SMCPNotificationTelemetryMessage
 * - SMCPAcknowledgeTelemetryMessage
 * - SMCPMemoryDumpTelemetryMessage.
 *
 * SMCPMessage internally contains
 * - SMCPMessageHeader
 * - SMCPMessageData
 *
 * See <a href="annotated.html">Class List</a> for complete API reference.
 *
 * @section usage Example Usages
 * @subsection example_creation Packet creation
 * @code
 * size_t DataSize = 1002;
 * uint8_t* buffer = new uint8_t[DataSize];
 * for (size_t i = 0; i < DataSize; i++) {
 * 	buffer[i] = (uint8_t) time;
 * }
 *
 * SMCPValueTelemetryMessage* smcpValueTelemetryMessage;
 * smcpValueTelemetryMessage = new SMCPValueTelemetryMessage();
 * smcpValueTelemetryMessage->getMessageHeader()->setLowerFOID(lowerFOID);
 * smcpValueTelemetryMessage->getMessageData()->setAttributeID(attributeID);
 * smcpValueTelemetryMessage->getMessageData()->setAttributeValues(buffer, DataSize);
 * smcpValueTelemetryMessage->setMessageLengthAuto();
 * std::vector<uint8_t> smcpByteArray = smcpValueTelemetryMessage->getAsByteVector();
 *
 * @endcode
 */


#include "SMCPTypeClasses.hh"
#include "SMCPCommandMessage.hh"
#include "SMCPTelemetryMessage.hh"
#include "SMCPUtility.hh"

#endif /* SMCP_HH_ */
