#ifndef INC_SOJOURNERST_H_
#define INC_SOJOURNERST_H_
#include <array>
#include "SojournerSTConfig.hpp"
#include "MotorController.hpp"
class SojournerST{
public:
    SojournerST();
    void UpdatePidLoop(uint32_t delta_t_ms);
private:
    // size will not change during runtime,
    // vectors will be initialized during SojournerST 
    // initializer list
    std::array<PidConfig, 2> pid_configs;
    std::array<MotorController, 2> motor_controllers;
};

#endif
