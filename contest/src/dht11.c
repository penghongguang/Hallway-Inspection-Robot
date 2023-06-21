#include "dht11.h"


void dht11_start()
{
  D_O;

  D_1;
  board_delay_us(2);
  D_0;
  board_delay_ms(25);
  D_1;
  board_delay_us(30);

  D_I;
}

unsigned char dht11_recv_byte()      //接收一个字节
{
  unsigned char i,dat=0;
  for(i=0;i<8;i++)    //从高到低依次接收8位数据
   {          
      while(digitalRead(DHT11_PIN));   //等待进入低电平
      while(!digitalRead(DHT11_PIN));   //等待50us低电平过去
      board_delay_us(50);     //延时50us，如果还为高则数据为1，否则为0 
      dat<<=1;//移位（低位补零）使正确接收8位数据，数据为0时直接移位
      if(digitalRead(DHT11_PIN))    //数据为1时，使dat加1来接收数据1
        dat+=1;
      while(digitalRead(DHT11_PIN));  //等待数据线拉低    
   }  
    return dat;
}

DHT11 dht11_read()
{
  unsigned char HH,HL,TH,TL,verify; 
  DHT11 dht11;

  dht11_start();
  
  if(!digitalRead(DHT11_PIN))
  {
    while(!digitalRead(DHT11_PIN));
    board_delay_us(80);

    HH      = dht11_recv_byte();
    HL      = dht11_recv_byte();
    TH      = dht11_recv_byte();
    TL      = dht11_recv_byte();
    verify  = dht11_recv_byte();

    board_delay_us(50);

    if((HH+HL+TH+TL)==verify)
    {
      if(TL>=10)
        dht11.t = TH + TL/100.0;
      else
        dht11.t = TH + TL/10.0;

      if(HL>=10)
        dht11.h = HH + HL/100.0;
      else
        dht11.h = HH + HL/10.0;

      dht11.status = true;  
    }
    else
    {
      dht11.status = false; 
    }
  }

  return dht11;
}
