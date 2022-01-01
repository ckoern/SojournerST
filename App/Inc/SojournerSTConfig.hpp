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


#ifndef INC_SOJOURNERSTCONFIG_H_
#define INC_SOJOURNERSTCONFIG_H_
#include <stdint.h>
#include <array>
constexpr uint8_t SOJ_N_MOTORS = 2;
constexpr uint32_t PID_UPDATE_RATE_MS = 50;
constexpr size_t command_size = 7;
constexpr size_t response_size = command_size;
constexpr uint8_t command_sync_byte = 0xcc;
constexpr uint8_t setter_cmd_id_offset = 32;

using CommandBuffer = std::array<uint8_t, command_size>;
using ResponseBuffer = std::array<uint8_t, response_size>;
#endif