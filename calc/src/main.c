/*****************************************************************************
* Model: calc.qm
* File:  .\src/main.c
*
* This code has been generated by QM tool (see state-machine.com/qm).
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*****************************************************************************/
/* @(/1/2/0) ...............................................................*/
/*****************************************************************************
* Model: calc.qm
* File:  .\src/main.c
*
* This code has been generated by QM tool (see state-machine.com/qm).
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*****************************************************************************/
/* @(/1/3/0) ...............................................................*/
/*****************************************************************************
* Product: Calculator Example with inheritance of whole state model
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

#include "qp_port.h"                        /* the port of the QP framework */
#include "bsp.h"                                   /* board support package */

#include <stdio.h>
#include <stdlib.h>

#define SIZE_OF_EVENT_QUEUE 128

QEvt const *l_eventQueue[SIZE_OF_EVENT_QUEUE];    /* Event queue storage for print time */


// Local objects -----------------------------------------------------------*/
static Calc l_calc;                             /* instantiate Calculator2 */


//..........................................................................*/
int main() {
    QF_init();    /* initialize the framework and the underlying RT kernel */

    Calc_ctor(&l_calc);    /* explicitly instantiate the calculator object */
    QF_poolInit((void *) &l_eventQueue, sizeof(l_eventQueue), sizeof(l_eventQueue[0]));
    QActive_start((QActive * ) & l_calc, 1, l_eventQueue, Q_DIM(l_eventQueue), (void *) 0, 0, (QEvt *) 0);
    BSP_Init(&l_calc);

    printf("Calculator example, QEP version: %s\n"
           "Press '0' .. '9'     to enter a digit\n"
           "Press '.'            to enter the decimal point\n"
           "Press '+'            to add\n"
           "Press '-'            to subtract or negate a number\n"
           "Press '*'            to multiply\n"
           "Press '/'            to divide\n"
           "Press '=' or <Enter> to get the result\n"
           "Press 'c' or 'C'     to Cancel\n"
           "Press 'e' or 'E'     to Cancel Entry\n"
           "Press <Esc>          to quit.\n\n",
           QEP_getVersion());

        CalcEvt e;                                      /* Calculator event */

