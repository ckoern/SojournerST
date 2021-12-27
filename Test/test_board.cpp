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

struct target_test_param {
    int32_t set_value;
    ResponseType expect_response;
    //int32_t get_value;
};

struct BoardComTargetCpsTestFixture : BoardComTest, testing::WithParamInterface<target_test_param>{

};

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


TEST_P(BoardComTargetCpsTestFixture, TargetCpsTest)
{
    auto as = GetParam();
    send =CommandPacket{0xcc, Channel, Channel1, channel_set_target_cps, 1234};
    send.command_value = as.set_value;
    send.put(command_data);
    board.ExecuteCommand(command_data, response_data);
    rcv.load(response_data);
    EXPECT_EQ( rcv.response_type, as.expect_response );
    if (as.expect_response == ResponseType::Success){
        send.command_id = channel_get_target_cps;
        send.put(command_data);
        board.ExecuteCommand(command_data, response_data);
        rcv.load(response_data);
        EXPECT_EQ( as.set_value, static_cast<int32_t>(rcv.response_value) );
    }
}

INSTANTIATE_TEST_CASE_P(BoardComTest, BoardComTargetCpsTestFixture,
  testing::Values(
  target_test_param{1234, Success},
  target_test_param{-1234, Success},
  target_test_param{0, Success},
  target_test_param{std::numeric_limits<int16_t>::max() +1, Error_Value},
  target_test_param{std::numeric_limits<int32_t>::max(), Error_Value},
  target_test_param{std::numeric_limits<int16_t>::min() -1, Error_Value},
  target_test_param{std::numeric_limits<int32_t>::min(), Error_Value}
  ));