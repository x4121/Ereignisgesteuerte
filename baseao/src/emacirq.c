#include "EMAC.h"
#include "tcpip.h"
#include <LPC23xx.h>
#include "uip_ao.h"


//static EMAC_Event emac, set this to receive since we only react only on receive
static Ethernet_Event ethernet_event = { ETHERNET_RECEIVE_SIG};
static EMAC_Event EmacEvt   = {ETHERNET_RECEIVE_SIG };
extern QActive * const ethernetAO;


static unsigned int emac_irq_status;
void EMACHandler(void) __irq                

{
	
	  unsigned char p;
	  emac_irq_status=  MAC_STATUS;
	  EmacEvt.type = MAC_STATUS;
#ifdef QK
 	__disable_irq(); 
  	  ++QK_intNest_; 
#endif  
	QF_INT_UNLOCK();
	
 //  Todo BNA -- buffer overflow handeling!!!!!!
   QActive_postFIFO(ethernetAO, (QEvent *)&ethernet_event);
QF_INT_LOCK(); 
  MAC_INTCLEAR  = 0xFFFF;												 // Clear interrupt flag
  VICVectAddr = 0;                               // Acknowledge Interrupt


#ifdef QK 
	   --QK_intNest_;
	   if (   QK_intNest_ ==0) 
	   	//	QK_schedule_();
		   p = QK_schedPrio_(); 
        if (p != (uint8_t)0) { 
            QK_schedExt_(p); 
        } 
#endif

}


void EMAC_InitIRQ(void) {
	
		MAC_INTENABLE = INT_RX_DONE | INT_TX_DONE;
		VICVectAddr21= (unsigned int)EMACHandler;
		VICVectCntl21 = 10;
		VICIntEnable =  ( 0x00000001 << 21);
	
}

