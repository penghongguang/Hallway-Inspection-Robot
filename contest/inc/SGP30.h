#ifndef __SGP30_H
#define __SGP30_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "embARC.h"
#include "embARC_debug.h"
#include "board_config.h"
#include "arc_timer.h"
#include "hx_drv_spi_s.h"
#include "spi_slave_protocol.h"
#include "hardware_config.h"
#include "hx_drv_iomux.h"
#include "hx_drv_uart.h"
#include "SC16IS750_Bluepacket.h"

#define SGP30_read  0xb1  //SGP30的读地址
#define SGP30_write 0xb0  //SGP30的写地址

#define ACK         0             //应答信号
#define NACK        1             //非应答信号

#define SCL  GPIO6 //SGP30的SCL引脚定义
#define SDA  GPIO7  //SGP30的SDA引脚定义

#define SCL_1 GPIOSetPinState(SC16IS750_PROTOCOL_SPI, CH_A, SCL, 1)
#define SCL_0 GPIOSetPinState(SC16IS750_PROTOCOL_SPI, CH_A, SCL, 0)
#define SDA_1 GPIOSetPinState(SC16IS750_PROTOCOL_SPI, CH_A, SDA, 1)
#define SDA_0 GPIOSetPinState(SC16IS750_PROTOCOL_SPI, CH_A, SDA, 0)

#define digitalRead(x) GPIOGetPinState(SC16IS750_PROTOCOL_SPI, CH_A, x)

#define SCL_I GPIOSetPinMode(SC16IS750_PROTOCOL_SPI, CH_A, SCL, INPUT)
#define SCL_O GPIOSetPinMode(SC16IS750_PROTOCOL_SPI, CH_A, SCL, OUTPUT)
#define SDA_I GPIOSetPinMode(SC16IS750_PROTOCOL_SPI, CH_A, SDA, INPUT)
#define SDA_O GPIOSetPinMode(SC16IS750_PROTOCOL_SPI, CH_A, SDA, OUTPUT)

typedef unsigned char u8;

//I2C起始信号
void I2CStart(void);

//I2C停止信号
void I2CStop(void);

//I2C写一个字节数据，返回ACK或者NACK
u8 I2C_Write_Byte(u8 Write_Byte);

//I2C读一个字节数据，入口参数用于控制应答状态，ACK或者NACK
u8 I2C_Read_Byte(u8 AckValue);

//初始化SGP30
void SGP30_Init(void);

//向SGP30写数据
void SGP30_Write(u8 a, u8 b);

//从SGP30读数据
unsigned long SGP30_Read(void);


#endif
