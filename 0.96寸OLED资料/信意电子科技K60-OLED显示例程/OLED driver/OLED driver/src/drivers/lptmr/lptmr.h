/*
 * File:        lptmr.h
 * Purpose:     Provide common low power timer functions
 *
 * Notes:
 */

#ifndef __LPTMR_H__
#define __LPTMR_H__

/********************************************************************/


typedef enum LPT0_ALTn
{
    //只有1、2管脚，并没有0、3管脚
    LPT0_ALT1 = 1,      // PTA19
    LPT0_ALT2 = 2       // PTC5
} LPT0_ALTn;

typedef enum LPT_CFG
{
    LPT_Rising  = 0,    //上升沿触发
    LPT_Falling = 1     //下降沿触发
} LPT_CFG;

/*          用于延时         */

void time_delay_ms(uint32 ms);      //延时
typedef void (*LPTMR_ISR_CALLBACK)(void);



/*       用于脉冲计数        */
#define  lptmr_counter_clean()      LPTMR_CSR_REG(LPTMR0_BASE_PTR)  &= ~LPTMR_CSR_TEN_MASK;\
                                    LPTMR_CSR_REG(LPTMR0_BASE_PTR)  |=LPTMR_CSR_TEN_MASK            //计数器计数清0

void lptmr_counter_init(LPT0_ALTn, u16 count, u8 PrescaleValue, LPT_CFG);                           //计数器初始化设置
uint8_t LPLD_LPTMR_Init(uint8_t mod, uint16_t period_ms, uint8_t channel, uint8_t irqc, LPTMR_ISR_CALLBACK func);
void LPLD_LPTMR_Reset(void);
uint16_t LPLD_LPTMR_GetPulseAcc(void);




/********************************************************************/

#endif /* __LPTMR_H__ */
