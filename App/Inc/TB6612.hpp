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