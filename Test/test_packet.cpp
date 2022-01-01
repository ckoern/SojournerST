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

#include <gtest/gtest.h>
#include <array>
#include "Packet.hpp"
// Demonstrate some basic assertions.
TEST(Packet, BasicAssertions) {
    // Expect two strings not to be equal.

    std::array<uint8_t, 7> data = {0x00,0x00,0x00,0x00,0x00,0x00,0};
    uint8_t checksum = communication::calculate_checksum(&(data[0]), 6);
    uint8_t wordsum =  communication::calculate_bytesum(&(data[0]), 6);
    data[6] = checksum;
    EXPECT_EQ(static_cast<uint8_t>(checksum+wordsum), 0);




    CommandPacket send { Global, Channel2, channel_pid_get_kd, 123953};
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

TEST(Packet, ResponseChecksum){
    ResponseBuffer data;
    ResponsePacket send_resp {0xcc, Error_Checksum , 123953};
    send_resp.put(data);
    uint8_t checksum = communication::calculate_checksum(&(data[0]), 6);
    uint8_t wordsum =  communication::calculate_bytesum(&(data[0]), 6);
    EXPECT_EQ(static_cast<uint8_t>(checksum+wordsum), 0);
}