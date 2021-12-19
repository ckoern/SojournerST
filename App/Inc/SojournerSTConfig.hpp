#ifndef INC_SOJOURNERSTCONFIG_H_
#define INC_SOJOURNERSTCONFIG_H_
#include <stdint.h>
constexpr uint8_t SOJ_N_MOTORS = 2;
constexpr uint32_t PID_UPDATE_RATE_MS = 50;
constexpr size_t command_size = 7;
constexpr size_t response_size = command_size;
#endif