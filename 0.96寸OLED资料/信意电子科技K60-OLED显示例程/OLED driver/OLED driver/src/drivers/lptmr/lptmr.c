/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：lptmr.c
 * 描述         ：lptmr定功耗定时器，目前只有一个定时器延时函数
 * 备注         ：官方例程
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：飞思卡尔公司
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/

#include "common.h"
#include "lptmr.h"
LPTMR_ISR_CALLBACK LPTMR_ISR;
#define LPTMR_irq     85u


//============================================================================
//函数名称：lptmr_counter_init
//函数返回：无
//参数说明：pin_select:选择的引脚（仅1,2）
//功能概要：脉冲累加计数中断
//
//============================================================================
void lptmr_counter_init(LPT0_ALTn altn, u16 count, u8 PrescaleValue, LPT_CFG cfg)
{

    if(PrescaleValue > 0x0f)PrescaleValue = 0x0f;

    //设置输入管脚
    if(altn == LPT0_ALT1)
    {
        SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;  //打开 PORTA 时钟
        PORTA_PCR19 = PORT_PCR_MUX(0x6);    //在PTA19上使用 ALT6
    }
    else if(altn == LPT0_ALT2)
    {
        SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;  //使能 PORTC 时钟
        PORTC_PCR5 = PORT_PCR_MUX(0x4);     //在PTC5上使用 ALT4
    }
    else                                    //不可能发生事件
    {
        assert_failed(__FILE__, __LINE__);  //设置管脚有误？
    }

    /* 开启模块时钟 */
    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;    //使能LPT模块时钟

    /* 清状态寄存器 */
    LPTMR0_CSR = 0x00;                      //先关了LPT，这样才能设置时钟分频等

    /* 设置累加计数值  */
    LPTMR_CMR_REG(LPTMR0_BASE_PTR)  =   LPTMR_CMR_COMPARE(count);                       //设置比较值

    /* 时钟选择 */
    LPTMR_PSR_REG(LPTMR0_BASE_PTR)  =   LPTMR_PSR_PCS(0x1) | LPTMR_PSR_PBYP_MASK  |  LPTMR_PSR_PRESCALE(PrescaleValue);        //使用 LPO clock 且 bypass glitch filter
    //                                                        开启和配置脉冲滤波器：2^n个时钟上升沿才识别

    /* 管脚设置、使能中断  */
    LPTMR_CSR_REG(LPTMR0_BASE_PTR)  =   LPTMR_CSR_TPS(altn) | LPTMR_CSR_TMS_MASK  | ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  | LPTMR_CSR_TEN_MASK  | LPTMR_CSR_TIE_MASK  ;
    //                                       选择输入管脚        选择脉冲计数                                   下降沿       上升沿           使能LPT
    // TFC = 0，即计数值等于比较值时，计数值复位

    enable_irq(LPTMR_irq);	                //开引脚的IRQ中断
}
/*
//清空计数
void lptmr_counter_clean(void)
{
    LPTMR_CSR_REG(LPTMR0_BASE_PTR)  &= ~LPTMR_CSR_TEN_MASK;     //禁用LPT的时候就会自动清计数器的值
    LPTMR_CSR_REG(LPTMR0_BASE_PTR)  |=LPTMR_CSR_TEN_MASK;
}*/



/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：time_delay_ms
*  功能说明：延时函数，使用定功耗定时器延时，准确
*  参数说明：ms   延时时间，单位为ms
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：官方例程自带
*************************************************************************/
void time_delay_ms(uint32 ms)
{
    /* Make sure the clock to the LPTMR is enabled */
    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;

    /* Set the compare value to the number of ms to delay */
    LPTMR0_CMR = ms;

    /* Set up LPTMR to use 1kHz LPO with no prescaler as its clock source */
    LPTMR0_PSR = LPTMR_PSR_PCS(1) | LPTMR_PSR_PBYP_MASK;

    /* Start the timer */
    LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;

    /* Wait for counter to reach compare value */
    while (!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK));

    /* Clear Timer Compare Flag */
    LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;

    return;
}


/*
 * LPLD_GPIO_Init
 * GPIO通用初始化函数
 * 
 * 参数:
 *    mod--选择LPTMR模式
 *      |__0-- 低功耗定时器模式
 *      |__1-- 脉冲累加模式
 *    period_ms--设定定时器的定时值 单位ms
 *      |__0-- 设置脉冲累加模式时选择为0
 *      |__1ms<= period_ms  <=65535ms 
 *    channel--选择累加输入通道
 *      |__0-- 设置为定时器时选择为0
 *      |__1-- 设置LPTMR_ALT1, PTA19为脉冲累加输入
 *      |__2-- 设置LPTMR_ALT2, PTC5 为脉冲累加输入
 *    irqc--输入中断
 *      |__0--不使能中断
 *      |__1--使能中断
 *    func
 *      |__低功耗定时器中断回调函数
 *  
 * 输出:
 *    0--配置错误
 *    1--配置成功
 */

uint8_t LPLD_LPTMR_Init(uint8_t mod, uint16_t period_ms, uint8_t channel, uint8_t irqc, LPTMR_ISR_CALLBACK func)
{
  SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;//初始化LPTMR时钟
   
  LPTMR0_CSR=0x00;  //清空寄存器
  LPTMR0_PSR=0x00;
  LPTMR0_CMR=0x00;
   
  if(mod==0)// 配置成低功耗定时器功能
  {
      
    /* 配置LPTMR */
    // LPO 1Khz
    //设置比较的值
    //使用内部时钟 分频系数Prescale为 2^(X+1)
    //定时器的一个计数周期=时钟源(Clk_Src)/ 预分频值(Prescale)
    //定时器的定时值=设置的比较值(compare_value)/定时器的一个计数周期
    LPTMR0_CMR=LPTMR_CMR_COMPARE(period_ms);                 //Set compare value
    LPTMR0_PSR=LPTMR_PSR_PCS(0x1)|LPTMR_PSR_PBYP_MASK;  //Use LPO clock with bypass enabled
        
  }
  else if(mod==1) //配置成脉冲捕获模式
  {
    if(channel==1)     //LPTMR_ALT1
    {
      SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; 
      PORTA_PCR19=PORT_PCR_MUX(0x6);     //设置PTA19为脉冲累加
    }
    else if(channel==2)//LPTMR_ALT2
    {
      SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; //设置PTC5为脉冲累加
      PORTC_PCR5=PORT_PCR_MUX(0x4);      
    }
    else
    {
      return 0;
    }
    
    //与所用的内部时钟无关
    //外部输入的信号作为计数时钟
    LPTMR0_PSR =LPTMR_PSR_PCS(0x1)|LPTMR_PSR_PBYP_MASK; 
    LPTMR0_CSR =LPTMR_CSR_TPS(channel);
    LPTMR0_CSR|=LPTMR_CSR_TMS_MASK; //捕获模式
    LPTMR0_CSR|=LPTMR_CSR_TFC_MASK; //自由计数模式
  }
  else
  {
    return 0;
  }
  
  if(func!=NULL)
  {
    LPTMR_ISR=func;
    enable_irq(85);
  }
  
  if(irqc==1)
  {
    LPTMR0_CSR|=LPTMR_CSR_TIE_MASK;
  }
  else if(irqc==0)
  {
    LPTMR0_CSR&=~LPTMR_CSR_TIE_MASK;
  }
  else
  {
    return 0;
  }
  
  LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //开启LPTMR
  return 1;
}

/*
 *  LPLD_LPTMR_Reset
 *  低功耗定时器复位，复位以后清空低功耗定时器Counter
 * 
 *  参数:
 *      无
 *  返回值:
 *      无
 */
void LPLD_LPTMR_Reset(void)
{
//  LPTMR0_CSR&=(~LPTMR_CSR_TEN_MASK);       //Reset LPTMR0 
  LPTMR_CSR_REG(LPTMR0_BASE_PTR)  &= ~LPTMR_CSR_TEN_MASK;
  LPTMR_CSR_REG(LPTMR0_BASE_PTR)  |=LPTMR_CSR_TEN_MASK; 
}


/*
 *  LPLD_LPTMR_GetPulseAcc
 *  得到脉冲累加的值
 * 
 *  参数:
 *      无
 *  返回值:
 *      脉冲累加值
 */
uint16_t LPLD_LPTMR_GetPulseAcc(void)
{
   return (uint16_t)LPTMR0_CNR;
}




