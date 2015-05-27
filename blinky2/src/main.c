/*****************************************************************************
* Model: blinky.qm
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
/* @(/2/2/2) ...............................................................*/
#include <stdio.h>
#include <LPC23xx.H>                    /* LPC23xx definitions                */
#include "blinky2.h"

#include "bsp\bspblinky.h"

static Blinky  l_blinky;

int main(void ) {

    BSP_Init();

    Blinky_ctor(&l_blinky);
    printf("Blinky as a FSM - Nested Switched Case  Example\n\r");
    QFsm_init_((QFsm *)&l_blinky, (QEvt *)0);     /* take the initial transition */
    while(1) {

        static ADEvt ad_evt =         { AD_READY_SIG, 0 };
        static QEvt lcd_evt =         { BLINK_LED_LCD_SIG };
        static QEvt v24_evt =         { VT24_OUTPUT_SIG };
        static QEvt tick_evt =        { TICK_SIG };

        l_blinky.AD_Old = l_blinky.AD_Value;
        l_blinky.AD_Value = AD_Get_Val();
        QFsm_dispatch_((QFsm *)&l_blinky, (QEvt *)&ad_evt);

     if (FSM_Get_Evt(BLINK_LED_LCD_SIG)) {
        QFsm_dispatch_((QFsm *)&l_blinky, (QEvt *)&lcd_evt);
     }

     if (Get_Clock_LS ()) {
        QFsm_dispatch_((QFsm *)&l_blinky, (QEvt *)&tick_evt);
     }

     if (FSM_Get_Evt(VT24_OUTPUT_SIG)) {
        QFsm_dispatch_((QFsm *)&l_blinky, (QEvt *)&v24_evt);
     }

   }

}
