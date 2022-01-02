/*
 * MIT License
 * 
 * Copyright (c) 2021-2022 ckoern
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#ifndef INC_PACKET_HPP_
#define INC_PACKET_HPP_

#include <stdint.h>
#include <cstring>
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
	channel_pid_get_kp = 16,
	channel_pid_get_ki,
	channel_pid_get_kd,
	channel_pid_get_kn,
	channel_get_target_cps,

	// write only 
	channel_stop = 32,
	channel_pid_reset,

	// r/w set commands, ids are offset by 32 compared to the getters
	channel_pid_set_kp = channel_pid_get_kp + setter_cmd_id_offset,
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

	Error_Undefined // generic error
};

namespace communication{
	inline uint8_t calculate_bytesum( uint8_t* buffer, size_t count ){
		uint8_t sum = 0;
		for (size_t i = 0; i < count; ++i){
			sum += buffer[i];
		}
		return sum;
	}
	inline uint8_t calculate_checksum( uint8_t* buffer, size_t count ){
		uint8_t sum = calculate_bytesum(buffer, count);
		return -static_cast<uint16_t>(sum); // implicit two's complement
	}

	// interpret the buffer as a float representation
	inline float   extractValueAsFloat(uint32_t value){
		float val;
		memcpy(&val, 
			   &(value), 
			   sizeof(uint32_t)
		);
		return val;
	}

	inline int32_t extractValueAsInt32(uint32_t value){
		return static_cast<int32_t>(value);
	}

	inline int16_t extractValueAsInt16(uint32_t value){
		return static_cast<int16_t>(static_cast<int32_t>(value));
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

	CommandPacket() = default;
	CommandPacket(const CommandPacket&) = default;
	CommandPacket( CommandScopeType, CommandBankType, CommandType, uint32_t );
	CommandPacket( uint8_t, CommandScopeType, CommandBankType, CommandType, uint32_t, uint8_t );

	bool operator==(const CommandPacket&) const;

	bool load( const CommandBuffer& buffer);
	bool put(CommandBuffer& buffer, bool recalculate_checksum = false);
	void fill_checksum();
};

struct ResponsePacket{
	uint8_t cmd_checksum;
	ResponseType response_type;
	uint32_t response_value;
	uint8_t checksum;

	ResponsePacket() = default;
	ResponsePacket(const ResponsePacket&) = default;
	ResponsePacket(uint8_t, ResponseType, uint32_t);
	ResponsePacket(uint8_t, ResponseType, uint32_t, uint8_t);

	bool operator==(const ResponsePacket&) const;

	bool load( const ResponseBuffer& buffer);
	bool put(ResponseBuffer& buffer, bool recalculate_checksum = false);
	void fill_checksum();

	inline void setValueFromFloat(float value);
};


void ResponsePacket::setValueFromFloat(float value){
    memcpy(&response_value, 
           &(value), 
           sizeof(uint32_t)
    );
}

#endif /* INC_PACKET_HPP_ */
