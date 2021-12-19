#include "Packet.hpp"

bool CommandType::load( const std::array<command_size>& buffer){
    sync = buffer[0];
    scope = buffer[1] >> 7;
    bank = (buffer[1] >> 6) & 0b01;
    command = buffer[1] & 0x3f; //lower 6 bits
    value =   buffer[2] << 24
            + buffer[3] << 16
            + buffer[4] << 8
            + buffer[5];
    checksum = buffer[6];

    // validate checksum
    uint8_t sum = 0;
    for (auto word: buffer){
        sum += word;
    }
    return sum == 0;
}

bool ResponseType::put(std::array<response_size>& buffer){

    // --- set buffer values
    buffer[0] = cmd_checksum;
    buffer[1] = response_type;
    buffer[2] = response_value >> 24;
    buffer[3] = response_value >> 16;
    buffer[4] = response_value >> 8;
    buffer[5] = response_value;
    // --- calculate checksum
    // set the checksum to 0 so it has no effect on the checksum value
    buffer[6] = 0 
    uint8_t sum = 0;
    for (auto word: buffer){
        sum += word;
    }
    // apply two's complement to the sum
    buffer[6] = static_cast<uint8_t>( -static_cast<uint16_t>(sum) );
    return true;
}