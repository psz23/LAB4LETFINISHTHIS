#include "3140_concur.h"
#include "utils.h"
#include "lock.h"


// deadlock
// nested locks

lock_t a;
lock_t b;

void p1 (){
	delay();
	l_lock(&a);
	l_lock(&b);
	LEDRed_Toggle();
	LEDRed_Toggle();
	l_unlock(&b);
	l_unlock(&a);
	delay();
}

void p2 (){
	delay();
	l_lock(&b);
	l_lock(&a);
	LEDBlue_Toggle();
	LEDBlue_Toggle();
	l_unlock(&a);
	l_unlock(&b);
	delay();
}

int main(void){
	LED_Initialize();

	l_init(&a);
	l_init(&b);
	
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
