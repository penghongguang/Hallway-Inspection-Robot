#ifndef __DHT11_H
#define __DHT11_H

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

#define DHT11_PIN GPIO5

#define D_1 GPIOSetPinState(SC16IS750_PROTOCOL_SPI, CH_A, DHT11_PIN, 1)
#define D_0 GPIOSetPinState(SC16IS750_PROTOCOL_SPI, CH_A, DHT11_PIN, 0)

#define digitalRead(x) GPIOGetPinState(SC16IS750_PROTOCOL_SPI, CH_A, x)

#define D_I GPIOSetPinMode(SC16IS750_PROTOCOL_SPI, CH_A, DHT11_PIN, INPUT)
#define D_O GPIOSetPinMode(SC16IS750_PROTOCOL_SPI, CH_A, DHT11_PIN, OUTPUT)

typedef struct{
  float t; //温度
  float h; //湿度
  bool status;
} DHT11;

DHT11 dht11_read();

#endif
