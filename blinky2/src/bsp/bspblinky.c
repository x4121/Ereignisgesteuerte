
#include "bspblinky.h"
#include "../blinky2.h"

#include <stdio.h>
#include <LPC23xx.H>                    /* LPC23xx definitions                */

int scheduler_started;

void BSP_Init(void ) {
	
	 Timer0_Init();
	 ADC_Init();
	 init_serial ();
	 init_serial ();
	 LED_Init();
	 BSP_LCD_Init();
	
}


void BSP_LCD_Init(void ) {
	
	lcd_init ( );
	lcd_clear();
  lcd_print ("BLinky 1- FSM ");
  set_cursor (0, 1);
  lcd_print ("ESD-SS2015");

}

/* Function for displaying bargraph on the LCD display                        */
void BSP_Disp_Bargraph(int pos_x, int pos_y, int value) {
  int i;

  set_cursor (pos_x, pos_y);
  for (i = 0; i < 16; i++)  {
    if (value > 5)  {
      lcd_putchar (0x05);
      value -= 5;
    }  else  {
      lcd_putchar (value);
      value = 0;
    }
  }
}


static short lcd_posted;
static short v24_posted;

void FSM_Post_Evt( short evt ) {
	
	switch (evt)  {
		
		case BLINK_LED_LCD_SIG:
				lcd_posted =1;
		break;
		
	  case VT24_OUTPUT_SIG:
			v24_posted =1;
		
		break;
		
	}
}
short  FSM_Get_Evt( short evt ) {
	
	short rtc;
	switch (evt)  {
		
		case BLINK_LED_LCD_SIG:
				rtc = lcd_posted;
		    lcd_posted =0;
		break;
		
	  case VT24_OUTPUT_SIG:
			rtc= v24_posted;
			v24_posted =0;
		
		break;
		
	}
	return rtc;
}

/*..........................................................................*/
/* this function is used by the QP embedded systems-friendly assertions */
void Q_onAssert(char const * const file, int line) {
    printf("Assertion failed in %s, line %d", file, line);
    fflush(stdout);
    _sys_exit(-1);
}
/*..........................................................................*/
/* Application specific callbacks from the framework  */
void QF_onCleanup(void)	  {

	 ;

	 }

void QF_onIdle(void)	  {

	
	 ;

	 }
void QF_onStartup(void)	  {

	
	scheduler_started=1 ;

	 }

