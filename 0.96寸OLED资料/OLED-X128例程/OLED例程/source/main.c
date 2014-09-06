/******************************************************************************

          版权所有 (C), 2011-2012, 信意电子科技(http://xydz123.taobao.com/)

 ******************************************************************************
  文 件 名   : main.c
  版 本 号   : v1.0
  作    者   : Guokaiyi
  生成日期   : 2012-11-10
  最近修改   : 
  功能描述   : OLED演示例程
               说明: 
               ----------------------------------------------------------------
               G    电源地
               VCC  接5V或3.3v电源
               D0   PORTA_PA0 宏为 LED_SCL
               D1   PORTA_PA1 宏为 LED_SDA
               RST  PORTA_PA2 宏为 LED_RST
               DC   PORTA_PA3 宏为 LED_DC 
               CS   可以不接，已经默认选中                     
               ----------------------------------------------------------------
  修改历史   :
  1.日    期   : 2012-11-10
    作    者   : Guokaiyi
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------------------------------------*
 * 包含头文件                                                                 *
 *----------------------------------------------------------------------------*/
#include<string.h>
#include"common.h"
#include"system_funtion.h"
#include"oled.h"

/*----------------------------------------------------------------------------*
 * 宏定义                                                                     *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * 全局变量                                                                   *
 *----------------------------------------------------------------------------*/
//此部分内存很大，使用时请按需要调整
UCHAR8 g_aucImageBuf[32][LED_MAX_COLUMN_NUM];

/*----------------------------------------------------------------------------*
 * 内部函数原型                                                               *
 *----------------------------------------------------------------------------*/
VOID MAIN_Init(VOID);
VOID MAIN_OLED_Display(VOID);

/*----------------------------------------------------------------------------*
 * 外部函数原型                                                               *
 *----------------------------------------------------------------------------*/

VOID main(void) 
{
    MAIN_Init();

    MAIN_OLED_Display();
    
    return;
}

VOID MAIN_Init(VOID)
{
    SYSTEM_PLL_Init();
    LED_Init();

    return;
}

VOID MAIN_OLED_Display(VOID)
{
    USHORT16 i, j;

    LED_P14x16Str(25, 0, (UCHAR8*)"信意电子科技");
    LED_P8x16Str(0, 2, (UCHAR8*)" http://xydz123.taobao.com");
    SYSTEM_TIME_Delay(2000);

    LED_Fill(0x00);    
    LED_P14x16Str(30, 1, (UCHAR8*)"屏幕测试");

    for (j = 0; j <= 100; j++)
    {
        SYSTEM_TIME_Delay(40);
        LED_PrintChar(45, 5, (CHAR8)j);        
    }

    LED_Fill(0xFF); //点亮全屏
    SYSTEM_TIME_Delay(2000);

    LED_Fill(0x00); //黑屏
    SYSTEM_TIME_Delay(2000);

    //只要将赛道信息放进图片缓存里
    //然后调用LED_PrintImage函数即可显示实时赛道信息

    //清空图片缓存
    (VOID)memset(g_aucImageBuf, 0x00, 32 * LED_MAX_COLUMN_NUM);

    //由上向下扫屏
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < LED_MAX_COLUMN_NUM; j++)
        {
            g_aucImageBuf[i][j] = LED_IMAGE_WHITE;
        }
        //该函数需要的图片缓存太大请谨慎使用
        LED_PrintImage((UCHAR8*)g_aucImageBuf, 32, LED_MAX_COLUMN_NUM);
        SYSTEM_TIME_Delay(10);
    }

    //清空图片缓存
    (VOID)memset(g_aucImageBuf, 0x00, 32 * LED_MAX_COLUMN_NUM);
    
    //由左向右扫屏
    for (i = 0; i < LED_MAX_COLUMN_NUM; i++)
    {
        for (j = 0; j < 32; j++)
        {
            g_aucImageBuf[j][i] = LED_IMAGE_WHITE;
        }
        //该函数需要的图片缓存太大请谨慎使用
        LED_PrintImage((UCHAR8*)g_aucImageBuf, 32, LED_MAX_COLUMN_NUM);
        SYSTEM_TIME_Delay(10);
    }
    
    LED_Fill(0x00); //黑屏
    SYSTEM_TIME_Delay(1000);
    
    LED_P8x16Str(10, 2, (UCHAR8*)"Test Finished!");

    for (;;)
    {
        ;
    }
    
    return;
}

