
/*****************************************************************************
* Product:  Blinky as a  "Nested Switch Statement
* Ported to Keil MCB 2300 - NXP 2378 Arm/TDMI STANDALONE
*/


#include <stdio.h>
#include <LPC23xx.H>                    /* LPC23xx definitions                */


#include "bsp\bspblinky.h"
#include "statetbl.h"


typedef struct BlinkyTag {	/* the Blink FSM */
	  StateTable super;
    unsigned char state;                            				 /* the scalar state-variable */
    short AD_Value;
	  short AD_Print;
	  short AD_Old;
} Blinky;

void Blinky_ctor(Blinky *me );            								/* the "constructor" */
void Blinky_init(Blinky *me);                              /* init() function */
void Blinky_dispatch(Blinky *me, Event const *e);

void Blinky_initial  (Blinky *me, Event const *e);  /* transition function */
void Blinky_process  (Blinky *me, Event const *e);  /* transition function */
void Blinky_display  (Blinky *me, Event const *e);  /* transition function */

void Blinky_ad_ready  (Blinky *me, Event const *e);  /* transition function */
void Blinky_tick  (Blinky *me, Event const *e);  /* transition function */
void Blinky_blink_led_lcd  (Blinky *me, Event const *e);  /* transition function */
void Blinky_vt24_output  (Blinky *me, Event const *e);  /* transition function */

void Blinky_ctor(Blinky *me ){
	/* state table for state machine */
  static const Tran blinky_state_table[LENGTH_STATE][LENGTH_SIG] = {
        { (Tran)&Blinky_process, (Tran)&Blinky_display,
				   &StateTable_empty, &StateTable_empty},
        { (Tran)&Blinky_ad_ready,   (Tran)&Blinky_tick,
          (Tran)&Blinky_blink_led_lcd,  (Tran)&Blinky_vt24_output  }
    };
    StateTable_ctor(&me->super,
                    &blinky_state_table[0][0], LENGTH_STATE, LENGTH_SIG,
                    (Tran)&Blinky_initial);      /* construct the superclass */
	
	me->AD_Value = 0x3ff;    // start with arbitrary value for conversion
}

void Blinky_init(Blinky *me){
	
	me->state = PROCESSING_STATE;
	
}

void Blinky_initial  (Blinky *me, Event const *e) {
}

void Blinky_process  (Blinky *me, Event const *e) {
	if ( ((ADEvt const *)e)->AD_Last != me->AD_Value) 
		
		me->AD_Value=((ADEvt const *)e)->AD_Last;
		me->AD_Print = ((ADEvt const *)e)->AD_Last;
		me->AD_Value  /= 13;

		if ( me->AD_Value != me->AD_Old ) {
			
			me->AD_Old=me->AD_Value;
			// BSP_Disp_Bargraph(0, 1, me->AD_Value);
			FSM_Post_Evt(BLINK_LED_LCD_SIG);
		}
		TRAN(DISPLAYING_STATE);
}

void Blinky_blink_led_lcd  (Blinky *me, Event const *e) {
	unsigned int n;
	unsigned int v;
	BSP_Disp_Bargraph(0, 1, me->AD_Value);
	
	v = (5*(me->AD_Print)) >> 9;          /* Scale the Value                    */
	for (n = 0; n < 8; n++) {              /* Show the Value                     */
		if (n < v) LED_On (n);              /* Turn LED On                        */
		else       LED_Off(n);              /* Turn LED Off                       */
	}
	TRAN(DISPLAYING_STATE);
}

void Blinky_display  (Blinky *me, Event const *e) {
	TRAN(DISPLAYING_STATE);
	// FSM_Post_Evt(VT24_OUTPUT_SIG);
}

void Blinky_ad_ready  (Blinky *me, Event const *e) {
	TRAN(PROCESSING_STATE);
}

void Blinky_tick  (Blinky *me, Event const *e) {
	FSM_Post_Evt(VT24_OUTPUT_SIG);
	TRAN(DISPLAYING_STATE);
}

void Blinky_vt24_output  (Blinky *me, Event const *e) {
	printf ("AD value = 0x%03x\n\r", me->AD_Print);
	TRAN(DISPLAYING_STATE);
}

/*void Blinky_dispatch(Blinky *me, Event const *e) {
	
		unsigned int n;
		unsigned int v;
	switch (me->state) {
		
		case PROCESSING_STATE:
		
		 switch (e->sig) {
                case AD_READY_SIG:
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
								break;
		
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
									
									v = (5*(me->AD_Print)) >> 9;          // Scale the Value
									for (n = 0; n < 8; n++)               // Show the Value 
										if (n < v) LED_On (n);              // Turn LED On 
										else       LED_Off(n);              // Turn LED Off
							
								break;
								
								case VT24_OUTPUT_SIG:
									
									printf ("AD value = 0x%03x\n\r", me->AD_Print);
								
								break;
							}
		 TRAN(PROCESSING_STATE);		
		break;
		
	}
	
}
*/

static Blinky  l_blinky;

int main(void ) {
	 
		BSP_Init();
		
		printf("Blinky as a FSM - Nested Switched Case  Example\n\r");
	
		Blinky_init(&l_blinky);
		Blinky_ctor(&l_blinky);
	  while(1) {
			
		static ADEvt ad_evt = 		{ AD_READY_SIG, 0};
		static Event const tick_evt ={ TICK_SIG   };
		static Event const lcd_evt ={ BLINK_LED_LCD_SIG   };
		static Event const v24_evt ={ VT24_OUTPUT_SIG   };
		Event const *e = (Event *)0;
		
		ad_evt.AD_Last = AD_Get_Val();
		StateTable_dispatch((StateTable*) &l_blinky, (Event *)&ad_evt); /* dispatch ad  event */
		
		
    
	 if (  FSM_Get_Evt(BLINK_LED_LCD_SIG) ) {
		 
			e = &lcd_evt;
		StateTable_dispatch((StateTable*)&l_blinky, (Event *)e);
	 }
		
	  
	 if ( Get_Clock_LS () ) {
			 StateTable_dispatch((StateTable*) &l_blinky, (Event *)&tick_evt); /* dispatch time  event */
		 }
	 
	  if (  FSM_Get_Evt(VT24_OUTPUT_SIG) ) {
		 
			e = &v24_evt;
		  StateTable_dispatch((StateTable*) &l_blinky, (Event *)e);
	 }
		
	}
		

}

