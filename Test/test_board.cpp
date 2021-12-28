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
};


struct commandcyle_test_param{
    CommandPacket write_cyle_cmd;
    ResponsePacket write_cycle_resp_expect;
    CommandPacket read_cyle_cmd;
    ResponsePacket read_cycle_resp_expect;
};
struct BoardComTargetCpsTestFixture : BoardComTest, testing::WithParamInterface<target_test_param>{

};

TEST_F(BoardComTest, UseBadChecksum) {
    // write/read cycle for target_cps
    send =CommandPacket{ Channel, Channel1, channel_set_target_cps, 1234};
    //modify checksum
    send.checksum = send.checksum - 1;
    send.put(command_data, false);
    board.ExecuteCommand(command_data, response_data);
    rcv.load(response_data);
    EXPECT_EQ( ResponseType::Error_Checksum,rcv.response_type );
}

TEST_F(BoardComTest, UseBadSync) {
    // write/read cycle for target_cps
    send =CommandPacket{ Channel, Channel1, channel_set_target_cps, 1234};
    // use bad sync byte
    send.sync = 0xca;
    // recalculate checksum
    send.fill_checksum();
    send.put(command_data, true);
    board.ExecuteCommand(command_data, response_data);
    rcv.load(response_data);
    EXPECT_EQ( ResponseType::Error_Sync,rcv.response_type );
}

TEST_P(BoardComTargetCpsTestFixture, TargetCpsTest)
{
    auto as = GetParam();
    send =CommandPacket{ Channel, Channel1, channel_set_target_cps, 1234};
    send.command_value = as.set_value;
    send.put(command_data, true);
    board.ExecuteCommand(command_data, response_data);
    rcv.load(response_data);
    EXPECT_EQ( rcv.response_type, as.expect_response );
    if (as.expect_response == ResponseType::Success){
        send.command_id = channel_get_target_cps;
        send.put(command_data, true);
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