//
// Created by lychee on 2025/10/26.
//

#ifndef INC_1025REMOTECTRL_RC_H
#define INC_1025REMOTECTRL_RC_H
#include "stm32f4xx_hal.h"


class RemoteControl {
private:
    uint32_t tick;
    bool status = true;
    uint8_t rx_buf[32]={0};
    uint8_t rx_data[32]={0};
    float ch0 = 0.0f;
    float ch1 = 0.0f;
    float ch2 = 0.0f;
    float ch3 = 0.0f;
    int16_t ch0_ori = 0;
    int16_t ch1_ori = 0;
    int16_t ch2_ori = 0;
    int16_t ch3_ori = 0;
    enum SwitchPos {
        RESET,
        UP,
        DOWN,
        MID
    };
    SwitchPos s1=RESET,s2=RESET;
public:

    RemoteControl();
    void init();
    bool updateStatus();
    bool checkStatus();
    void handle();
    void uartRxCallback(uint16_t Size);

};


#endif //INC_1025REMOTECTRL_RC_H