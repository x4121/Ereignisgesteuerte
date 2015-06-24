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

#include "../bsp/LCD.h"

#define DISP_WIDTH  9
#define LCD_WIDTH  17

static enum display_state_t {
    DS_CLEAR,
    DS_OP1,
    DS_OP2,
    DS_ERR
} display_state;

static struct display_t {
    char op1[DISP_WIDTH + 1];
    int  op1_len;
    char op2[DISP_WIDTH + 1];
    int  op2_len;
    char op;
    char result[LCD_WIDTH + 1];
    int  result_len;
} display;

static char l_display[DISP_WIDTH + 1];            /* the calculator display */
//static char lcd_display_top[LCD_WIDTH + 1];
//static char lcd_display_bot[LCD_WIDTH + 1];
//static char operand1[DISP_WIDTH];
//static char operand2[DISP_WIDTH];
static int  l_len;                        /* number of displayed characters */
//static int  lcd_top_pos;
//static int  lcd_bot_pos;

void delay ( int val ){
    int i;
    for(i=0;i<val; i++);
}

void wait(void) {
    delay(10000);       /* Wait for initial display           */
}

void display_clear() {
    display.op = ' ';

    memset(display.op1, ' ', DISP_WIDTH);
    display.op1[DISP_WIDTH] = '\0';
    display.op1_len = 0;

    memset(display.op2, ' ', DISP_WIDTH);
    display.op2[DISP_WIDTH] = '\0';
    display.op2_len = 0;

    memset(display.result, ' ', LCD_WIDTH);
    display.result[LCD_WIDTH] = '\0';
    display.result_len = 0;
}

void BSP_err(char* msg) {
    display_state = DS_ERR;
    strcpy(display.op1, msg);
    strcpy(l_display, msg);
}  

void clear(void) {
    BSP_clear();
    display_clear();
    display_state = DS_CLEAR;
}

/*BSP Init for the MC2300 */
void BSP_Init(void) {
    // Init_Timer1( );
    init_serial();                               /* Init UART                   */
    uart_init_0();
    lcd_init();
    clear();

    wait();
}

/*..........................................................................*/
void BSP_clear(void) {
    memset(l_display, ' ', DISP_WIDTH - 1);
    l_display[DISP_WIDTH - 1] = '0';
    l_display[DISP_WIDTH] = '\0';
    l_len = 0;
    BSP_display();
}

/*..........................................................................*/
void BSP_insert(int keyId) {
    if (l_len == 0) {
        l_display[DISP_WIDTH - 1] = (char)keyId;
        ++l_len;
    } else if (l_len < (DISP_WIDTH - 1)) {
        memmove(&l_display[0], &l_display[1], DISP_WIDTH - 1);
        l_display[DISP_WIDTH - 1] = (char)keyId;
        ++l_len;
    }

    switch (display_state) {
        case DS_CLEAR:
            if (display.op1_len < DISP_WIDTH - 1) {
                if (display.op1_len != 0)
                    memmove(&display.op1[0], &display.op1[1], DISP_WIDTH - 1);
                display.op1[DISP_WIDTH - 1] = (char)keyId;
                ++display.op1_len;
            }
            break;
        case DS_OP1:
            if (display.op2_len < DISP_WIDTH - 1) {
                if (display.op2_len != 0)
                    memmove(&display.op2[0], &display.op2[1], DISP_WIDTH - 1);
                display.op2[DISP_WIDTH - 1] = (char)keyId;
                ++display.op2_len;
            }
            break;
        case DS_OP2:
            // op1 = result
            // op2 = key
            // state = op1
            break;
        case DS_ERR:
            break;
    }
}

/*..........................................................................*/
void BSP_negate(void) {
    BSP_clear();
    l_display[DISP_WIDTH - 2] = '-';
    display.op1[DISP_WIDTH - 2] = '-';
}

/*..........................................................................*/
void BSP_display(void) {
    printf("\n[%s] ", l_display);

    char lcd_display_top[LCD_WIDTH];
    char lcd_display_bot[LCD_WIDTH];

    switch (display_state) {
        case DS_CLEAR:
            // top = op1
            // bot = '= ' + result
            break;
        case DS_OP1:
        case DS_OP2:
            // top = op1 + op + op2
            // bot = '= ' + result
            break;
        case DS_ERR:
            // top = op1
            // bot = clear
            break;
    }

    BSP_LCD_display_str(lcd_display_top, 0, -1);
    BSP_LCD_display_str(lcd_display_bot, 1, -1);
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
        case KEY_PLUS:
            result = operand1 + operand2;
            break;
        case KEY_MINUS:
            result = operand1 - operand2;
            break;
        case KEY_MULT:
            result = operand1 * operand2;
            break;
        case KEY_DIVIDE:
            if ((operand2 < -1e-30) || (1e-30 < operand2)) {
                result = operand1 / operand2;
            } else {
                BSP_err(" Error 0 ");        /* error: divide by zero */
                ok = 0;
            }
            break;
    }
    if (ok) {
        if ((-0.000001 < result) && (result < 0.000001)) {
            result = 0.0;
        }
        if ((-99999999.0 < result) && (result < 99999999.0)) {
            sprintf(l_display, "%9.6g", result);
            sprintf(display.result, "%9.6g", result);
        } else {
            BSP_err(" Error 1 ");          /* error: out of range */
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

int _kbhit(void) {
    /*int i;
      i = 0xFF;
      return(i);*/
    return(0xFF);
}

int _getch() {
    FILE *f;
    return(fgetc(f));
}

#ifdef BSP
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    _sys_exit(-1);
}

#endif     
void BSP_LCD_display_str( char * msg, int line, int pos) {
    set_cursor (pos,line);
    lcd_print ( (unsigned char const*) msg);

    printf("%s",msg);
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

void QF_onCleanup(void) {
    ;
}

void QF_onIdle(void) {
    ;
}

void QF_onStartup(void) {
    ;
}
