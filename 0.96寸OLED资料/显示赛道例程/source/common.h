/******************************************************************************

          版权所有 (C), 2011-2012, 信意电子科技(http://xydz123.taobao.com/)

 ******************************************************************************
  文 件 名   : common.h
  版 本 号   : v1.0
  作    者   : Guokaiyi
  生成日期   : 2012-11-10
  最近修改   : 
  功能描述   : 

  修改历史   :
  1.日    期   : 2012-11-10
    作    者   : Guokaiyi
    修改内容   : 创建文件

******************************************************************************/
#ifndef __COMMON_H__
#define __COMMON_H__

/*----------------------------------------------------------------------------*
 * 包含头文件                                                                 *
 *----------------------------------------------------------------------------*/
#include <hidef.h>      /* common defines and macros */
#include "MC9S12XS128.h"
#include "derivative.h"      /* derivative-specific definitions */

/*----------------------------------------------------------------------------*
 * 宏定义                                                                     *
 *----------------------------------------------------------------------------*/
#ifndef VOID
    #define VOID void
#endif /* VOID */

#ifndef UCHAR8
    #define UCHAR8 unsigned char
#endif /* UCHAR8 */

#ifndef CHAR8
    #define CHAR8 char  
#endif /* CHAR8 */

#ifndef USHORT16
    #define USHORT16 unsigned short
#endif /* USHORT16 */

#ifndef SHORT16
    #define SHORT16 short
#endif /* SHORT16 */

#ifndef ULONG32
    #define ULONG32 unsigned long int  
#endif /* ULONG32 */

#ifndef LONG32
    #define LONG32 long int
#endif /* LONG32 */

#ifndef ULONG64
    #define ULONG64 unsigned long long int
#endif /* ULONG64 */

#ifndef LONG64
    #define LONG64 long long int
#endif /* LONG64 */

#ifndef TRUE
    #define TRUE 1
#endif /* TRUE */

#ifndef FALSE
    #define FALSE 0
#endif /* FALSE */

/*----------------------------------------------------------------------------*
 * 全局变量                                                                   *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * 内部函数原型                                                               *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * 外部函数原型                                                               *
 *----------------------------------------------------------------------------*/

#endif /* __COMMON_H__ */

