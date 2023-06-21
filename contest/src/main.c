#include "main.h"
#include "tflitemicro_algo.h"
#include "model_settings.h"
#include "synopsys_sdk_camera_drv.h"
#include "SC16IS750_Bluepacket.h"

#include "SGP30.h"
#include "dht11.h"

#define imgW 640
#define imgH 480 

DEV_UART * uart0_ptr;
DEV_UART * uart1_ptr;
char uart_buf[uart_buf_size] = {0};

extern uint32_t g_wdma2_baseaddr; //图片地址

int8_t test_img [kNumRows * kNumCols] = {0};
uint8_t output_img [kNumRows * kNumCols] = {0};
uint8_t output_height = kNumRows;
uint8_t output_width = kNumCols;

DHT11 dht11; //DHT11数据
unsigned int CO2Data,TVOCData;//定义CO2浓度变量与TVOC浓度变量
unsigned long sgp30_dat;
int32_t score; //检测分数

void sys_init()
{
    // SPI->IO
    HX_GPIOSetup();
    IRQSetup();
    UartInit(SC16IS750_PROTOCOL_SPI);

    uart0_ptr = hx_drv_uart_get_dev(USE_SS_UART_0);

    synopsys_camera_init();
    tflitemicro_algo_init();

    SGP30_Init();
    SGP30_Write(0x20,0x08);
    sgp30_dat = SGP30_Read();//读取SGP30的值
    CO2Data = (sgp30_dat & 0xffff0000) >> 16;
    TVOCData = sgp30_dat & 0x0000ffff; 
    while(CO2Data == 400 && TVOCData == 0) //等待跳过初始化
    {
        SGP30_Write(0x20,0x08);
        sgp30_dat = SGP30_Read();//读取SGP30的值
        CO2Data = (sgp30_dat & 0xffff0000) >> 16;//取出CO2浓度值
        TVOCData = sgp30_dat & 0x0000ffff;       //取出TVOC值
    }
    sprintf(uart_buf, "Init success!!!\r\n");   
    uart0_ptr->uart_write(uart_buf, strlen(uart_buf));
    board_delay_ms(10);
}

void sendData()
{
    char ack;
    char verify = score + CO2Data + TVOCData + (int) (dht11.t*100) + (int) (dht11.h*100); //校验码

    sprintf(uart_buf, "%d,%d,%d,%d,%d,%d\r\n", score, CO2Data, TVOCData,(int) (dht11.t*100), (int) (dht11.h*100), verify);   
    uart0_ptr->uart_write(uart_buf, strlen(uart_buf));
    // uart0_ptr->uart_read(&ack, 1); //等待应答信号
}

int main(void)
{
    uint8_t * img_ptr;
    char gpio_value;

    sys_init();
    D_O;
    while(1)
    {
        synopsys_camera_start_capture(); //采集图片
        board_delay_ms(50); //延时等待采集完成
        img_ptr = (uint8_t *) g_wdma2_baseaddr;
        synopsys_camera_down_scaling(img_ptr, imgW, imgH, &output_img[0], output_width, output_height);
        for(int32_t pixel_index = 0; pixel_index < kImageSize; pixel_index ++)
            test_img[pixel_index] = output_img[pixel_index] - 128;
        score = tflitemicro_algo_run(&test_img[0]); //检测

        SGP30_Write(0x20,0x08);
        sgp30_dat = SGP30_Read();//读取SGP30的值
        CO2Data = (sgp30_dat & 0xffff0000) >> 16;//取出CO2浓度值
        TVOCData = sgp30_dat & 0x0000ffff;       //取出TVOC值

        dht11 = dht11_read();

        sendData();//发送数据
    }

	return 0;
}

