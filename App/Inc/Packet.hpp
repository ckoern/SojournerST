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
#include "SojournerSTConfig.hpp"

enum CommandScopeType{
	Global = 0,
	Channel
};
enum CommandBankType{
	Channel1 = 0,
	Channel2
};
enum CommandType{
	// --- global command scope ---
	global_foo_bar = 0,
	global_bar_baz,


	// --- channel command scope ---
	// this can collide with the global commands,
	// they can be differentiated with the CommandScopeType bit

	// read only
	channel_current_cps = 0,
	channel_pid_integrator_state,
	channel_pid_filter_state,
	channel_pid_gain,
	channel_pid_setpoint_error,

	// r/w get commands
	channel_pid_get_kp,
	channel_pid_get_ki,
	channel_pid_get_kd,
	channel_pid_get_kn,
	channel_get_target_cps,

	// write only 
	channel_stop,
	channel_pid_reset,

	// r/w set commands, ids are offset by 32 compared to the getters
	channel_pid_set_kp = channel_pid_get_kp + 32,
	channel_pid_set_ki,
	channel_pid_set_kd,
	channel_pid_set_kn,
	channel_set_target_cps,

};

enum ResponseType{
	Success = 1,
	Error_Checksum = 129,
	Error_Sync,
	Error_Value,
	Error_UnknownCommand,

};
namespace communication{
	inline uint8_t calculate_sum( uint8_t* buffer, size_t count ){
		uint8_t sum = 0;
		for (size_t i = 0; i < count; ++i){
			sum += buffer[i];
		}
		return sum;
	}
	inline uint8_t calculate_checksum( uint8_t* buffer, size_t count ){
		uint8_t sum = calculate_sum(buffer, count);
		return -static_cast<uint16_t>(sum); // two's complement
	}
};
struct CommandPacket {
	// sync byte (8 bit)
	// Command scope (global/channel) 1 bit
	// Bank/channel 1 bit
	// Command ID 6 bit (64 command motor commands, 128 global commands)
	// 32 bit value
	// 8 bit checksum
	uint8_t sync;
	CommandScopeType scope;
	CommandBankType bank;
	CommandType command_id;
	uint32_t command_value;
	uint8_t checksum;


	bool operator==(const CommandPacket&) const;

	bool load( const CommandBuffer& buffer);
	bool put(CommandBuffer& buffer);

};

struct ResponsePacket{
	uint8_t cmd_checksum;
	ResponseType response_type;
	uint32_t response_value;

	bool operator==(const ResponsePacket&) const;

	bool load( const ResponseBuffer& buffer);
	bool put(ResponseBuffer& buffer);

};



#endif /* INC_PACKET_HPP_ */
