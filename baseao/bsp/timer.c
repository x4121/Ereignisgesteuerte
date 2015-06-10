#include <stdio.h>

#include <LPC23xx.H>                    /* LPC23xx definitions                */
#include "qp_port.h"  
static int clock_1s;


int Get_Clock_LS ( void ) {
	
	if ( clock_1s ) { 

		clock_1s=0;
		return(1);
	}
		
	else
		return(0);
	
	
}


/* Timer0 IRQ: Executed periodically                                          */
__irq void T0_IRQHandler (void) {
 
 static int clk_cntr;


  clk_cntr++;
  if (clk_cntr >= 1000) {
    clk_cntr = 0;
    clock_1s = 1;                       /* Activate flag every 1 second       */
  }
	
	QF_tick();
	
  AD0CR |= 0x01000000;                  /* Start A/D Conversion               */

  T0IR        = 1;                      /* Clear interrupt flag               */
  VICVectAddr = 0;                      /* Acknowledge Interrupt              */
}


void  Timer0_Init(void )  {
	
	
  /* Enable and setup timer interrupt, start timer                            */
  T0MR0         = 11999;                       /* 1msec = 12000-1 at 12.0 MHz */
  T0MCR         = 3;                           /* Interrupt and Reset on MR0  */
  T0TCR         = 1;                           /* Timer0 Enable               */
  VICVectAddr4  = (unsigned long)T0_IRQHandler;/* Set Interrupt Vector        */
  VICVectCntl4  = 15;                          /* use it for Timer0 Interrupt */
  VICIntEnable  = (1  << 4);                   /* Enable Timer0 Interrupt     */

	
}
