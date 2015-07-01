/*****************************************************************************
* Product: BSP for Calculator Example with inheritance of whole state model
* Last Updated for Version: 4.0.01
* Date of the Last Update:  Sep 23, 2008
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2008 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#ifndef bsp_h
#define bsp_h

#define KEY_PLUS    '+'
#define KEY_MINUS   '-'
#define KEY_MULT    '*'
#define KEY_DIVIDE  '/'

void BSP_clear(void);

void BSP_negate(void);

void BSP_insert(int keyId);

double BSP_get_value(void);

int BSP_eval(double operand1, int oper, double operand2);

void BSP_exit(void);

void BSP_display(void);

void BSP_message(char const *state);

/*  bsp function references for the mcb 2300 */

void Init_Timer1(void);

void init_serial(void);

/* Init UART                   */
void uart_init_0(void);

void _sys_exit(int);

#define _exit(x)  _sys_exit(x)

/* bsp function references for standalone port of QM FSM 7 HSM */

void delay(int time);

int _kbhit(void);

int kbhit(void);

int _getch(void);

int getch(void);

void _sys_exit(int);

//void BSP_display(uint8_t timeout);

void BSP_boom(void);

void BSP_LCD_display_str(char *msg, int line, int pos);

/*  bsp function references for the mcb 2300 */

void Init_Timer1(void);

void init_serial(void);

/* Init UART                   */
void uart_init_0(void);

void _sys_exit(int);

#define _exit(x)  _sys_exit(x)

/* bsp function references for standalone port of QM FSM 7 HSM */

void delay(int time);

int _kbhit(void);

int kbhit(void);

int _getch(void);

int getch(void);

void _sys_exit(int);

void BSP_boom(void);

void BSP_LCD_display_str(char *msg, int pos, int line);

void BSP_display_str(char *msg);

void BSP_exit(void);

void BSP_Init(Calc *calc);

void BSP_onKeyboardInput(int buf);

#endif                                                             /* bsp_h */
