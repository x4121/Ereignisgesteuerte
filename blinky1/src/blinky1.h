#ifndef blinky_h
#define blinky_h

enum BlinkySignals {                          /* all signals for the Bomb FSM */
    AD_READY_SIG,
    TICK_SIG,
	  BLINK_LED_LCD_SIG,
	  VT24_OUTPUT_SIG,
	
	  LENGTH_SIG
};

enum BlinkyStates {                            /* all states for the  FSM */
    PROCESSING_STATE,
    DISPLAYING_STATE,
	
	  LENGTH_STATE
};

typedef struct EventTag {
    unsigned short  sig;                                    /* signal of the event */
						/* add event parameters by derivation from the Event structure... */
} Event;


typedef struct AD_EventTag {
	
	Event super;
	short AD_Last;
	
} ADEvt;
	
		
typedef struct TickEvtTag {
    Event super;                         										/* derive from the Event structure */
    unsigned char  fine_time;                          			/* the fine 1  s counter */
} TickEvt;

                                     /* macro for taking a state transition */
// #define TRAN(target_)  (me->state = (unsigned char)(target_))

#endif