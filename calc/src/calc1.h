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
#ifndef calc1_h
#define calc1_h

enum Calc1Signals {
    C_SIG = Q_USER_SIG,
    CE_SIG,
    DIGIT_0_SIG,
    DIGIT_1_9_SIG,
    POINT_SIG,
    OPER_SIG,
    EQUALS_SIG,
    OFF_SIG,

    MAX_CALC1_SIG
};

typedef struct CalcEvtTag {
    QEvent  super;                                    /* derive from QEvent */
    uint8_t key_code;                                    /* code of the key */
} CalcEvt;


// Calculator HSM class for inheritance ------------------------------------*/
typedef struct Calc1Tag {
    QHsm    super;                                      /* derive from QHsm */
    double  operand1;                             /* the value of operand 1 */
    uint8_t operator;                               /* operator key entered */
} Calc1;

void   Calc1_ctor   (Calc1 *me);                                    /* ctor */
QState Calc1_initial(Calc1 *me, QEvent const *e);    /* initial pseudostate */

QState Calc1_on       (Calc1 *me, QEvent const *e);
QState Calc1_error    (Calc1 *me, QEvent const *e);
QState Calc1_ready    (Calc1 *me, QEvent const *e);
QState Calc1_result   (Calc1 *me, QEvent const *e);
QState Calc1_begin    (Calc1 *me, QEvent const *e);
QState Calc1_negated1 (Calc1 *me, QEvent const *e);
QState Calc1_operand1 (Calc1 *me, QEvent const *e);
QState Calc1_zero1    (Calc1 *me, QEvent const *e);
QState Calc1_int1     (Calc1 *me, QEvent const *e);
QState Calc1_frac1    (Calc1 *me, QEvent const *e);
QState Calc1_opEntered(Calc1 *me, QEvent const *e);
QState Calc1_negated2 (Calc1 *me, QEvent const *e);
QState Calc1_operand2 (Calc1 *me, QEvent const *e);
QState Calc1_zero2    (Calc1 *me, QEvent const *e);
QState Calc1_int2     (Calc1 *me, QEvent const *e);
QState Calc1_frac2    (Calc1 *me, QEvent const *e);
QState Calc1_final    (Calc1 *me, QEvent const *e);

extern QStateHandler  Calc1_state_on;
extern QStateHandler  Calc1_state_error;
extern QStateHandler  Calc1_state_ready;
extern QStateHandler  Calc1_state_result;
extern QStateHandler  Calc1_state_begin;
extern QStateHandler  Calc1_state_negated1;
extern QStateHandler  Calc1_state_operand1;
extern QStateHandler  Calc1_state_zero1;
extern QStateHandler  Calc1_state_int1;
extern QStateHandler  Calc1_state_frac1;
extern QStateHandler  Calc1_state_opEntered;
extern QStateHandler  Calc1_state_negated2;
extern QStateHandler  Calc1_state_operand2;
extern QStateHandler  Calc1_state_zero2;
extern QStateHandler  Calc1_state_int2;
extern QStateHandler  Calc1_state_frac2;
extern QStateHandler  Calc1_state_final;

#endif                                                           /* calc1_h */
