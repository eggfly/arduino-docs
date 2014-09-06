/******************************************************************************

          版权所有 (C), 2011-2012, 信意电子科技(http://xydz123.taobao.com/)

 ******************************************************************************
  文 件 名   : system_funtion.c
  版 本 号   : v2.0
  作    者   : Guokaiyi
  生成日期   : 2012-12-1
  最近修改   : 
  功能描述   : 芯片内部功能函数
  修改历史   :
  1.日    期   : 2012-11-12
    作    者   : Guokaiyi
    修改内容   : 创建文件
    
  2.日    期   : 2012-12-1
    作    者   : Guokaiyi
    修改内容   : 增加DFLASH功能函数

******************************************************************************/

/*----------------------------------------------------------------------------*
 * 包含头文件                                                                 *
 *----------------------------------------------------------------------------*/
#include "system_funtion.h"

/*----------------------------------------------------------------------------*
 * 宏定义                                                                     *
 *----------------------------------------------------------------------------*/
#define FLASH_DFLASH_GLOBAL_ADDR_BASE   0x0010
#define FLASH_DFLASH_LOCAL_ADDR_BASE    0x0800
#define FLASH_DFLASH_PAGE_SIZE          0x0400  // 1Kbytes
/*----------------------------------------------------------------------------*
 * 枚举定义                                                                     *
 *----------------------------------------------------------------------------*/
enum tagSystemFlashCmd
{
    FLASH_ERASE_VERIFY_ALL_BLOCKS     = 0x01,
    FLASH_ERASE_VERIFY_BLOCK          = 0x02,
    FLASH_ERASE_VERIFY_PFLASH_SECTION = 0x03,
    FLASH_READ_ONCE                   = 0x04,
    FLASH_PROGRAM_PFLASH              = 0x06,
    FLASH_PROGRAM_ONCE                = 0x07,
    FLASH_ERASE_ALL_BLOCKS            = 0x08,
    FLASH_ERASE_FLASH_BLOCK           = 0x09,
    FLASH_ERASE_PFLASH_SECTOR         = 0x0A,
    FLASH_UNSECURE_FLASH              = 0x0B,
    FLASH_VERIFY_BACKDOOR_ACCESS_KEY  = 0x0C,
    FLASH_SET_USER_MARGIN_LEVEL       = 0x0D,
    FLASH_SET_FIELD_MARGIN_LEVEL      = 0x0E,
    FLASH_ERASE_VERIFY_DFLASH_SECTION = 0x10,
    FLASH_PROGRAM_DFLASH              = 0x11,
    FLASH_ERASE_DFLASH_SECTOR         = 0x12,
}SystemFlashCmd_E;

/*----------------------------------------------------------------------------*
 * 全局变量                                                                   *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * 内部函数原型                                                               *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * 外部函数原型                                                               *
 *----------------------------------------------------------------------------*/

/*****************************************************************************
 函 数 名  : PLL_Init
 功能描述  : 初始化锁相环
 输入参数  : NONE  
 输出参数  : NONE
 返 回 值  : NONE
*****************************************************************************/
VOID SYSTEM_PLL_Init(VOID) 
{
    CLKSEL &= 0x7f;       //set OSCCLK as sysclk
    PLLCTL &= 0x8F;       //Disable PLL circuit
    CRGINT &= 0xDF;

    #if (SYSTEM_BUS_CLOCK == 40000000) 
    SYNR = 0x44;
    #elif (SYSTEM_BUS_CLOCK == 64000000)
    SYNR = 0xc7;     
    #elif (SYSTEM_BUS_CLOCK == 24000000)
    SYNR = 0x42;
    #endif 

    REFDV  = 0xC1;            //PLLCLK=2×OSCCLK×(SYNR+1)/(REFDV+1)＝128MHz ,fbus=32M
    PLLCTL = PLLCTL | 0x70;   //Enable PLL circuit
    
    asm NOP;
    asm NOP;
    
    while (!(CRGFLG & 0x08)); //PLLCLK is Locked already
    CLKSEL |= 0x80;           //set PLLCLK as sysclk

    return;
}

/*****************************************************************************
 函 数 名  : SYSTEM_ATD_Init
 功能描述  : 芯片内部数模转换模块初始化
 输入参数  : NONE  
 输出参数  : NONE
 返 回 值  : NONE
*****************************************************************************/
VOID SYSTEM_ATD_Init(VOID)
{
    ATD0CTL2 = 0xc0;  //启动A/D, 快速清零, 无等待模式, 禁止外部触发, 中断禁止
    ATD0CTL1_SRES = 0;  //选用8位模数转换
    ATD0CTL3 = 0x88;  //每次只转换一个通道,No FIFO, Freeze模式下继续转换
    ATD0CTL4 = 0x01;  //设置AD模块时钟频率

    return;
}

/*****************************************************************************
 函 数 名  : SYSTEM_ATD_Convert
 功能描述  : 芯片内部数模转换
 输入参数  : VOID  
 输出参数  : NONE
 返 回 值  : UCHAR8 数模转换结果 0-255
*****************************************************************************/
UCHAR8 SYSTEM_ATD_Convert(VOID) 
{
    UCHAR8 ucATDResult;
    
    ATD0CTL5 = 0x06;    //转换AD06
    while (!ATD0STAT0_SCF);
    ucATDResult = ATD0DR0L;

    return ucATDResult;
}

/*****************************************************************************
 函 数 名  : SYSTEM_TIME_Delay
 功能描述  : 延时函数 
 输入参数  : USHORT16 usTime
 输出参数  : NONE
 返 回 值  : NONE
*****************************************************************************/
VOID SYSTEM_TIME_Delay(USHORT16 usTime)
{
    ULONG32 i;
    USHORT16 j;
    
    for (i = (ULONG32)100 * usTime; i > 0; i--)
    {
        for(j = 210; j > 0; j--)
        {
        }
    }
    
    return;
}
  
/*****************************************************************************
 函 数 名  : SYSTEM_DFLASH_Init
 功能描述  : 芯片内部DFlash初始化
 输入参数  : NONE  
 输出参数  : NONE
 返 回 值  : NONE
*****************************************************************************/
VOID SYSTEM_DFLASH_Init(VOID)  
{    
    while (!FSTAT_CCIF);      //向任意FLASH寄存器写入前都需保证CCIF不为0即上次命令完成
    FCLKDIV = 0x0F;           //外部晶振为16M.FLASH时钟不超过1M，具体参照手册  
    FCNFG   = 0x00;           //禁止中断  
    while (!FCLKDIV_FDIVLD);  //等待时钟设置成功

    return;
}  

/*****************************************************************************
 函 数 名  : SYSTEM_DFLASH_Erase
 功能描述  : 擦除DFLASH的一个扇区
             S12XS128的DFLASH共有32个扇区，每扇区265 bytes，共8Kbytes
             扇区  起始地址的低16位
               0:  0x0000     1:  0x0100     2:  0x0200     3:  0x0300
               4:  0x0400     5:  0x0500     6:  0x0600     7:  0x0700
               8:  0x0800     9:  0x0900    10:  0x0A00    11:  0x0B00
              12:  0x0C00    13:  0x0D00    14:  0x0E00    15:  0x0F00
              16:  0x1000    17:  0x1100    18:  0x1200    19:  0x1300
              20:  0x1400    21:  0x1500    22:  0x1600    23:  0x1700
              24:  0x1800    25:  0x1900    26:  0x1A00    27:  0x1B00
              28:  0x1C00    29:  0x1D00    30:  0x1E00    31:  0x1F00

 输入参数  : USHORT16 usAddress 所选扇区的起始地址的低16位
 输出参数  : NONE
 返 回 值  : NONE
*****************************************************************************/
VOID SYSTEM_DFLASH_Erase(USHORT16 usAddress)
{
    while (!FSTAT_CCIF);     //向任意FLASH寄存器写入前都需保证CCIF不为0即上次命令完成
    if (1 == FSTAT_ACCERR)   //判断读写错误标志
    {
        FSTAT_ACCERR = 1;    //清除标志位
    }
    if (1 == FSTAT_FPVIOL)   //判断保护违反标志
    {
        FSTAT_FPVIOL = 1;    //清除标志位
    }
    
    FCCOBIX_CCOBIX = 0x00; 
    FCCOB = ((USHORT16)FLASH_ERASE_DFLASH_SECTOR << 8) | FLASH_DFLASH_GLOBAL_ADDR_BASE; //写入命令和高位地址  
    FCCOBIX_CCOBIX = 0x01;   
    FCCOB = usAddress;       //写入低16位地址 
    
    FSTAT_CCIF=1;            //启动执行命令  
    while (!FSTAT_CCIF);     //等待执行完成  

    return;
}  

/*****************************************************************************
 函 数 名  : SYSTEM_DFLASH_Write
 功能描述  : 向芯片内部DFlash写入数据，向已写入数据的空间写入前需擦除，否则写入失败
 输入参数  : USHORT16 usAddress   0x0000 - 0x1FF8
             USHORT16 *pusDataBuf 大于4words(8bytes)的缓存地址
 输出参数  : NONE
 返 回 值  : NONE
*****************************************************************************/
VOID SYSTEM_DFLASH_Write(USHORT16 usAddress, USHORT16 *pusDataBuf)  
{  
    while (!FSTAT_CCIF);     //向任意FLASH寄存器写入前都需保证CCIF不为0即上次命令完成
    if(1 == FSTAT_ACCERR)    //判断读写错误标志
    {
        FSTAT_ACCERR = 1;    //清除标志位
    }
    if(1 == FSTAT_FPVIOL)    //判断保护违反标志
    {
        FSTAT_FPVIOL = 1;    //清除标志位
    }

    FCCOBIX_CCOBIX = 0x00;   
    FCCOB = ((USHORT16)FLASH_PROGRAM_DFLASH << 8) | FLASH_DFLASH_GLOBAL_ADDR_BASE; //写入命令和高位地址 
    FCCOBIX_CCOBIX = 0x01;   
    FCCOB = usAddress;       //写入低16位地址  
    FCCOBIX_CCOBIX = 0x02;   
    FCCOB = pusDataBuf[0];   //写入第一个数据  
    FCCOBIX_CCOBIX = 0x03;   
    FCCOB = pusDataBuf[1];   //写入第二个数据  
    FCCOBIX_CCOBIX = 0x04;   
    FCCOB = pusDataBuf[2];   //写入第三个数据  
    FCCOBIX_CCOBIX = 0x05;   
    FCCOB = pusDataBuf[3];   //写入第四个数据  

    FSTAT_CCIF = 1;          //写入执行命令  
    while (!FSTAT_CCIF);     //等待执行完毕  

    return;
}  
  
/*****************************************************************************
 函 数 名  : SYSTEM_DFLASH_Read
 功能描述  : 从芯片内部DFlash读出数据
 输入参数  : USHORT16 usAddress   0x0000 - 0x1FF8 
             USHORT16 *pusDataBuf 大于4words(8bytes)的缓存地址
 输出参数  : NONE
 返 回 值  : NONE
*****************************************************************************/
VOID SYSTEM_DFLASH_Read(USHORT16 usAddress, USHORT16 *pusDataBuf)  
{  
    UCHAR8 ucLastEpage;
    USHORT16 *pusMapAddr;

    ucLastEpage = EPAGE;     //保存EPAGE的值  

    EPAGE = (UCHAR8)(usAddress >> 10); //计算EPAGE  

    //计算usAddress映射到CPU内存的地址
    pusMapAddr = (USHORT16*)(FLASH_DFLASH_LOCAL_ADDR_BASE + (usAddress & (FLASH_DFLASH_PAGE_SIZE - 1))); 

    pusDataBuf[0] = *pusMapAddr;
    pusDataBuf[1] = *(pusMapAddr + 1);
    pusDataBuf[2] = *(pusMapAddr + 2);
    pusDataBuf[3] = *(pusMapAddr + 3);

    EPAGE = ucLastEpage;      //恢复EPAGE的值  

    return;
}  

