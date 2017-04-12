#include "3140_concur.h"
#include "utils.h"
#include "lock.h"

// process running that doesn't need locks side by side with process that does

lock_t a;

void p1 (){
	int i;
	int j;
	for (i=0; i < 10; i++) {
		delay();
		LEDBlue_Toggle();
	}
}

void p (){
	int i;
	int j;
	for (i=0; i < 5; i++) {
		delay();
		LEDRed_Toggle();
	}
}

void p2 (){
	delay();
	l_lock(&a);
	p();
	l_unlock(&a);
	delay();

}

int main(void){
	LED_Initialize();

	l_init (&a);
	
	if (process_create (p1,20) < 0) {
	 	return -1;
	}
	if (process_create (p2,20) < 0) {
	 	return -1;
	}
	
	process_start();
  LEDGreen_On();

	while(1);
	return 0;	
}
