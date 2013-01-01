/*
 * SMCPMessaegType.hh
 *
 *  Created on: Dec 31, 2012
 *      Author: yuasa
 */

#ifndef SMCPMESSAEGTYPE_HH_
#define SMCPMESSAEGTYPE_HH_

/** A class which collects SMCP Message Type IDs. */
class SMCPMessageType {
public:
	enum {
		CommandMessage = 0x00, //
		TelemetryMessage = 0x01
	};
};

/** A class which collects SMCP Telemetry Type IDs.
 * Used by SMCPTelemetryMessageHeader.
 */
class SMCPTelemetryTypeID {
public:
	static const uint8_t ValueTelemetry = 0x00; //0000b
	static const uint8_t NotificationTelemetry = 0x01; //0001b
	static const uint8_t AcknowledgeTelemetry = 0x02; //0010b
	static const uint8_t MemoryDumpTelemetry = 0x05; //0101b
};

/** A class which collects SMCP Acknowledge Request.
 * Used by SMCPCommandMessageHeader.
 */
class SMCPAcknowledgeRequest {
public:
	static const uint8_t NoAcknowledgeTelemetry = 0x00; //00b
	static const uint8_t RequestAcknowledgeTelemetry = 0x01; //01b
};

/** A class which collects SMCP Command Type ID.
 * Used by SMCPCommandMessageHeader.
 */
class SMCPCommandTypeID {
public:
	static const uint8_t ActionCommand = 0x00; //0000b
	static const uint8_t GetCommand = 0x01; //0001b
	static const uint8_t MemoryLoadCommand = 0x04; //0100b
	static const uint8_t MemoryDumpCommand = 0x05; //0101b
};


#endif /* SMCPMESSAEGTYPE_HH_ */
