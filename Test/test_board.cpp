#include <gtest/gtest.h>
#include <array>
#include "SojournerST.hpp"
#include "Packet.hpp"

struct BoardComTest : testing::Test
{
    SojournerST board;
    CommandBuffer command_data;
    ResponseBuffer response_data;
    CommandPacket send;
    ResponsePacket rcv;
    BoardComTest()
    {}
    virtual ~BoardComTest()
    {}
};

// Demonstrate some basic assertions.
TEST_F(BoardComTest, SetTargetCps1) {

    // write/read cycle for target_cps
    send = CommandPacket{0xcc, Channel, Channel1, channel_set_target_cps, 1234};
    send.put(command_data);
    board.ExecuteCommand(command_data, response_data);
    send.command_id = channel_get_target_cps;
    send.put(command_data);
    board.ExecuteCommand(command_data, response_data);
    rcv.load(response_data);
    EXPECT_EQ( 1234, static_cast<int32_t>(rcv.response_value) );

}
TEST_F(BoardComTest, SetTargetCps2) {
    // write/read cycle for target_cps
    send.command_id = channel_set_target_cps;
    send.command_value = static_cast<uint32_t>(-1234);
    send.put(command_data);
    board.ExecuteCommand(command_data, response_data);
    send.command_id = channel_get_target_cps;
    send.put(command_data);
    board.ExecuteCommand(command_data, response_data);
    rcv.load(response_data);
    EXPECT_EQ( -1234, static_cast<int32_t>(rcv.response_value) );
    
}
TEST_F(BoardComTest, UseBadChecksum) {
    // write/read cycle for target_cps
    send =CommandPacket{0xcc, Channel, Channel1, channel_set_target_cps, 1234};
    send.put(command_data);
    //modify checksum
    command_data[command_data.size()-1] = command_data[command_data.size()-1] - 1;
    board.ExecuteCommand(command_data, response_data);
    rcv.load(response_data);
    EXPECT_EQ( ResponseType::Error_Checksum,rcv.response_type );
}