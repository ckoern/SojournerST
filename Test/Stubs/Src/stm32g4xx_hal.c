#include "stm32g4xx_hal.h"

GPIO_TypeDef gpio_a;
GPIO_TypeDef gpio_b;

TIM_TypeDef tim1;
TIM_TypeDef tim2;
TIM_TypeDef tim3;
TIM_TypeDef tim4;
TIM_HandleTypeDef htim1 = {.Instance = &tim1};
TIM_HandleTypeDef htim2 = {.Instance = &tim2};
TIM_HandleTypeDef htim3 = {.Instance = &tim3};
TIM_HandleTypeDef htim4 = {.Instance = &tim4};