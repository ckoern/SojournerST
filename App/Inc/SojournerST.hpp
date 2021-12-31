#ifndef INC_SOJOURNERST_H_
#define INC_SOJOURNERST_H_
#include <array>
#include "SojournerSTConfig.hpp"
#include "MotorController.hpp"
#include "Packet.hpp"
#include "stm32g4xx_hal.h"
class SojournerST{
public:
    SojournerST();
    void setup();
    void UpdatePidLoop(uint32_t delta_t_ms);

    bool ExecuteCommand(const CommandBuffer& cmd, ResponseBuffer& resp);
    
    SPI_HandleTypeDef* spi_handle;
private:
    // size will not change during runtime,
    // vectors will be initialized during SojournerST 
    // initializer list
    std::array<PidConfig, 2> pid_configs;
    std::array<MotorController, 2> motor_controllers;

    ResponseType ExecuteGlobalCommand(const CommandPacket& cmd, ResponsePacket& resp);
    ResponseType ExecuteChannelCommand(const CommandPacket& cmd, ResponsePacket& resp);

};

#endif
