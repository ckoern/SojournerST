#include "SojournerST.hpp"

#include <cstring>
#include <limits>
#include "tim.h"
#include "main.h"
#include "Packet.hpp"

SojournerST::SojournerST() : 
    
    // initialize the PID values with some known-good
    // predefined values
    pid_configs(
        {
            PidConfig( 0.0004f, 0.00001f, 0.0001f, 1.0f ),
            PidConfig( 0.0004f, 0.00001f, 0.0001f, 1.0f )
        }
    ),
    // initialize the motor controllers
    motor_controllers(
        {
            MotorController(
                TB6612( &htim2, TIM_CHANNEL_1,
                    M1_IN1_GPIO_Port,
                    M1_IN1_Pin,
                    M1_IN2_GPIO_Port,
                    M1_IN2_Pin), 
                TimerEncoder(&htim4),
                &(pid_configs[0])
            ),
            MotorController(
                TB6612( &htim2, TIM_CHANNEL_4,
                    M2_IN1_GPIO_Port,
                    M2_IN1_Pin,
                    M2_IN2_GPIO_Port,
                    M2_IN2_Pin), 
                TimerEncoder(&htim3),
                &(pid_configs[1])
            )
        }
    )
{

}

void SojournerST::setup(){
    for (auto& controller: motor_controllers){
        controller.setup();
    }
}

void SojournerST::UpdatePidLoop(uint32_t delta_t_ms){
    for( auto& controller: motor_controllers){
        controller.step(delta_t_ms);
    }
}



bool SojournerST::ExecuteCommand(const CommandBuffer& cmd, ResponseBuffer& resp){

    CommandPacket command_packet;
    ResponsePacket response_packet;
    bool success = command_packet.load(cmd);

    // check if checksum is valid
    if (!success){
        response_packet.response_type = Error_Checksum;
        response_packet.put(resp, true);
        return false;
    }
    
    response_packet.cmd_checksum = command_packet.checksum;

    // check if packet starts with sync byte
    if (command_packet.sync != command_sync_byte){
        response_packet.response_type = Error_Sync;
        response_packet.put(resp, true);
        return false;
    }

    // general checks were successfull, now forward them
    // depending on global or channel command for processing
    if (command_packet.scope == Global ){
        response_packet.response_type = ExecuteGlobalCommand( command_packet, response_packet);
    }
    else{
        response_packet.response_type = ExecuteChannelCommand( command_packet, response_packet);
    }

    // encode the response packet and store it into the com buffer
    response_packet.put(resp, true);
    return response_packet.response_value == Success;
}


ResponseType SojournerST::ExecuteGlobalCommand(const CommandPacket& cmd, ResponsePacket& resp){
    return Error_Undefined;
}


ResponseType SojournerST::ExecuteChannelCommand(const CommandPacket& cmd, ResponsePacket& resp){
    //size_t channel = cmd.bank;
    auto& motor_controller = motor_controllers[cmd.bank];
    auto& pid_config = pid_configs[cmd.bank];
    switch (cmd.command_id){
        // ----- read only commands
        case channel_current_cps:
            resp.setValueFromFloat(motor_controller.GetEncoderState().enc_cps);
            break;

        case channel_pid_integrator_state:
            resp.setValueFromFloat(motor_controller.GetPidState().integrator_state);
            break;

        case channel_pid_filter_state:
            resp.setValueFromFloat(motor_controller.GetPidState().filter_state);
            break;

        case channel_pid_gain:
            resp.setValueFromFloat(motor_controller.GetPidState().gain);
            break;

        case channel_pid_setpoint_error:
            resp.setValueFromFloat(motor_controller.GetPidState().setpoint_error);
            break;

        // ----- write only commands
        case channel_stop:
            motor_controller.SetTargetCps(0);
            break;


        case channel_pid_reset :
            motor_controller.ResetPid();
            break;


        // ----- read/write commands
        case channel_get_target_cps:
        {
            int32_t target = motor_controller.GetTargetCps();
            resp.response_value = static_cast<uint32_t>(target);
            break;
        }
        case channel_set_target_cps:
        {
            int32_t target = static_cast<int32_t>( cmd.command_value );
            if ( (target < std::numeric_limits<int16_t>::min() )
               || (target > std::numeric_limits<int16_t>::max() ))
            {
                resp.response_value = cmd.command_value;
                return Error_Value;
            }
            else{
                motor_controller.SetTargetCps(target);
            }
            break;
        }

        case channel_pid_get_kp:
            resp.setValueFromFloat(pid_config.pid_kp);
            break;            
        
        case channel_pid_set_kp:
            pid_config.pid_kp = communication::extractValueAsFloat(cmd.command_value);
            break;    


        
        case channel_pid_get_ki:
            resp.setValueFromFloat(pid_config.pid_ki);
            break;            
        
        case channel_pid_set_ki:
            pid_config.pid_ki = communication::extractValueAsFloat(cmd.command_value);
            break;

        
        
        case channel_pid_get_kd:
            resp.setValueFromFloat(pid_config.pid_kd);
            break;            
        
        case channel_pid_set_kd:
            pid_config.pid_kd = communication::extractValueAsFloat(cmd.command_value);
            break;  

        
        case channel_pid_get_kn:
            resp.setValueFromFloat(pid_config.pid_kn);
            break;            
        
        case channel_pid_set_kn:
            pid_config.pid_kn = communication::extractValueAsFloat(cmd.command_value);
            break;  

        default:
            return Error_UnknownCommand;
    }
    return Success;
}
