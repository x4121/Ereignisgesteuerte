

enum BlinkySignals {                          /* all signals for the Bomb FSM */
    ADREADY__SIG,
    TICK_SIG,
	  BLINK_LED_LCD_SIG,
	  VT24_OUTPUT_SIG
};

enum BlinkySates {                            /* all states for the  FSM */
    PROCESSING_STATE,
    DISPLAYING_STATE
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

typedef struct BlinkyTag {                                   /* the Blink FSM */
    unsigned char state;                            				 /* the scalar state-variable */
    short AD_Value;
	  short AD_Print;
	  short AD_Old;
} Blinky;

void Blinky_ctor(Blinky *me );            								/* the "constructor" */
void Blinky_init(Blinky *me);                              /* init() function */
void Blinky_dispatch(Blinky *me, Event const *e);

                                     /* macro for taking a state transition */
#define TRAN(target_)  (me->state = (unsigned char)(target_))
