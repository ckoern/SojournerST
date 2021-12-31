#ifndef INC_TIMERENCODER_H_
#define INC_TIMERENCODER_H_
#include <stdint.h>
class TimerEncoder{
public:
    TimerEncoder(TIM_HandleTypeDef* timer_handle) 
        : timer(timer_handle)
    {
        
    }
    int32_t GetCount() const {return timer->Instance->CNT;};
    void setup(){
        HAL_TIM_Encoder_Start(timer, TIM_CHANNEL_ALL);
    };
private:
    TIM_HandleTypeDef* timer;
};
#endif