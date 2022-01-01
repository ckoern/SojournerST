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

#ifndef INC_MOTORCONTROLLER_H_
#define INC_MOTORCONTROLLER_H_

#include <stdint.h>
#include "TB6612.hpp"
#include "TimerEncoder.hpp"

struct PidState{
	float integrator_state;
	float filter_state;
	int8_t integrator_reset;
	int8_t filter_reset;
	float gain;
	float setpoint_error;
	MotorMode direction;
};

struct EncoderState{
	uint32_t last_count;
	float enc_cps;
};
struct PidConfig{
	float pid_kp;
	float pid_ki;
	float pid_kd;
	float pid_kn;

    PidConfig(float pid_kp, float pid_ki, float pid_kd, float pid_kn) :
       pid_kp(pid_kp), pid_ki(pid_ki), pid_kd(pid_kd), pid_kn(pid_kn) {}

    PidConfig(float pid_kp, float pid_ki, float pid_kd) : PidConfig(pid_kp, pid_ki, pid_kd, 1.0f){};
    PidConfig() : PidConfig(0,0,0){}
};

class MotorController {
public: 
    MotorController(
        TB6612 tb6612,
        TimerEncoder encoder,
        PidConfig* pidConfig
    );
	void setup();
    void reset();
    void step(uint32_t delta_t_ms);
	const EncoderState& GetEncoderState() const { return encoderState;};
	const PidState& GetPidState() const { return pidState;};
	const PidConfig* GetPidConfig() const { return pidConfig;};
	PidConfig* GetPidConfig() { return pidConfig;};
	void SetTargetCps( int16_t value ) {target_cps = value;}
	int16_t GetTargetCps() const {return  target_cps;}
	void ResetPid();
private:
	TB6612 tb6612;
	TimerEncoder encoder;
	PidConfig* pidConfig;
	PidState pidState;
	EncoderState encoderState;
    int16_t target_cps;

};

#endif
