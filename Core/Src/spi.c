/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

SPI_HandleTypeDef hspi1;

/* SPI1 init function */
void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspInit 0 */

  /* USER CODE END SPI1_MspInit 0 */
    /* SPI1 clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration
    PB4     ------> SPI1_MISO
    PB3     ------> SPI1_SCK
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI1_MspInit 1 */

  /* USER CODE END SPI1_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspDeInit 0 */

  /* USER CODE END SPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();

    /**SPI1 GPIO Configuration
    PB4     ------> SPI1_MISO
    PB3     ------> SPI1_SCK
    PA7     ------> SPI1_MOSI
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_4|GPIO_PIN_3);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_7);

  /* USER CODE BEGIN SPI1_MspDeInit 1 */

  /* USER CODE END SPI1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void bmi088_write_byte(uint8_t tx_data) {
  HAL_SPI_Transmit(&hspi1, &tx_data, 1, 1000);
  while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_TX);
}
void bmi088_read_byte(uint8_t *rx_data, uint8_t length)
{
  HAL_SPI_Receive(&hspi1, rx_data, length, 1000);
  while (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_RX);
}
void bmi088_write_reg(uint8_t reg, uint8_t data)
{
  bmi088_write_byte(reg& 0x7F);
  bmi088_write_byte(data);
}

void BMI088_ACCEL_NS_L(void) {
  HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_RESET);
}
void BMI088_ACCEL_NS_H(void)
{
  HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_SET);
}
void BMI088_GYRO_NS_L(void)
{
  HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_RESET);;
}
void BMI088_GYRO_NS_H(void)
{
  HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_SET);;
}

// 参考: acc写入，相当于加上片选的 bmi088_write_reg 函数
void bmi088_accel_write_single_reg(uint8_t reg, uint8_t data) {
  BMI088_GYRO_NS_H();
  BMI088_ACCEL_NS_L();

  bmi088_write_byte(reg & 0x7F);
  bmi088_write_byte(data);

  BMI088_ACCEL_NS_H();
}

// 尝试完成 ↓
void bmi088_accel_read_reg(uint8_t reg, uint8_t *rx_data, uint8_t length)
{
  BMI088_GYRO_NS_H();
  BMI088_ACCEL_NS_L();

  bmi088_write_byte(reg | 0x80);
  bmi088_read_byte(rx_data, 1);//dummy byte
  bmi088_read_byte(rx_data, length);

  BMI088_ACCEL_NS_H();
}

void bmi088_gyro_read_reg(uint8_t reg, uint8_t *rx_data, uint8_t length)
{
  BMI088_ACCEL_NS_H();
  BMI088_GYRO_NS_L();

  bmi088_write_byte(reg | 0x80);
  bmi088_read_byte(rx_data, length);

  BMI088_GYRO_NS_H();
}
void bmi088_gyro_write_single_reg(uint8_t reg, uint8_t data)
{
  BMI088_ACCEL_NS_H();
  BMI088_GYRO_NS_L();

  bmi088_write_byte(reg & 0x7F);
  bmi088_write_byte(data);

  BMI088_GYRO_NS_H();
}
void bmi088_init(void) {
  // Soft Reset ACCEL
  BMI088_ACCEL_NS_L();
  bmi088_write_reg(0x7E, 0xB6); // Write 0xB6 to ACC_SOFTRESET(0x7E)
  HAL_Delay(1);
  BMI088_ACCEL_NS_H();

  // Soft Reset GYRO
  BMI088_GYRO_NS_L();
  bmi088_write_reg(0x14, 0xB6); // Write 0xB6 to GYRO_SOFTRESET(0x14)
  HAL_Delay(30);
  BMI088_GYRO_NS_H();

  // Switch ACCEL to Normal Mode
  BMI088_ACCEL_NS_L();
  HAL_Delay(1);
  bmi088_write_reg(0x7D, 0x04); // Write 0x04 to ACC_PWR_CTRL(0x7D)
  HAL_Delay(1);
  BMI088_ACCEL_NS_H();
}

extern float Accel_X_in_mg, Accel_Y_in_mg, Accel_Z_in_mg, Gyro_X_in_dps, Gyro_Y_in_dps, Gyro_Z_in_dps;
extern float kAccel, kGyro;
extern uint8_t rx_acc_data[6], rx_gyro_data[6];

void acc_calculate(){
  // 1. 设置/读取acc0x41寄存器中的量程range参数，并换算为量程系数
  uint8_t raw_range;
  bmi088_accel_read_reg(0x41, &raw_range, 1);
  kAccel = (float)((1 << (raw_range + 1)) * 1.5 * 1000 );

  // 2. 读取acc0x12寄存器中的6位acc数据
  bmi088_accel_read_reg(0x12, rx_acc_data, 6);


  // 3. 用量程系数将原始数据转换为常用单位
  int16_t acc_x_raw = (int16_t)(rx_acc_data[1] << 8 | rx_acc_data[0]);
  int16_t acc_y_raw = (int16_t)(rx_acc_data[3] << 8 | rx_acc_data[2]);
  int16_t acc_z_raw = (int16_t)(rx_acc_data[5] << 8 | rx_acc_data[4]);

  Accel_X_in_mg = (float)acc_x_raw * kAccel / 32768;
  Accel_Y_in_mg = (float)acc_y_raw * kAccel / 32768;
  Accel_Z_in_mg = (float)acc_z_raw * kAccel / 32768;
}

void gyro_calculate(){
  // 1. 设置/读取gyro0x0F寄存器中的量程range参数，并换算为量程系数
  uint8_t raw_range;
  bmi088_gyro_read_reg(0x0F, &raw_range, 1);
  kGyro = (float)(125*(1<<(4-raw_range)));
  // 2. 读取gyro0x02寄存器中的6位gyro数据
  bmi088_gyro_read_reg(0x02, rx_gyro_data, 6);
  // 3. 用量程系数将原始数据转换为常用单位
  int16_t gyro_x_raw = (int16_t)(rx_gyro_data[1] << 8 | rx_gyro_data[0]);
  int16_t gyro_y_raw = (int16_t)(rx_gyro_data[3] << 8 | rx_gyro_data[2]);
  int16_t gyro_z_raw = (int16_t)(rx_gyro_data[5] << 8 | rx_gyro_data[4]);

  Gyro_X_in_dps = (float)gyro_x_raw * kGyro / 32768;
  Gyro_Y_in_dps = (float)gyro_y_raw * kGyro / 32768;
  Gyro_Z_in_dps = (float)gyro_z_raw * kGyro / 32768;

}
/* USER CODE END 1 */
