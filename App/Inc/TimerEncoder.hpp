#ifndef INC_TIMERENCODER_H_
#define INC_TIMERENCODER_H_
#include <stdint.h>
class TimerEncoder{
public:
    TimerEncoder(TIM_HandleTypeDef* timer_handle) 
        : timer(timer_handle)
    {
        HAL_TIM_Encoder_Start(timer, TIM_CHANNEL_ALL);
    }
    int32_t GetCount() const {return timer->Instance->CNT;};
private:
    TIM_HandleTypeDef* timer;
};
#endif