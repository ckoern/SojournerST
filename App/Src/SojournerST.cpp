#include "SojournerST.hpp"

#include "tim.h"
#include "main.h"

SojournerST::SojournerST() : 
    
    // initialize the PID values with some known-good
    // predefined values
    pid_configs(
        {
            PidConfig( 0.0006f, 0.000001f, 0.001f, 1.0f ),
            PidConfig( 0.0006f, 0.000001f, 0.001f, 1.0f )
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
                TimerEncoder(&htim3), 
                &(pid_configs[0])
            ),
            MotorController(
                TB6612( &htim2, TIM_CHANNEL_2,
                    M2_IN1_GPIO_Port,
                    M2_IN1_Pin,
                    M2_IN2_GPIO_Port,
                    M2_IN2_Pin), 
                TimerEncoder(&htim4), 
                &(pid_configs[1])
            )
        }
    )
{

}


void SojournerST::UpdatePidLoop(uint32_t delta_t_ms){
    for( auto& controller: motor_controllers){
        controller.step(delta_t_ms);
    }
}
