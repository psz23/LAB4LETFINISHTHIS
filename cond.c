#include "lock.h"
#include "cond.h"
#include "shared_structs.h"
#include <stdlib.h>
#include <fsl_device_registers.h>

/**
 * Initialises the conditional variable structure
 *
 * @param l ignored
 * @param c pointer to conditional variable to be initialised
 */
void c_init(lock_t* l, cond_t* c){
	c->ready = 0;
}


/**
 * wait until condition is true
 put process in waiting queue and block it
 *
 * @param l pointer to lock/mutex for conditional variable
 * @param c pointer to conditional variable
 */
void c_wait(lock_t* l, cond_t* c){
	while(!c->ready);
	l_lock(l);
}


/**
 * Check if processes are waiting on conditional variable
 *
 * @param l Pointer to lock/mutex. Not used, but mutex must be acquired
 * before calling to ensure atomicity
 * @param c pointer to conditional variable
 *
 * @return 0 if no processes waiting
 * @return 1 if processes waiting
 */
int c_waiting(lock_t* l, cond_t* c){
	if (l->locked){
		
	}
}


/**
 * Signal that condition is met
 first process in waiting queue is scheduled
 *
 * @warning Will misbehave if no processes are waiting and is signalled
 * 
 * @param l pointer to lock (is released after signalling)
 * @param c pointer to conditional variable
 */
void c_signal(lock_t* l, cond_t* c){
	c->ready = 1;
	
	l_unlock(l);
}

#endif /* __LOCK_H_INCLUDED */
