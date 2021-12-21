#include <gtest/gtest.h>
#include <array>
#include "SojournerST.hpp"
#include "Packet.hpp"
// Demonstrate some basic assertions.
TEST(Board, BasicAssertions) {
    SojournerST board;
    ASSERT_EQ(0,0);

    CommandBuffer command_data;
    ResponseBuffer response_data;


    // write/read cycle for target_cps
    CommandPacket send {0xcc, Channel, Channel1, channel_set_target_cps, 1234};
    ResponsePacket rcv;
    send.put(command_data);
    board.ExecuteCommand(command_data, response_data);
    send.command_id = channel_get_target_cps;
    send.put(command_data);
    board.ExecuteCommand(command_data, response_data);
    rcv.load(response_data);
    EXPECT_EQ( 1234, static_cast<int32_t>(rcv.response_value) );

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