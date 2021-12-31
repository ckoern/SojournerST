#ifndef INC_TB6612_H_
#define INC_TB6612_H_
#include "stm32g4xx_hal.h"
#include <stdint.h>

enum MotorMode{
    CCW=0,
    CW,
    BREAK,
    STOP
};
class TB6612{
public:
    TB6612(
        TIM_HandleTypeDef* pwm_timer, uint8_t pwm_timer_channel,
        GPIO_TypeDef* in1_gpio, uint16_t in1_gpio_pin, 
        GPIO_TypeDef* in2_gpio, uint16_t in2_gpio_pin
    );
    void setup();
    void SetPWM(uint8_t pwm_duty);
    void SetMode( MotorMode mode );

private:
    TIM_HandleTypeDef* pwm_timer;
	uint8_t			  pwm_timer_channel;
	GPIO_TypeDef*	  in1_gpio;
	uint16_t		  in1_gpio_pin;
	GPIO_TypeDef*	  in2_gpio;
	uint16_t		  in2_gpio_pin;

};
#endif /* INC_TB6612_H_ */