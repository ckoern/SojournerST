#include <gtest/gtest.h>
#include <array>
#include "Packet.hpp"
// Demonstrate some basic assertions.
TEST(Packet, BasicAssertions) {
    // Expect two strings not to be equal.

    std::array<uint8_t, 7> data = {0x00,0x00,0x00,0x00,0x00,0x00,0};
    uint8_t checksum = communication::calculate_checksum(&(data[0]), 6);
    uint8_t wordsum =  communication::calculate_sum(&(data[0]), 6);
    data[6] = checksum;
    EXPECT_EQ(static_cast<uint8_t>(checksum+wordsum), 0);




    CommandPacket send {0xcc, Global, Channel2, channel_pid_get_kd, 123953};
    send.put(data);
    CommandPacket rcv;
    rcv.load(data);
    EXPECT_EQ(send, rcv);


    
    ResponsePacket send_resp {0xcc, Error_Checksum , 123953};
    send_resp.put(data);
    ResponsePacket rcv_resp;
    rcv_resp.load(data);
    EXPECT_EQ(send_resp, rcv_resp);
}