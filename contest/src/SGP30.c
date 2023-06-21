#include "SGP30.h"

void SGP30_IO_Init()
{
  SCL_O;
  SDA_O;
}

//I2C起始信号
void I2CStart(void)
{
  SDA_O;
  SDA_1;                            //发送起始条件的数据信号 
  SCL_1;
  board_delay_us(50);                    //起始条件建立时间大于4.7us,延时 
  SDA_0;                            //发送起始信号
  board_delay_us(50);                    //起始条件锁定时间大于4μs 
  SCL_0;                            //钳住I2C总线，准备发送或接收数据 
  board_delay_us(50);
}

//I2C停止信号

void I2CStop(void)
{
  SDA_O;
  SDA_0;                        //发送结束条件的数据信号 
  SCL_0;
  board_delay_us(50);
  SCL_1;                        //发送结束条件的时钟信号 
  board_delay_us(50);                //结束条件建立时间大于4μs 
  SDA_1;                        //发送I2C总线结束信号 
  board_delay_us(50);
}

//I2C写一个字节数据，返回ACK或者NACK
u8 I2C_Write_Byte(u8 Write_Byte)  //Sendbyte
{
  u8 i;
  SDA_O;
  SCL_0;
  board_delay_us(10);
  for(i=0; i<8; i++)            //要传送的数据长度为8位 
  {
    if(Write_Byte&0x80)   //判断发送位 
    {
      SDA_1;
    }
    else
    {
      SDA_0;
    }
    board_delay_us(5);
    SCL_1;                //输出SDA稳定后，拉高SCL给出上升沿，从机检测到后进行数据采样
    board_delay_us(5);         //保证时钟高电平周期大于4μs 
    SCL_0;
    board_delay_us(5);
    Write_Byte <<= 1;
  }
  board_delay_us(1);
  SDA_1;                      //8位发送完后释放数据线，准备接收应答位-ZLG
  board_delay_us(40);
  SCL_1;                      //MCU告知SHT2X数据发送完毕，等待从机的应答信号
  board_delay_us(40);
  SDA_I;
  /*以下是判断I2C总线接收应到应答信号是ACK还是NACK*/
  if(digitalRead(SDA)==1)                                   //SDA为高，收到NACK
  {
    board_delay_us(40);
    SCL_0;
    return NACK;
  }
  else                                         //SDA为低，收到ACK
  {
    board_delay_us(40);
    SCL_0;
    return ACK;

  }
}

//I2C读一个字节数据，入口参数用于控制应答状态，ACK或者NACK
u8 I2C_Read_Byte(u8 AckValue)//receivebyte
{
  u8 i,RDByte=0;
  SDA_O;
  SCL_0;                                   //置时钟线为低，准备接收数据位 
  board_delay_us(40);
  SDA_1;                                 //释放总线,置数据线为输入方式
  SDA_I; 
  for (i=0; i<8; i++)
  {
    SCL_1;                          //SCL高电平期间，采集SDA信号，并作为有效数据 //置时钟线为高使数据线上数据有效 
    board_delay_us(20);
    RDByte <<= 1;                  //移位
    if(digitalRead(SDA)==1)                           //采样获取数据
    {
      RDByte |= 0x01;
    }
    else
    {
      RDByte &= 0xfe;
    }
    board_delay_us(10);
    SCL_0;                             //下降沿，从机给出下一位值
    board_delay_us(60);
  }
  SDA_O;
  /*以下是I2C总线发送应答信号ACK或者NACK*/
  if(AckValue) SDA_1;
  else SDA_0;
  board_delay_us(30);
  SCL_1;
  board_delay_us(50);                  //时钟低电平周期大于4μs 
  SCL_0;                                  //清时钟线，钳住I2C总线以便继续接收 
  board_delay_us(150);
  return RDByte;
}


//初始化IIC接口
void SGP30_Init(void)
{
  SGP30_IO_Init();
  SGP30_Write(0x20,0x03);
}

void SGP30_Write(u8 a, u8 b)
{
  I2CStart();
  I2C_Write_Byte(SGP30_write); //发送器件地址+写指令
  I2C_Write_Byte(a);    //发送控制字节
  I2C_Write_Byte(b);
  I2CStop();
  board_delay_ms(100);
}

unsigned long SGP30_Read(void)
{
  unsigned long dat;
  int crc;
  I2CStart();
  I2C_Write_Byte(SGP30_read); //发送器件地址+读指令
  dat = I2C_Read_Byte(ACK);
  dat <<= 8;
  dat += I2C_Read_Byte(ACK);
  crc = I2C_Read_Byte(ACK); //check数据，舍去
  crc = crc;             //避免编译产生警告，这句可有可无
  dat <<= 8;
  dat += I2C_Read_Byte(ACK);
  dat <<= 8;
  dat += I2C_Read_Byte(NACK);
  I2CStop();
  return(dat);
}
