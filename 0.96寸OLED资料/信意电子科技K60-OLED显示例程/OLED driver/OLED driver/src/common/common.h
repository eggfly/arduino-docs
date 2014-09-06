/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：common.h
 * 描述         ：通用配置头文件，适合全部的工程
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_

//为使用仿真模式而添加的，仿真模式应该屏蔽串口发送、和PLL锁相环设置
//#define   Simulator
#ifdef    Simulator
#define NO_PLL_INIT   //禁用锁相环
#define NPRINTF       //禁用printf
#endif

/********************************************************************/

/*
 * Debug prints ON (#define) or OFF (#undef)
 */
#define DEBUG_PRINT


/*****************************设置数据类型*****************************/
typedef 	unsigned 	char		u8;	  //无符号型
typedef 	unsigned 	short int	u16;
typedef 	unsigned 	long  int	u32;
typedef    	__UINT64_T_TYPE__       u64;

typedef 	char					s8;	  //有符号型
typedef 	short int				s16;
typedef 	long  int				s32;
typedef    	__INT64_T_TYPE__     	s64;

typedef unsigned char byte;
typedef unsigned int uint;

typedef char *pchar;
typedef unsigned char *puchar;
typedef int  *pint;
typedef unsigned int *puint;
/* Fixed size types. These are all optional. */
#ifdef __INT8_T_TYPE__
  typedef __INT8_T_TYPE__   int8_t;
  typedef __UINT8_T_TYPE__ uint8_t;
#endif /* __INT8_T_TYPE__ */

#ifdef __INT16_T_TYPE__
  typedef __INT16_T_TYPE__   int16_t;
  typedef __UINT16_T_TYPE__ uint16_t;
#endif /* __INT16_T_TYPE__ */

#ifdef __INT32_T_TYPE__
  typedef __INT32_T_TYPE__   int32_t;
  typedef __UINT32_T_TYPE__ uint32_t;
#endif /* __INT32_T_TYPE__ */

#ifdef __INT64_T_TYPE__
  #pragma language=save
  #pragma language=extended
  typedef __INT64_T_TYPE__   int64_t;
  typedef __UINT64_T_TYPE__ uint64_t;
  #pragma language=restore
#endif /* __INT64_T_TYPE__ */
/**************************************************/

typedef	union
{
	u32	DW;
	u16	W[2];
	u8	B[4];
	struct
	{
		u32 b0:1; u32 b1:1; u32 b2:1; u32 b3:1; u32 b4:1; u32 b5:1; u32 b6:1; u32 b7:1; 
		u32 b8:1; u32 b9:1; u32 b10:1;u32 b11:1;u32 b12:1;u32 b13:1;u32 b14:1;u32 b15:1;
		u32 b16:1;u32 b17:1;u32 b18:1;u32 b19:1;u32 b20:1;u32 b21:1;u32 b22:1;u32 b23:1;
		u32 b24:1;u32 b25:1;u32 b26:1;u32 b27:1;u32 b28:1;u32 b29:1;u32 b30:1;u32 b31:1;
	};
}Dtype;		//sizeof(Dtype)	为 4 

/*
 * 定义运行到RAM的函数
 */
#if defined(__ICCARM__)
	#define 	__RAMFUN	__ramfunc	//IAR 用 __ramfunc 来声明
#else
	#define 	__RAMFUN
#endif

/*
 * Include the generic CPU header file
 */
#define ARM_MATH_CM4
#include "arm_math.h"
#include "arm_cm4.h"

/*
 * Include the cpu specific header file
 */
#include "MK60DZ10.h"


/*
 * Include the platform specific header file
 */
#include "k60_fire.h"




/*
 * Include any toolchain specfic header files
 */
#if (defined(CW))
#include "cw.h"
#elif (defined(IAR))
#include "iar.h"
#else
#warning "No toolchain specific header included"
#endif

/*
 * Include common utilities
 */
#include "assert.h"
#include "io.h"
#include "startup.h"
#include "stdlib.h"


#if (defined(IAR))
#include "intrinsics.h"
#endif


#include  "sysinit.h"           //系统配置
#include "mcg.h"
#include "fire_drivers_cfg.h"   //管脚复用配置



#include "lptmr.h"
#define DELAY()         time_delay_ms(500)
#define DELAY_MS(ms)    time_delay_ms(ms)

extern volatile u8 row_flag; // 行中断标志位 
extern volatile u8 field_flag; // 场中断标志位



/********************************************************************/

#endif /* _COMMON_H_ */
