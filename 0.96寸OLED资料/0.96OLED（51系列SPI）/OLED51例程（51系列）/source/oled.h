/******************************************************************************

          版权所有 (C), 2011-2012, 信意电子科技(http://xydz123.taobao.com/)

 ******************************************************************************
  文 件 名   : oled.h
  版 本 号   : v1.0
  作    者   : Guokaiyi
  生成日期   : 2012-11-12
  最近修改   : 
  功能描述   : oled.c 的头文件
  修改历史   :
  1.日    期   : 2012-11-12
    作    者   : Guokaiyi
    修改内容   : 创建文件

******************************************************************************/
#ifndef __OLED_H__
#define __OLED_H__

/*----------------------------------------------------------------------------*
 * 包含头文件                                                                 *
 *----------------------------------------------------------------------------*/
#include "common.h"

/*----------------------------------------------------------------------------*
 * 宏定义                                                                     *
 *----------------------------------------------------------------------------*/
#define LED_IMAGE_WHITE       1
#define LED_IMAGE_BLACK       0

#define LED_MAX_ROW_NUM      64
#define LED_MAX_COLUMN_NUM  128

/*----------------------------------------------------------------------------*
 * 全局变量                                                                   *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * 内部函数原型                                                               *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * 外部函数原型                                                               *
 *----------------------------------------------------------------------------*/
extern VOID LED_Init(VOID);
extern VOID LED_SetPos(UCHAR8 ucIdxX, UCHAR8 ucIdxY); 
extern VOID LED_P6x8Char(UCHAR8 ucIdxX,UCHAR8 ucIdxY,UCHAR8 ucData);
extern VOID LED_P6x8Str(UCHAR8 ucIdxX,UCHAR8 ucIdxY,UCHAR8 ucDataStr[]);
extern VOID LED_P8x16Str(UCHAR8 ucIdxX,UCHAR8 ucIdxY,UCHAR8 ucDataStr[]);
extern VOID LED_P14x16Str(UCHAR8 ucIdxX,UCHAR8 ucIdxY,UCHAR8 ucDataStr[]);
extern VOID LED_PXx16MixStr(UCHAR8 ucIdxX, UCHAR8 ucIdxY, UCHAR8 ucDataStr[]);
extern VOID LED_Fill(UCHAR8 ucData);
extern VOID LED_PrintChar(UCHAR8 ucIdxX, UCHAR8 ucIdxY, CHAR8 cData);
extern VOID LED_PrintShort(UCHAR8 ucIdxX, UCHAR8 ucIdxY, SHORT16 sData);
extern VOID LED_PrintImage(UCHAR8 *pucTable, USHORT16 usRowNum, USHORT16 usColumnNum);

#endif

