// Created By 常曙&沉默的大树
// Last modified date: 2016-01-24
// Location: Chengdu

// TODO: Date/time switch
// TODO: Automatically turn off LED

#include "stm32f10x.h"
#include "HV57708.h"
#include "ds3231.h"
#include <stdbool.h>



u8 dis_data[]={9,3,4,3,3,3};
extern Time_Typedef TimeValue;
extern u8 Display_Time[8];

static bool Neon_status = 0;
static u8 second_previous = 0;

void Neon_Init(void)
{
    Neon_status = 0;
    GPIO_ResetBits(GPIOC, GPIO_Pin_1); // DOT1
    GPIO_ResetBits(GPIOC, GPIO_Pin_0); // DOT2
    GPIO_ResetBits(GPIOC, GPIO_Pin_3); // DOT3
    GPIO_ResetBits(GPIOC, GPIO_Pin_2); // DOT4
}

void Neon_Flip(void) 
{
    Neon_status = !Neon_status;
    if(Neon_status == 1) {
        GPIO_ResetBits(GPIOC, GPIO_Pin_1); // DOT1
        GPIO_ResetBits(GPIOC, GPIO_Pin_0); // DOT2
        GPIO_ResetBits(GPIOC, GPIO_Pin_3); // DOT3
        GPIO_ResetBits(GPIOC, GPIO_Pin_2); // DOT4
    } else {
        GPIO_SetBits(GPIOC, GPIO_Pin_1); // DOT1
        GPIO_SetBits(GPIOC, GPIO_Pin_0); // DOT2
        GPIO_SetBits(GPIOC, GPIO_Pin_3); // DOT3
        GPIO_SetBits(GPIOC, GPIO_Pin_2); // DOT4
    }
}

void LED_Init(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;														//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;														//速度50MHZ
	GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;														//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;														//速度50MHZ
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}

void LED_ON()
{
    GPIO_ResetBits(GPIOA,GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
    GPIO_ResetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_5);
}

void LED_OFF()
{
    GPIO_SetBits(GPIOA,GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
    GPIO_SetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_5);
}

int main(void)
{	
	//u32 i,j;
	//u8 x=0;
	
	HV57708_Init(HV57708_MODE_NORMAL);
	IIC_GPIO_Config(); 
    Neon_Init();
    LED_Init();
    //LED_ON();
    //Neon_Flip();
    
	//GPIO_SetBits(GPIOA,GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
 	//GPIO_SetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_5);
	//GPIO_SetBits(GPIOC,GPIO_Pin_14|GPIO_Pin_15);
    
    //while(1) {
    //    Neon_Flip();
    //    Delay(5000);
    //}
    
    //HV57708_SendData(0, 0);
    //HV57708_OutputData();
    //Neon_Flip();
    //while(1);
    
   //SetDisplay(dis_data);
   //while(1);
   
//Relay
	//GPIO_ResetBits(GPIOB,GPIO_Pin_9);
	//GPIO_SetBits(GPIOB,GPIO_Pin_9);
	
	//GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
	
	TimeValue.year=DS3231_YEARDATA;
	TimeValue.month=0x01;
	TimeValue.week=0x07;
	TimeValue.date=0x24;
	TimeValue.hour=0x02;
	TimeValue.minute=0x49;
	TimeValue.second=0x00;

    Time_Handle();
    second_previous = Display_Time[7] - 0x30;
	//DS3231_Time_Init(&TimeValue);
    //Time_Handle();
    //while(1);
	while(1) {
        Time_Handle();
        dis_data[0] = Display_Time[0] - 0x30;
        dis_data[1] = Display_Time[1] - 0x30;
        dis_data[2] = Display_Time[3] - 0x30;
        dis_data[3] = Display_Time[4] - 0x30;
        dis_data[4] = Display_Time[6] - 0x30;
        dis_data[5] = Display_Time[7] - 0x30;
        if(dis_data[5] != second_previous) {
            second_previous = dis_data[5];
            Neon_Flip();
        }
        SetDisplay(dis_data);
        Delay(500);
    }
}

