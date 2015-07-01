#include <stdio.h>
#include <LPC23xx.H>                    /* LPC23xx definitions                */

void BSP_C(void);
void BSP_CE(void);

static short AD_last;                   /* Last converted value               */

/* A/D IRQ: Executed when A/D Conversion is done                              */
__irq void ADC_IRQHandler(void) {
    short current = (AD0DR0 >> 6) & 0x3FF;    /* Read Conversion Result             */
    if (current < AD_last - 50) {
        AD_last = current;
        BSP_C();
    } else if (current > AD_last + 50) {
        AD_last = current;
        BSP_CE();
    }

    VICVectAddr = 0;                    /* Acknowledge Interrupt              */
}

/* Power enable, Setup pin, enable and setup AD converter interrupt           */
void ADC_Init(void) {
    PCONP |= (1 << 12);                        /* Enable power to AD block    */
    PINSEL1 = 0x4000;                          /* AD0.0 pin function select   */
    AD0INTEN = (1 << 0);                       /* CH0 enable interrupt        */
    AD0CR = 0x00200301;                        /* Power up, PCLK/4, sel AD0.0 */
    VICVectAddr18 = (unsigned long) ADC_IRQHandler;/* Set Interrupt Vector    */
    VICVectCntl18 = 14;                        /* use it for ADC Interrupt    */
		//VICIntEnable = (1 << 18);                  /* Enable ADC Interrupt        */
    //VICIntEnClr = (1 << 18);                   /* Disable ADC Interrupt        */
}

short AD_Get_Val(void) {
    return (AD_last);
}

void ADC_IRQ_ENABLE(void) {
		VICIntEnable = (1 << 18);                  /* Enable ADC Interrupt        */
}

void ADC_IRQ_DISABLE(void) {
		VICIntEnClr = (1 << 18);                  /* Disable ADC Interrupt        */
}


