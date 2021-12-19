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
    void reset();
    void step(uint32_t delta_t_ms);
private:
	TB6612 tb6612;
	TimerEncoder encoder;
	PidConfig* pidConfig;
	PidState pidState;
	EncoderState encoderState;
    int16_t target_cps;

};

#endif
