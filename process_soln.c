#include "3140_concur.h"
#include <fsl_device_registers.h>
#include "shared_structs.h"

/* the currently running process. current_process must be NULL if no process is running,
    otherwise it must point to the process_t of the currently running process
*/
typedef struct process_state process_t;

process_t * current_process = NULL; 
process_t * process_queue = NULL;
process_t * process_tail = NULL;

<<<<<<< HEAD
process_t * blocked_queue = NULL;
process_t * blocked_tail = NULL;

static void push_tail_block(process_t *proc) {
	if (!blocked_queue) {
		blocked_queue = proc;
		current_process->next_block = proc;
=======
static void push_tail_blocked(process_t *proc) {
	if (!proc->lock->blocked_queue) {
		proc->lock->blocked_queue = proc;
>>>>>>> e87b20ddff584d9b101bf3c6e3057c38ebcc50ae
	}
	if (proc->lock->blocked_tail) {
		proc->lock->blocked_tail->next_block = proc;
	}
	proc->lock->blocked_tail = proc;
	proc->next_block = NULL;
}

static process_t * pop_front_blocked(lock_t *l) {
	if (!l->blocked_queue) return NULL;
	process_t *proc = l->blocked_queue;
	l->blocked_queue = proc->next_block;
	if (l->blocked_tail == proc) {
		l->blocked_tail = NULL;
	}
	proc->next_block = NULL;
	proc->lock = NULL;
	proc->waiting = 0;
	return proc;
}

static process_t * pop_front_process() {
	if (!process_queue) return NULL;
	process_t *proc = process_queue;
	process_queue = proc->next;
	if (process_tail == proc) {
		process_tail = NULL;
	}
	proc->next = NULL;
	return proc;
}

static void push_tail_process(process_t *proc) {
	if (!process_queue) {
		process_queue = proc;
	}
	if (process_tail) {
		process_tail->next = proc;
	}
	process_tail = proc;
	proc->next = NULL;
}

static void process_free(process_t *proc) {
	process_stack_free(proc->orig_sp, proc->n);
	free(proc);
}


/* Called by the runtime system to select another process.
   "cursp" = the stack pointer for the currently running process
*/
unsigned int * process_select (unsigned int * cursp) {
	if (cursp) {
		if (current_process->waiting) {
			current_process->sp = cursp;
<<<<<<< HEAD
			push_tail_block(current_process);
			current_process = pop_front_process();
		}
		else if (blocked_queue) {
			current_process->sp = cursp;
			push_tail_process(current_process);
			process_t *tmp = pop_blocked_process();
			tmp->waiting = 0;
			push_tail_process(tmp);
=======
			push_tail_blocked(current_process);
>>>>>>> e87b20ddff584d9b101bf3c6e3057c38ebcc50ae
		}
		else {
			// Suspending a process which has not yet finished, save state and make it the tail
			current_process->sp = cursp;
			push_tail_process(current_process);
		}
	} 
	else {
		// Check if a process was running, free its resources if one just finished
		if (current_process) {
			process_free(current_process);
		}
	}
	
	// Select the new current process from the front of the queue
	current_process = pop_front_process();
	
	if (current_process) {
		// Launch the process which was just popped off the queue
		return current_process->sp;
	} else {
		// No process was selected, exit the scheduler
		return NULL;
	}
}

/* Starts up the concurrent execution */
void process_start (void) {
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	PIT->MCR = 0;
	PIT->CHANNEL[0].LDVAL = DEFAULT_SYSTEM_CLOCK / 10;
	NVIC_EnableIRQ(PIT0_IRQn);
	// Don't enable the timer yet. The scheduler will do so itself
	
	// Bail out fast if no processes were ever created
	if (!process_queue) return;
	process_begin();
}

/* Create a new process */
int process_create (void (*f)(void), int n) {
	unsigned int *sp = process_stack_init(f, n);
	if (!sp) return -1;
	
	process_t *proc = (process_t*) malloc(sizeof(process_t));
	if (!proc) {
		process_stack_free(sp, n);
		return -1;
	}
	
	proc->sp = proc->orig_sp = sp;
	proc->n = n;
	
	push_tail_process(proc);
	return 0;
}
