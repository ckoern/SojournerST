/*
 * Packet.hpp
 *
 *  Created on: 05.12.2021
 *      Author: Christian
 */

#ifndef INC_PACKET_HPP_
#define INC_PACKET_HPP_

#include <stdint.h>
#include <array>

enum CommandScopeType{
	Global = 0,
	Channel
};
enum CommandBankType{
	Channel1 = 0,
	Channel2
};
enum CommandType{
	PID_STATE=0,
	PID_CONFIG=1,
	ENC_STATE=2,
	CONTROLLER=3
};
enum CommandPidStateParameter{

};

enum ResponseType{

};

struct CommandPacket {
	// sync byte (8 bit)
	// Command scope (global/channel) 1 bit
	// Bank/channel 1 bit
	// Command ID 6 bit (64 command motor commands, 128 global commands)
	// 32 bit value
	// 8 bit checksum?
	uint8_t sync;
	CommandScopeType scope;
	CommandBankType bank;
	CommandType command;
	uint32_t command_value;
	uint8_t checksum;

	bool load( const std::array<command_size>& buffer);
};

struct ResponsePacket{
	uint8_t cmd_checksum;
	ResponseType response_type;
	uint32_t response_value;

	bool put(std::array<response_size>& buffer);

};



#endif /* INC_PACKET_HPP_ */
