#define PBDIR (volatile unsigned char *) 0xFFFFFFF5
#define IVECT (volatile unsigned int *) (0x20)

// DISPLAY STATE 
unsigned char count = 0;

/*interrupt*/ void intserv() {
	if(count > 9) {
		count = 0;
	} else {
		count++;
	}

	*CSTAT = 0x00;
}


//ASSIGNMENT:
//1) INCREMENT DIGIT EVERY SECOND
//2) E ENABLES TIMER, D PAUSES
//
//DONE:
//enable global interrupts
//TODO: 
//[] read input from E (start) and D (stop)
//[] configure timer
//[] set interrupt source

int main() {

	// B PORT CONFIG
	*PBDIR = 0xFF; // set port B as output
	*IVECT = (unsigned int *) &intserv; // load ISR to 0x20
	asm( "MoveControl PSR, #040" ); // PSR[IE] = 1 (enable interrupts)
	
	// TIMER CONFIG
	*CTCON = 0x02; // stop countdown if running
	*CTNM = 100000000; // count for 1 seconds	
	*CTSTAT = 0x00; // clear reached 0 flag
	
	
	while(1){
		// if E pressed
		if(1) {
			*CTCON = 0x01; // start timer
		}
		// if D pressed
		if(0) {
			*CTCON = 0x02; // stop timer
		}
		
		// UPDATE DISPLAY	
	}
}
