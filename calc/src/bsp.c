/*****************************************************************************
* Product: Board Support Package (BSP) for the QM example calc
* Keil ARM MDK  2300 
* Dr. Ron Barker 
*****************************************************************************/


#include "bsp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
  /* Exact-width types. WG14/N843 C99 Standard, Section 7.18.1.1 */
#include <stdint.h>

#include "LCD.h"

#define DISP_WIDTH  9

static char l_display[DISP_WIDTH + 1];            /* the calculator display */
static int  l_len;                        /* number of displayed characters */


/*BSP Init for the MC2300 */

void BSP_Init(void) {

  int i;
  

 // Init_Timer1( );
  init_serial();                               /* Init UART                   */
  uart_init_0 ( );
  lcd_init();
  lcd_clear();
  lcd_print ("MCB2300 HSM CALC");
  set_cursor (0, 1);
  lcd_print ("EventDrivenSystem");

  for (i = 0; i < 10000; i++);       /* Wait for initial display           */

}


/*..........................................................................*/
void BSP_clear(void) {
    memset(l_display, ' ', DISP_WIDTH - 1);
    l_display[DISP_WIDTH - 1] = '0';
    l_display[DISP_WIDTH] = '\0';
    l_len = 0;
}
/*..........................................................................*/
void BSP_insert(int keyId) {
    if (l_len == 0) {
        l_display[DISP_WIDTH - 1] = (char)keyId;
        ++l_len;
    }
    else if (l_len < (DISP_WIDTH - 1)) {
        memmove(&l_display[0], &l_display[1], DISP_WIDTH - 1);
        l_display[DISP_WIDTH - 1] = (char)keyId;
        ++l_len;
    }
}
/*..........................................................................*/
void BSP_negate(void) {
    BSP_clear();
    l_display[DISP_WIDTH - 2] = '-';
}
/*..........................................................................*/
void BSP_display(void) {
    printf("\n[%s] ", l_display);
 //   fflush(stdout);
}
 
/*..........................................................................*/
double BSP_get_value(void) {
    return strtod(l_display, (char **)0);
}
/*..........................................................................*/
int BSP_eval(double operand1, int oper, double operand2) {
    int ok = 1;
    double result;
    switch (oper) {
        case KEY_PLUS: {
            result = operand1 + operand2;
            break;
        }
        case KEY_MINUS: {
            result = operand1 - operand2;
            break;
        }
        case KEY_MULT: {
            result = operand1 * operand2;
            break;
        }
        case KEY_DIVIDE: {
            if ((operand2 < -1e-30) || (1e-30 < operand2)) {
                result = operand1 / operand2;
            }
            else {
            strcpy(l_display, " Error 0 ");        /* error: divide by zero */
                ok = 0;
            }
            break;
        }
    }
    if (ok) {
        if ((-0.000001 < result) && (result < 0.000001)) {
            result = 0.0;
        }
        if ((-99999999.0 < result) && (result < 99999999.0)) {
            sprintf(l_display, "%9.6g", result);
        }
        else {
            strcpy(l_display, " Error 1 ");          /* error: out of range */
            ok = 0;
        }
    }
    return ok;
}
/*..........................................................................*/
void BSP_message(char const *msg) {
    printf(msg);
}

 
/*..........................................................................*/
/* this function is used by the QP embedded systems-friendly assertions */
void Q_onAssert(char const * const file, int line) {
    printf("Assertion failed in %s, line %d", file, line);
    fflush(stdout);
    _sys_exit(-1);
}




int _kbhit(void ) {

	int i;
	 i = 0xFF;
	 return(i);
}


int _getch()

{
	 FILE *f;
 return	 ( fgetc(  f) );
}
	

void delay ( int val ){

int i;
	for(i=0;i<val; i++);


}


#ifdef BSP
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    _sys_exit(-1);
}
#endif		 
void  BSP_LCD_display_str( char * str, int line, int pos){

  set_cursor (line,pos);
  lcd_print ( (unsigned char const*) str);

  printf("%s",str);
};


/*..........................................................................*/
void BSP_display_str(char *msg) {

 // printf("%S", msg);
  BSP_LCD_display_str( msg,0,1);
  
}
/*..........................................................................*/
void BSP_exit(void) {
    printf("Bye, Bye!");
    _sys_exit(0);
}



void QF_onCleanup(void)	  {

	 ;

	 }
	


void QF_onIdle(void)	  {

	
	 ;

	 }
void QF_onStartup(void)	  {

	
	 ;

	 }

