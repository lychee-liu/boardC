//
// Created by lychee on 2025/10/26.
//
#include "tim.h"
#include "usart.h"
#include "RC.h"

extern RC remote;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    if (htim == &htim6) {
        remote.handle();
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart==&huart3) {
        remote.uartRxCallback(Size);
    }
}