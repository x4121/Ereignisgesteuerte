
/*****************************************************************************
* Product:  Blinky as a  "Nested Switch Statement
* Ported to Keil MCB 2300 - NXP 2378 Arm/TDMI STANDALONE
*/


#include <stdio.h>
#include <LPC23xx.H>                    /* LPC23xx definitions                */


#include "bsp\bspblinky.h"

void Blinky_ctor(Blinky *me ){
	
	me->AD_Value = 0x3ff;    // start with arbitrary value for conversion
	
}
void Blinky_init(Blinky *me){
	
	me->state = PROCESSING_STATE;
	
}

void Blinky_dispatch(Blinky *me, Event const *e) {
	
		unsigned int n;
		unsigned int v;
	switch (me->state) {
		
		case PROCESSING_STATE:
		
		 switch (e->sig) {
                case ADREADY__SIG:
								if ( ((ADEvt const *)e)->AD_Last != me->AD_Value) 
									
									me->AD_Value=((ADEvt const *)e)->AD_Last;
									me->AD_Print = ((ADEvt const *)e)->AD_Last;
									me->AD_Value  /= 13;
							
								  if ( me->AD_Value != me->AD_Old ) {
										
										me->AD_Old=me->AD_Value;
										// BSP_Disp_Bargraph(0, 1, me->AD_Value);
										FSM_Post_Evt(BLINK_LED_LCD_SIG);
										TRAN(DISPLAYING_STATE);
									}
		
								case TICK_SIG:
									
										FSM_Post_Evt(VT24_OUTPUT_SIG);
										TRAN(DISPLAYING_STATE);
								break;
									
							}
		break;
		
		case DISPLAYING_STATE:
			
		 switch (e->sig) {
                					
								case BLINK_LED_LCD_SIG:
						
									BSP_Disp_Bargraph(0, 1, me->AD_Value);
									
									v = (5*(me->AD_Print)) >> 9;          /* Scale the Value                    */
									for (n = 0; n < 8; n++)               /* Show the Value                     */
										if (n < v) LED_On (n);              /* Turn LED On                        */
										else       LED_Off(n);              /* Turn LED Off                       */
							
								break;
								
								case VT24_OUTPUT_SIG:
									
									printf ("AD value = 0x%03x\n\r", me->AD_Print);
								
								break;
							}
		 TRAN(PROCESSING_STATE);		
		break;
		
	}
	
}	

static Blinky  l_blinky;

int main(void ) {
	 
		BSP_Init();
		
		printf("Blinky as a FSM - Nested Switched Case  Example\n\r");
	
		Blinky_init(&l_blinky);
		Blinky_ctor(&l_blinky);
	  while(1) {
			
		static ADEvt ad_evt = 		{ ADREADY__SIG, 0};
		static Event const tick_evt ={ TICK_SIG   };
		static Event const lcd_evt ={ BLINK_LED_LCD_SIG   };
		static Event const v24_evt ={ VT24_OUTPUT_SIG   };
		Event const *e = (Event *)0;
		
		ad_evt.AD_Last = AD_Get_Val();
		Blinky_dispatch(&l_blinky, (Event *)&ad_evt); /* dispatch ad  event */
		
		
    
	 if (  FSM_Get_Evt(BLINK_LED_LCD_SIG) ) {
		 
			e = &lcd_evt;
		 
	 }
		
		Blinky_dispatch(&l_blinky, (Event *)e);
	  
	 if ( Get_Clock_LS () ) {
			 Blinky_dispatch(&l_blinky, (Event *)&tick_evt); /* dispatch time  event */
		 }
	 
	  if (  FSM_Get_Evt(VT24_OUTPUT_SIG) ) {
		 
			e = &v24_evt;
		  Blinky_dispatch(&l_blinky, (Event *)e);
	 }
		
	}
		

}

