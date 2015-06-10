#include "qp_port.h"                /* the port of the QEP event processor */

#include "bombqmbsp.h"
#include "base_ao.h"
#include <stdio.h>
#include <stdlib.h>   

#define SIZE_OF_EVENT_QUEUE  128
				   
void BaseAO_ctor(BaseAO *me);

static QState BaseAO_initial(BaseAO * me, QEvt const *e);
static QState BaseAO_processing(BaseAO *me, QEvt const *e);
static QState BaseAO_idle(BaseAO *me, QEvt const *e);
static QState BaseAO_displaying(BaseAO *me, QEvt const *e);
static BaseAO  l_BaseAO;    /* the Base active objec----------------------------------------------*/
static BaseTimEvt BaseTimer;  
                                 
 		  
QActive * const Base  = (QActive *) &l_BaseAO;

QEvt const *l_PrintEvtQSto[ SIZE_OF_EVENT_QUEUE];    /* Event queue storage for print time */

/*..........................................................................*/
void BaseAO_ctor(BaseAO *me) {
  
    QActive_ctor(&me->super, (QStateHandler)&BaseAO_initial);
    QTimeEvt_ctor(&BaseTimer.super, BASE_TIME_SIG);
}

QState BaseAO_initial(BaseAO *me, QEvt const *e) {
    (void)e;               /* avoid compiler warning about unused parameter */
    return Q_TRAN(&BaseAO_idle);
}


QState BaseAO_processing (BaseAO * me, 	  QEvt const *e) {
           (void)me;              /* avoid compiler warning about unused parameter */
           (void)e;               /* avoid compiler warning about unused parameter */
		 
	 	   switch ( e->sig ) {

		   case Q_INIT_SIG: {
 	      
		  	printf("processing-INIT;\r\n");
			BaseTimer.clk_ctr=0;
			QTimeEvt_postEvery(&BaseTimer.super, (QActive *)me, 0x10);
			return Q_HANDLED();
  			}

		   	case Q_ENTRY_SIG: {
		    printf("processing-ENTRY;\r\n");
			   return Q_HANDLED();
			}
		   
		   case BASE_TIME_SIG: {


			  ((BaseTimEvt *)e)->clk_ctr ++;
				 
   		  if (  ((BaseTimEvt *)e)->clk_ctr >= 10 ){
			  		 static QEvt const SerDisplayEvt = { SER_DISPLAY_SIG, 0};
			         ((BaseTimEvt *)e)->clk_ctr =0;
			

 		         QActive_postFIFO((QActive *)me, &SerDisplayEvt);
				 }
 
			   	return Q_HANDLED();

		   }
	  
		   case Q_EXIT_SIG: {
   				
 			      printf("processing-EXIT;\r\n");
		      	
		   }
		   	  

	}
 
	 return Q_SUPER(&BaseAO_displaying);
	
}
QState BaseAO_displaying (BaseAO *me, QEvt const *e) {
 	 	   switch ( e->sig ) {

	 		case Q_INIT_SIG: {  

				 return Q_TRAN(&BaseAO_processing);
		   	    
   	
  			}
	      
		   	case Q_ENTRY_SIG: {
			  	 printf("display-ENTRY;\r\n");
			   return Q_HANDLED();
			}
		   
		   case  SER_DISPLAY_SIG: { 
				 printf("Serial Display Signal Processed\r\n");
			  return Q_HANDLED();
		   
		   
		    
		   }
	 	   case Q_EXIT_SIG: {
			  	 printf("display-EXIT;\r\n");	  	   
			}
	
		 	
	}

 
	 return Q_SUPER(&BaseAO_idle);

}

 QState BaseAO_idle(BaseAO *me, QEvt const *e) {
    switch (e->sig) {
   
    	case Q_INIT_SIG: {
  			
			 printf("idle-INIT;\r\n");
                return Q_TRAN(&BaseAO_displaying);
      }
	
        case Q_ENTRY_SIG: {
            printf("idle-ENTRY;\r\n");

            return Q_HANDLED();
        }
       
    }
    return Q_SUPER(&QHsm_top);
}

int main ()	{


            BSP_Init();
			BSP_LCD_display_str("Base ActiveObj",0,0);	  
			BSP_LCD_display_str("EventDrivenSys",0,1);

 			QF_init();    /* initialize the framework and the underlying RT kernel */

    			                    /* instantiate and start the active objects... */
  			BaseAO_ctor(&l_BaseAO);
				QF_poolInit( (void*) &l_PrintEvtQSto, sizeof(l_PrintEvtQSto), sizeof (BasePrintEvt));
    		QActive_start((QActive *)&l_BaseAO, 1, l_PrintEvtQSto, Q_DIM(l_PrintEvtQSto),
                  (void *)0, 0, (QEvt *)0);

	
             QF_run();                                     /* run the QF application */

             return(0);
}

