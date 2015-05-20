/******************************************************************************/
/* BLINKY.C: LED Flasher                                                      */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2006 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/
                  
//#include <stdio.h>
#include <LPC23xx.H>                    /* LPC23xx definitions                */
#include "LCD.h"                        /* Graphic LCD function prototypes    */
#include "Blinky.h"
#include <stdint.h>


/* Function that initializes LEDs                                             */
void LED_Init(void) {
  PINSEL10 = 0;                         /* Disable ETM interface, enable LEDs */
  FIO2DIR  = 0x000000FF;                /* P2.0..7 defined as Outputs         */
  FIO2MASK = 0x00000000;
}

/* Function that turns on requested LED                                       */
void LED_On (unsigned int num) {
  FIO2SET = (1 << num);
}

/* Function that turns off requested LED                                      */
void LED_Off (unsigned int num) {
  FIO2CLR = (1 << num);
}

/* Function that outputs value to LEDs                                        */
void LED_Out(unsigned int value) {
  FIO2CLR = 0xFF;                       /* Turn off all LEDs                  */
  FIO2SET = (value & 0xFF);             /* Turn on requested LEDs             */
}


/* Function for displaying bargraph on the LCD display                        */
void Disp_Bargraph(int pos_x, int pos_y, int value) {
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

/* Import external IRQ handlers from IRQ.c file                               */
extern __irq void T0_IRQHandler  (void);
extern __irq void ADC_IRQHandler (void);

/* Import external functions from Serial.c file                               */
extern       void init_serial    (void);

void Init_Display() {
	int i;
	LED_Init();                           /* LED Initialization                 */

  /* Enable and setup timer interrupt, start timer                            */
  T0MR0         = 11999;                       /* 1msec = 12000-1 at 12.0 MHz */
  T0MCR         = 3;                           /* Interrupt and Reset on MR0  */
  T0TCR         = 1;                           /* Timer0 Enable               */
  //VICVectAddr4  = (unsigned long)T0_IRQHandler;/* Set Interrupt Vector        */
  VICVectCntl4  = 15;                          /* use it for Timer0 Interrupt */
  VICIntEnable  = (1  << 4);                   /* Enable Timer0 Interrupt     */

  /* Power enable, Setup pin, enable and setup AD converter interrupt         */
  PCONP        |= (1 << 12);                   /* Enable power to AD block    */
  PINSEL1       = 0x4000;                      /* AD0.0 pin function select   */
  AD0INTEN      = (1 <<  0);                   /* CH0 enable interrupt        */
  AD0CR         = 0x00200301;                  /* Power up, PCLK/4, sel AD0.0 */
  //VICVectAddr18 = (unsigned long)ADC_IRQHandler;/* Set Interrupt Vector       */
  VICVectCntl18 = 14;                          /* use it for ADC Interrupt    */
  VICIntEnable  = (1  << 18);                  /* Enable ADC Interrupt        */

  init_serial();                               /* Init UART                   */

  for (i = 0; i < 20000000; i++);       /* Wait for initial display           */
	
	lcd_init();
  lcd_clear();
  lcd_print ("  MCB2300 DEMO  ");
  set_cursor (0, 1);
  lcd_print ("  www.keil.com  ");
}

enum BlinkySignals {
	UP_SIG,
	DOWN_SIG,
	TICK_SIG
};

enum BlinkyStates {
	BLINK
};

typedef struct EventTag {
    uint16_t sig;
} Event;

typedef struct TickEvtTag {
    Event super;                         /* derive from the Event structure */
    uint8_t fine_time;                           /* the fine 1/10 s counter */
} TickEvt;

typedef struct BlinkyTag {
	uint8_t state;
	uint8_t value;
} Blinky;

void Blinky_ctor(Blinky *me);
void Blinky_dispatch(Blinky *me, Event const *e);

void Blinky_ctor(Blinky *me) {
	Init_Display();
	me->value = 0;
	me->state = BLINK;
}

void Blinky_dispatch(Blinky *me, Event const *e) {
	switch (e->sig) {
		case UP_SIG:
			if (me->value < 16*5)	me->value++;
			break;
		case DOWN_SIG:
			if (me->value > 0) me->value--;
			break;
		case TICK_SIG:
			Disp_Bargraph(0, 0, me->value);
			break;
	}		
}

static Blinky l_blinky;

int main (void) {
  
	Blinky_ctor(&l_blinky);
	
	printf("Blinky (Nested switch)\n"
           "Press 'a'   for UP   event\n"
           "Press 'd'   for DOWN event\n"
           "Press <Esc> to quit.\n");
	
	for(;;) {
		static TickEvt tick_evt = { TICK_SIG, 0};
		if (++tick_evt.fine_time == 10) {
            tick_evt.fine_time = 0;
        }
        printf("T(%1d)%c", tick_evt.fine_time,
                            (tick_evt.fine_time == 0) ? '\n' : ' ');

				Blinky_dispatch(&l_blinky, (Event *)&tick_evt);
				if (kbhit()) {
            static Event const up_evt   = { UP_SIG   };
            static Event const down_evt = { DOWN_SIG };
            Event const *e = (Event *)0;

            switch (_getch()) {
                case 'a': {                                     /* UP event */
                    printf("\nUP  : ");
                    e = &up_evt;                   /* generate the UP event */
                    break;
                }
                case 'd': {                                   /* DOWN event */
                    printf("\nDOWN: ");
                    e = &down_evt;               /* generate the DOWN event */
                    break;
                }
                case '\33': {                                  /* <Esc> key */
                    printf("\nESC : Bye! Bye!");
                    fflush(stdout);
                    _exit(0);
                    break;
                }
								default:
					
								
								break;
            }

            if (e != (Event *)0) {             /* keyboard event available? */
                Blinky_dispatch(&l_blinky, e);           /* dispatch the event */
            }
        }
	}
}
