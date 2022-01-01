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


#ifndef STUBS_INC_TIM_H
#define STUBS_INC_TIM_H

#ifdef __cplusplus
extern "C" {
#endif


#define TIM_CHANNEL_1                      0x00000000U                          /*!< Capture/compare channel 1 identifier      */
#define TIM_CHANNEL_2                      0x00000004U                          /*!< Capture/compare channel 2 identifier      */
#define TIM_CHANNEL_3                      0x00000008U                          /*!< Capture/compare channel 3 identifier      */
#define TIM_CHANNEL_4                      0x0000000CU                          /*!< Capture/compare channel 4 identifier      */
#define TIM_CHANNEL_5                      0x00000010U                          /*!< Compare channel 5 identifier              */
#define TIM_CHANNEL_6                      0x00000014U                          /*!< Compare channel 6 identifier              */
#define TIM_CHANNEL_ALL                    0x0000003CU                          /*!< Global Capture/compare channel identifier  */


#ifdef __cplusplus
}
#endif

#endif