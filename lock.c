#include "lock.h"
#include "shared_structs.h"
#include <stdlib.h>
#include <fsl_device_registers.h>


/**
 * Initialises the lock structure
 *
 * @param l pointer to lock to be initialised
 */
void l_init(lock_t* l) {
	l->blocked_queue = NULL;
	l->blocked_tail = NULL;
	l->locked = 0;
}

/**
 * Grab the lock or block the process until lock is available
 *
 * @param l pointer to lock to be grabbed
 */
void l_lock(lock_t* l) {
	PIT->CHANNEL[0].TCTRL = 0x1;
	if (l->locked) {
		current_process->waiting = 1;
		if (!l->blocked_queue) {
			l->blocked_queue = current_process;
		}
		if (l->blocked_tail) {
			l->blocked_tail->next_block = current_process;
		}
		l->blocked_tail = current_process;
		current_process->next_block = NULL;
		process_blocked();
	}
	else {
		current_process->waiting = 0;
		l->locked = 1;
	}
	PIT->CHANNEL[0].TCTRL = 0x3;
}

/**
 * Release the lock along with the first process that may be waiting on
 * the lock. This ensures fairness wrt lock acquisition.
 *
 * @param l pointer to lock to be unlocked
 */
void l_unlock(lock_t* l) {
	PIT->CHANNEL[0].TCTRL = 0x1;
	if (!l->blocked_queue) {
		l->locked = 0;
	}
	else {
		//pop from block queue
		process_t *proc = l->blocked_queue;
		l->blocked_queue = proc->next_block;
		if (l->blocked_tail == proc) {
			l->blocked_tail = NULL;
		}
		proc->next_block = NULL;
		proc->waiting = 0;
		
		//push to process queue
		if (!process_queue) {
			process_queue = proc;
			proc->next = NULL;
		}
		else {
			process_t *tmp = process_queue;
			while (tmp->next) {
				tmp = tmp->next;
			}
			tmp->next = proc;
			proc->next = NULL;
		}
	}
	PIT->CHANNEL[0].TCTRL = 0x3;
}
