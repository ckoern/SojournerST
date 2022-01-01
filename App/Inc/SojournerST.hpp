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
