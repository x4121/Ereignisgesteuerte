#include <stdio.h>
#include <LPC23xx.H>                    /* LPC23xx definitions                */

void BSP_C(void);
void BSP_CE(void);

static short changed = 0;

/* A/D IRQ: Executed when A/D Conversion is done                              */
__irq void ADC_IRQHandler(void) {
    short current = (short) ((AD0DR0 >> 6) & 0x3FF);    /* Read Conversion Result             */
    if (current > 0x300) {
        if (!changed) {
            changed = 1;
            BSP_C();
        }
    } else if (current < 0x100) {
        if (!changed) {
            changed = 1;
            BSP_CE();
        }
    } else if (current > 0x100+50 && current < 0x300-50) {
        changed = 0;
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


void ADC_IRQ_ENABLE(void) {
		VICIntEnable = (1 << 18);                  /* Enable ADC Interrupt        */
}

void ADC_IRQ_DISABLE(void) {
		VICIntEnClr = (1 << 18);                  /* Disable ADC Interrupt        */
}


