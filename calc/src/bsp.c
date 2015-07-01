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

#define DISP_WIDTH  7
#define LCD_WIDTH  16

static char lcd_display_top[LCD_WIDTH + 1];
static char lcd_display_bot[LCD_WIDTH + 1];

static enum display_state_t display_state;

static struct display_t {
    char op1[DISP_WIDTH + 1];
    int op1_len;
    char op2[DISP_WIDTH + 1];
    int op2_len;
    char op;
    char result[LCD_WIDTH + 1];
    int result_len;
} display;

static char l_display[DISP_WIDTH + 1];            /* the calculator display */
static int l_len;                        /* number of displayed characters */
static Calc *me;

void delay(int val) {
    int i;
    for (i = 0; i < val; i++);
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

void display_state_trans(enum display_state_t state) {
	display_state = state;
}

void BSP_err(char *msg) {
    display_state = DS_ERR;
    strcpy(display.op1, msg);
    strcpy(l_display, msg);
}

void clear(void) {
    display_clear();
    BSP_clear();
    display_state_trans(DS_CLEAR);
}
/*
char get_operator(uint8_t keyId) {
	switch(keyId) {
		case 
	}
	
	#define KEY_PLUS    '+'
#define KEY_MINUS   '-'
#define KEY_MULT    '*'
#define KEY_DIVIDE  '/'
}*/

/*BSP Init for the MC2300 */
void BSP_Init(Calc *calc) {
    me = calc;
	
		Timer0_Init( );
    // Init_Timer1( );
    init_serial();                               /* Init UART                   */
		ADC_Init();
    uart_init_0();
    lcd_init();
	
	  memset(lcd_display_top, ' ', LCD_WIDTH);
    lcd_display_top[LCD_WIDTH] = '\0';
	
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
        l_display[DISP_WIDTH - 1] = (char) keyId;
        ++l_len;
    } else if (l_len < (DISP_WIDTH - 1)) {
        memmove(&l_display[0], &l_display[1], DISP_WIDTH - 1);
        l_display[DISP_WIDTH - 1] = (char) keyId;
        ++l_len;
    }

    switch (display_state) {
        case DS_CLEAR:
            if (display.op1_len < DISP_WIDTH - 1) {
                if (display.op1_len != 0)
                    memmove(&display.op1[0], &display.op1[1], DISP_WIDTH - 1);
                display.op1[DISP_WIDTH - 1] = (char) keyId;
                ++display.op1_len;
            }
            break;
        case DS_OP:
            if (display.op2_len < DISP_WIDTH - 1) {
                if (display.op2_len != 0)
                    memmove(&display.op2[0], &display.op2[1], DISP_WIDTH - 1);
                display.op2[DISP_WIDTH - 1] = (char) keyId;
                ++display.op2_len;
            }
            break;
        case DS_OP2:
					memmove(display.op1, display.result, DISP_WIDTH - 1);
					display.op1_len = display.result_len;
				  // fallthrough
				case DS_OP1:
					display.op = (char) keyId;//get_operator(keyId);
					display_state = DS_OP;
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
    char lcd_display_top[LCD_WIDTH];
    char lcd_display_bot[LCD_WIDTH];
    printf("\n[%s] ", l_display);
    switch (display_state) {
        case DS_ERR:
						memmove(&lcd_display_top[0], &display.op1[0], DISP_WIDTH);
            break;
				default:
						memmove(lcd_display_top, display.op1, DISP_WIDTH);
						lcd_display_top[DISP_WIDTH + 1] = display.op;
						memmove(&lcd_display_top[DISP_WIDTH + 3], display.op2, DISP_WIDTH);
						lcd_display_bot[0] = '=';
						lcd_display_bot[1] = ' ';
						memmove(&lcd_display_bot[2], display.result, LCD_WIDTH);
						break;
    }

    BSP_LCD_display_str(lcd_display_top, 0, -1);
    BSP_LCD_display_str(lcd_display_bot, 1, -1);
}

/*..........................................................................*/
double BSP_get_value(void) {
    return strtod(l_display, (char **) 0);
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
void Q_onAssert(char const *const file, int line) {
    printf("Assertion failed in %s, line %d", file, line);
    fflush(stdout);
    _sys_exit(-1);
}

int _kbhit(void) {
    /*int i;
      i = 0xFF;
      return(i);*/
    return (0xFF);
}

int _getch() {
    FILE *f;
    return (fgetc(f));
}

#ifdef BSP
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    _sys_exit(-1);
}
#endif

void BSP_LCD_display_str(char *msg, int line, int pos) {
    set_cursor(pos, line);
    lcd_print((unsigned char const *) msg);

    printf("%s", msg);
};

/*..........................................................................*/
void BSP_display_str(char *msg) {
    // printf("%S", msg);
    BSP_LCD_display_str(msg, 0, 1);

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

void BSP_onKeyboardInput(int buf) {
    CalcEvt e;                                      /* Calculator event */
    e.key_code = (uint8_t) buf;             /* get a char with echo */
    if (e.key_code != 0) {
        switch (e.key_code) {
            case 'c':                         /* intentionally fall through */
            case 'C': {
                ((QEvent * ) & e)->sig = C_SIG;
                break;
            }
            case 'e':                         /* intentionally fall through */
            case 'E': {
                ((QEvent * ) & e)->sig = CE_SIG;
                break;
            }
            case '0': {
                ((QEvent * ) & e)->sig = DIGIT_0_SIG;
                break;
            }
            case '1':                         /* intentionally fall through */
            case '2':                         /* intentionally fall through */
            case '3':                         /* intentionally fall through */
            case '4':                         /* intentionally fall through */
            case '5':                         /* intentionally fall through */
            case '6':                         /* intentionally fall through */
            case '7':                         /* intentionally fall through */
            case '8':                         /* intentionally fall through */
            case '9': {
                ((QEvent * ) & e)->sig = DIGIT_1_9_SIG;
                break;
            }
            case '.': {
                ((QEvent * ) & e)->sig = POINT_SIG;
                break;
            }
            case '+':                         /* intentionally fall through */
            case '-':                         /* intentionally fall through */
            case '*':                         /* intentionally fall through */
            case '/': {
                ((QEvent * ) & e)->sig = OPER_SIG;
                break;
            }
            case '%': {                              /* new event for Calc2 */
                ((QEvent * ) & e)->sig = PERCENT_SIG;
                break;
            }
            case '=':                         /* intentionally fall through */
            case '\n':
            case '\r': {                                       /* Enter key */
                ((QEvent * ) & e)->sig = EQUALS_SIG;
                break;
            }
            case '\33': {                                        /* ESC key */
                ((QEvent * ) & e)->sig = OFF_SIG;
                break;
            }
            default: {
                ((QEvent * ) & e)->sig = 0;                   /* invalid event */
                break;
            }
        }

        if (me && ((QEvent * ) & e)->sig != 0) {           /* valid event generated? */
            QActive_postFIFO((QActive *)me, (QEvent * ) & e); /* dispatch event */
            BSP_display();                                  /* show the display */
            printf(": ");
        }
    }
}

void BSP_CE(void) {
    CalcEvt e;
    ((QEvent * ) & e)->sig = C_SIG;
    if (me && ((QEvent * ) & e)->sig != 0) {           /* valid event generated? */
        QActive_postFIFO((QActive *) me, (QEvent * ) & e); /* dispatch event */
        BSP_display();                                  /* show the display */
        printf(": ");
    }
}

void BSP_C(void) {
    CalcEvt e;
    ((QEvent * ) & e)->sig = CE_SIG;
    if (me && ((QEvent * ) & e)->sig != 0) {           /* valid event generated? */
        QActive_postFIFO((QActive *) me, (QEvent * ) & e); /* dispatch event */
        BSP_display();                                  /* show the display */
        printf(": ");
    }
}
