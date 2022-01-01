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


#include "MotorController.hpp"
#include <math.h>
MotorController::MotorController(
    TB6612 tb6612,
    TimerEncoder encoder,
    PidConfig* pidConfig
) : tb6612(tb6612), encoder(encoder),
    pidConfig(pidConfig), 
    pidState({
        .integrator_state = 0,
        .filter_state = 0,
        .integrator_reset = 2,
        .filter_reset = 2
    }),
    encoderState({
        .last_count = 0,
        .enc_cps = 0
    }),
    target_cps(0)
{
    
}

void MotorController::setup(){
	tb6612.setup();
	encoder.setup();
}

void MotorController::reset(){
    pidState = {
        .integrator_state = 0,
        .filter_state = 0,
        .integrator_reset = 2,
        .filter_reset = 2
    };
}

void MotorController::step(uint32_t delta_t_ms){
    // read encoder
	uint32_t current_count =encoder.GetCount();
	uint32_t last_count =  encoderState.last_count;
	int32_t enc_difference = current_count - last_count;

	// catch upwards rollover
	if ( current_count < 500 && last_count > 60000 ){
		enc_difference += 1<<16;
	}

	// catch downwards rollover
	if ( last_count < 500 && current_count > 60000 ){
		enc_difference -= 1<<16;
	}

	// update the state struct
	encoderState.last_count = current_count;
	encoderState.enc_cps = 1000.0f*enc_difference/delta_t_ms;



	// apply PID
	float filter_prod;
	float sum;
	float setpoint_error;


	setpoint_error = (float)target_cps - encoderState.enc_cps;
	pidState.setpoint_error = setpoint_error;
	filter_prod = (setpoint_error * pidConfig->pid_kd -pidState.filter_state) * pidConfig->pid_kn;
	sum = (setpoint_error*pidConfig->pid_kp + pidState.integrator_state) + filter_prod;

	// saturate check
	if (sum > 1){
		pidState.gain = 1;
	}else if (sum < -1){
		pidState.gain = -1;
	}else{
		pidState.gain = sum;
	}

	pidState.direction = (sum>0)? MotorMode::CCW : MotorMode::CW;
	// update motor output
    tb6612.SetMode(pidState.direction);
	tb6612.SetPWM((uint8_t)(255.0f*fabs(pidState.gain)));
	

	// update PID state
	pidState.integrator_state += ( (pidState.gain-sum) + setpoint_error*pidConfig->pid_ki ) * delta_t_ms* 0.001f;
	pidState.integrator_reset = 0;
	pidState.filter_state += delta_t_ms*0.001f*filter_prod;
	pidState.filter_reset = 0;
}


void MotorController::ResetPid(){
	pidState.integrator_state = 0;
	pidState.filter_state = 0;
}