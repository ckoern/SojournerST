#include "TB6612.hpp"

TB6612::TB6612(
    TIM_HandleTypeDef* pwm_timer, uint8_t pwm_timer_channel,
    GPIO_TypeDef* in1_gpio, uint16_t in1_gpio_pin, 
    GPIO_TypeDef* in2_gpio, uint16_t in2_gpio_pin
) : pwm_timer(pwm_timer), pwm_timer_channel(pwm_timer_channel),
    in1_gpio(in1_gpio), in1_gpio_pin(in1_gpio_pin),
    in2_gpio(in2_gpio), in2_gpio_pin(in2_gpio_pin)
{
	
}

void TB6612::setup(){
    HAL_TIM_PWM_Start(pwm_timer, pwm_timer_channel);
    SetPWM(0);
}
void TB6612::SetPWM(uint8_t pwm_duty){
	__HAL_TIM_SET_COMPARE(pwm_timer, pwm_timer_channel, pwm_duty);
}
void TB6612::SetMode( MotorMode mode ){
    switch (mode){
    case (CCW):
        HAL_GPIO_WritePin(in1_gpio, in1_gpio_pin,  GPIO_PIN_RESET);
        HAL_GPIO_WritePin(in2_gpio, in2_gpio_pin,  GPIO_PIN_SET);
        break;

    case (CW):
        HAL_GPIO_WritePin(in1_gpio, in1_gpio_pin,  GPIO_PIN_SET);
        HAL_GPIO_WritePin(in2_gpio, in2_gpio_pin,  GPIO_PIN_RESET);
        break;

    case (BREAK):
        HAL_GPIO_WritePin(in1_gpio, in1_gpio_pin,  GPIO_PIN_SET);
        HAL_GPIO_WritePin(in2_gpio, in2_gpio_pin,  GPIO_PIN_SET);
        break;

    case (STOP):
        HAL_GPIO_WritePin(in1_gpio, in1_gpio_pin,  GPIO_PIN_RESET);
        HAL_GPIO_WritePin(in2_gpio, in2_gpio_pin,  GPIO_PIN_RESET);
        break;

	}
}