#include <gtest/gtest.h>
#include <array>
#include "SojournerST.hpp"
#include "Packet.hpp"


#include "algorithm"
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
struct BoardComTargetCpsTestFixture : BoardComTest, 
          testing::WithParamInterface<target_test_param>
{};

struct BoardComReadWriteCycleTestFixture : BoardComTest, 
          testing::WithParamInterface<commandcyle_test_param>
{};

std::ostream& operator<<(std::ostream &os, const ResponsePacket &resp){
    os <<   "\tCmd Checksum:   0x" << std::hex << std::setfill('0') << std::setw(2) << resp.cmd_checksum;
    os << "\n\tResponse Type:  0x"<< resp.cmd_checksum;
    os << "\n\tResponse Value: 0x"<< resp.response_value;
    return os;
}

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



TEST_P(BoardComReadWriteCycleTestFixture, RWCycleTest){
    auto as = GetParam();
    send = as.write_cyle_cmd;
    send.put(command_data);
    board.ExecuteCommand(command_data, response_data);
    rcv.load(response_data);

    
    // only check response type and cmd_checksum 
    // as value (and therefore checksum) can be undefined during write calls
    EXPECT_EQ( rcv.response_type, as.write_cycle_resp_expect.response_type );
    
    // if we expect a success we also check the read cycle
    if (as.write_cycle_resp_expect.response_type != ResponseType::Success){
        return; 
    }
    send = as.read_cyle_cmd;
    send.put(command_data);
    board.ExecuteCommand(command_data, response_data);
    rcv.load(response_data);

    // for reads, require equality also for the value
    EXPECT_EQ( rcv, as.read_cycle_resp_expect );
    EXPECT_EQ(rcv.checksum, as.read_cycle_resp_expect.checksum);
    //EXPECT_EQ( communication::calculate_bytesum(&(response_data[0]), response_size), 0 );
}


std::vector<commandcyle_test_param> generate_commandcyle_test_param_sets(){
    std::vector<commandcyle_test_param> test_sets;
    std::vector<CommandType> read_commands = {
        channel_pid_get_kp,
        channel_pid_get_ki,
        channel_pid_get_kd,
        channel_pid_get_kn,
        channel_get_target_cps,
    };
    std::vector<uint32_t> set_values = {12345, static_cast<uint32_t>(-12345)};
    for (auto read_cmd_id: read_commands){
        for (auto channel: {Channel1, Channel2}){
            for(auto set_value: set_values){
                CommandPacket write_cmd { Channel, channel, static_cast<CommandType>(read_cmd_id + 32), set_value };
                ResponsePacket write_response { write_cmd.checksum, Success, 0 };
                CommandPacket read_cmd { Channel, channel, static_cast<CommandType>(read_cmd_id), 0 };
                ResponsePacket read_response { read_cmd.checksum, Success, set_value };

                test_sets.push_back( {write_cmd, write_response, read_cmd, read_response} );
            }
        }
    }

    return test_sets;
}


INSTANTIATE_TEST_CASE_P(BoardComTest, BoardComReadWriteCycleTestFixture,
  testing::ValuesIn( generate_commandcyle_test_param_sets() ));
