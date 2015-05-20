#include <stdio.h>
#include <LPC23xx.H>                    /* LPC23xx definitions                */

#include "..\statetbl.h"




short  FSM_Get_Evt( short evt );
void   FSM_Post_Evt( short evt );

void BSP_Init(void );
void BSP_LCD_Init(void);
void BSP_Disp_Bargraph(int pos_x, int pos_y, int value);

/*  Forward Declarations  */
void  Timer0_Init(void );
int Get_Clock_LS ( void );

void 	ADC_Init(void);
short AD_Get_Val(void );


void lcd_init  (void);
void lcd_clear   (void);
void lcd_print ( char * msg);
void lcd_putchar (int val);
void set_cursor (int x, int y);


void init_serial (void);
void LED_Init(void) ;
void LED_On (unsigned int n);
void LED_Off (unsigned int n);
