/******************************************************************************

          版权所有 (C), 2011-2012, 信意电子科技(http://xydz123.taobao.com/)

 ******************************************************************************
  文 件 名   : ov5116.c
  版 本 号   : v1.0
  作    者   : Guokaiyi
  生成日期   : 2012-11-20
  最近修改   : 
  功能描述   : OV5116摄像头采集底层驱动

  修改历史   :
  1.日    期   : 2012-11-20
    作    者   : Guokaiyi
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------------------------------------*
 * 包含头文件                                                                 *
 *----------------------------------------------------------------------------*/
#include "ov5116.h"

/*----------------------------------------------------------------------------*
 * 宏定义                                                                     *
 *----------------------------------------------------------------------------*/
#define CCD_ROW_IGNORE 30 //忽略的行数

/*----------------------------------------------------------------------------*
 * 全局变量                                                                   *
 *----------------------------------------------------------------------------*/
volatile UCHAR8 g_ucRowItrpFlag;   //行中断标志
volatile UCHAR8 g_ucFieldItrpFlag; //场中断标志

/*----------------------------------------------------------------------------*
 * 内部函数原型                                                               *
 *----------------------------------------------------------------------------*/
VOID CCD_Delay(USHORT16 usTime);

/*----------------------------------------------------------------------------*
 * 外部函数原型                                                               *
 *----------------------------------------------------------------------------*/

/*****************************************************************************
 函 数 名  : CCD_Init
 功能描述  : 摄像头初始化
 输入参数  : NONE  
 输出参数  : NONE
 返 回 值  : NONE
*****************************************************************************/
VOID CCD_Init(VOID)
{
    CCD_SIGNAL_DIR = 0; // 设置图像管脚为输入 

    return;
}

/*****************************************************************************
 函 数 名  : CCD_GetImage
 功能描述  : 图像采集，并存放进pucImageTable指向的二维数组
 输入参数  : USHORT16 usRowNum       图像采集的行数
             USHORT16 usColumnNum    图像采集的列数
             USHORT16 usRowInterval  图像采集的行间隔数
 输出参数  : UCHAR8 *pucImageTable   二维图像数组的地址
 返 回 值  : NONE
*****************************************************************************/
VOID CCD_GetImage(UCHAR8 *pucImageTable, USHORT16 usRowNum, 
                  USHORT16 usColumnNum, USHORT16 usRowInterval)
{   
    USHORT16 i, j, usTotalRow;
    
    //总行数 = 行间隔 * 采集行数 + 忽略的行数
    usTotalRow = usRowInterval * usRowNum + CCD_ROW_IGNORE; 
    
    EnableInterrupts;        // 打开总中断允许
    CCD_ENABLE_ITRP_ROW();   // 打开行中断允许 
    CCD_ENABLE_ITRP_FIELD(); // 打开场中断允许 

    g_ucFieldItrpFlag = 0; // 清除残留场信号
    while (!g_ucFieldItrpFlag); // 等待场信号 

    for (i = 0; i < usTotalRow; i++) //33~313行为有效行
    {
        while (!g_ucRowItrpFlag); // 等待行信号 
        g_ucRowItrpFlag = 0;

        // 忽略前面CCD_ROW_IGNORE行 
        if (i < CCD_ROW_IGNORE) 
        {
            continue;
        }
        
        if (i % usRowInterval == 0) 
        {
            CCD_Delay(200); // 过滤每行图像前段无用信息，并令图像居中
                            // 按实际情况调节
            
            for (j = 0; j < usColumnNum; j++) 
            {
                *pucImageTable = CCD_SIGNAL; //采集二值化后的图像
                pucImageTable++;
            }
        }
    }

    CCD_DISNABLE_ITRP_ROW();   //关闭行中断允许
    CCD_DISNABLE_ITRP_FIELD(); // 关闭场中断允许 

    return;
}

/*****************************************************************************
 函 数 名  : CCD_Delay
 功能描述  : 延时函数 
 输入参数  : USHORT16 usTime
 输出参数  : NONE
 返 回 值  : NONE
*****************************************************************************/
VOID CCD_Delay(USHORT16 usTime)
{
    USHORT16 i;
    
    for (i = usTime; i > 0; i--)
    {
    }
    
    return;
}

// 行中断 
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt 25 VOID CCD_RowItrpProc(VOID) 
{
    CCD_CLR_ITRP_ROW_FLAG();
    g_ucRowItrpFlag = 1;
}
#pragma CODE_SEG DEFAULT

// 场中断 
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt 24 VOID CCD_FieldItrpProc(VOID) 
{   
    CCD_CLR_ITRP_FIELD_FLAG();
    g_ucFieldItrpFlag = 1;
}
#pragma CODE_SEG DEFAULT

