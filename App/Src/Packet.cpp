#include "Packet.hpp"

// Set all variable command fields, sync byte and checksum will be filled automatically
CommandPacket::CommandPacket( CommandScopeType scope, CommandBankType bank, 
        CommandType command_id, uint32_t command_value)
    : sync(command_sync_byte),  scope(scope), bank(bank), command_id(command_id),
      command_value(command_value), checksum(0)
{
    fill_checksum();
}

// Initialize all values as given, no valid packet is guarenteed
CommandPacket::CommandPacket( uint8_t sync, CommandScopeType scope, CommandBankType bank, 
        CommandType command_id, uint32_t command_value, uint8_t checksum)
    : sync(sync),  scope(scope), bank(bank), command_id(command_id),
      command_value(command_value), checksum(checksum)
{
}


bool CommandPacket::operator==(const CommandPacket& rhs) const{
    return sync == rhs.sync
       &&  scope == rhs.scope
       && bank == rhs.bank
       && command_id == rhs.command_id
       && command_value == rhs.command_value; // no need to compare checksum
}
bool CommandPacket::load( const CommandBuffer& buffer){
    sync = buffer[0];
    scope = static_cast<CommandScopeType>(buffer[1] >> 7);
    bank = static_cast<CommandBankType>((buffer[1] >> 6) & 0b01);
    command_id = static_cast<CommandType>(buffer[1] & 0x3f); //lower 6 bits
    command_value = (buffer[2] << 24)
                  + (buffer[3] << 16)
                  + (buffer[4] << 8)
                  + (buffer[5]);
    checksum = buffer[6];
    // validate checksum
    uint8_t sum = 0;
    for (auto word: buffer){
        sum += word;
    }
    return sum == 0;
}

bool CommandPacket::put( CommandBuffer& buffer, bool recalculate_checksum){
    buffer[0] = sync;
    buffer[1] = 0;
    buffer[1] |= static_cast<uint8_t>(scope) << 7;
    buffer[1] |= static_cast<uint8_t>(bank) << 6;
    buffer[1] |= static_cast<uint8_t>(command_id);
    
    buffer[2] = command_value >> 24;
    buffer[3] = command_value >> 16;
    buffer[4] = command_value >> 8;
    buffer[5] = command_value;

    if (recalculate_checksum){
        checksum = communication::calculate_checksum(&(buffer[0]), command_size-1);
    }
    buffer[6] = checksum;
    return true;

}

void CommandPacket::fill_checksum(){
    // create a temporary buffer on the stack and use the 
    // fill it with the content and calculate the cecksum from the temp buffer
    CommandBuffer buf;
    put(buf, true);
}

bool ResponsePacket::operator==(const ResponsePacket& rhs) const{
    return cmd_checksum == rhs.cmd_checksum
       &&  response_type == rhs.response_type
       && response_value == rhs.response_value;
}

bool ResponsePacket::load( const std::array<uint8_t, command_size>& buffer){

    cmd_checksum = buffer[0];
    response_type = static_cast<ResponseType>(buffer[1]);
    response_value = (buffer[2] << 24)
                   + (buffer[3] << 16)
                   + (buffer[4] << 8)
                   + (buffer[5]);

    // validate checksum
    uint8_t sum = 0;
    for (auto word: buffer){
        sum += word;
    }
    return sum == 0;
}

bool ResponsePacket::put(std::array<uint8_t, response_size>& buffer){

    // --- set buffer values
    buffer[0] = cmd_checksum;
    buffer[1] = response_type;
    buffer[2] = response_value >> 24;
    buffer[3] = response_value >> 16;
    buffer[4] = response_value >> 8;
    buffer[5] = response_value;
    // --- calculate checksum
    // set the checksum to 0 so it has no effect on the checksum value
    buffer[6] = communication::calculate_checksum(&(buffer[0]), response_size-1);
    return true;
}

