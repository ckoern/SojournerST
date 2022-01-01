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


#ifndef STUBS_INC_STM32G4XX_HAL_H
#define STUBS_INC_STM32G4XX_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
typedef struct{
    uint16_t CNT;
} TIM_TypeDef;

typedef struct{
    TIM_TypeDef* Instance;
} TIM_HandleTypeDef;



typedef struct{

}SPI_TypeDef;

typedef struct{
    SPI_TypeDef* Instance;
} SPI_HandleTypeDef;


extern TIM_TypeDef tim1;
extern TIM_TypeDef tim2;
extern TIM_TypeDef tim3;
extern TIM_TypeDef tim4;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

typedef struct{

} GPIO_TypeDef;

extern GPIO_TypeDef gpio_a;
extern GPIO_TypeDef gpio_b;

#define HAL_TIM_PWM_Start(_x_, _y_) ("")
#define HAL_GPIO_WritePin(_x_, _y_, _z_) ("")
#define __HAL_TIM_SET_COMPARE(_x_, _y_, _z_) ("")
#define HAL_TIM_Encoder_Start(_x_, _y_) ("")



#define GPIO_PIN_0                 ((uint16_t)0x0001)  /* Pin 0 selected    */
#define GPIO_PIN_1                 ((uint16_t)0x0002)  /* Pin 1 selected    */
#define GPIO_PIN_2                 ((uint16_t)0x0004)  /* Pin 2 selected    */
#define GPIO_PIN_3                 ((uint16_t)0x0008)  /* Pin 3 selected    */
#define GPIO_PIN_4                 ((uint16_t)0x0010)  /* Pin 4 selected    */
#define GPIO_PIN_5                 ((uint16_t)0x0020)  /* Pin 5 selected    */
#define GPIO_PIN_6                 ((uint16_t)0x0040)  /* Pin 6 selected    */
#define GPIO_PIN_7                 ((uint16_t)0x0080)  /* Pin 7 selected    */
#define GPIO_PIN_8                 ((uint16_t)0x0100)  /* Pin 8 selected    */
#define GPIO_PIN_9                 ((uint16_t)0x0200)  /* Pin 9 selected    */
#define GPIO_PIN_10                ((uint16_t)0x0400)  /* Pin 10 selected   */
#define GPIO_PIN_11                ((uint16_t)0x0800)  /* Pin 11 selected   */
#define GPIO_PIN_12                ((uint16_t)0x1000)  /* Pin 12 selected   */
#define GPIO_PIN_13                ((uint16_t)0x2000)  /* Pin 13 selected   */
#define GPIO_PIN_14                ((uint16_t)0x4000)  /* Pin 14 selected   */
#define GPIO_PIN_15                ((uint16_t)0x8000)  /* Pin 15 selected   */
#define GPIO_PIN_All               ((uint16_t)0xFFFF)  /* All pins selected */

#define GPIO_PIN_MASK              (0x0000FFFFU) /* PIN mask for assert test */


#define GPIOA ( (GPIO_TypeDef*)(&gpio_a) )
#define GPIOB ( (GPIO_TypeDef*)(&gpio_b) )


#ifdef __cplusplus
}
#endif

#endif