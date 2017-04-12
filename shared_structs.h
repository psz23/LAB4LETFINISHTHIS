#ifndef __SHARED_STRUCTS_H__
#define __SHARED_STRUCTS_H__

/** Implement your structs here */

/**
 * This structure holds the process structure information
 */
typedef struct process_state {
	unsigned int *sp;
	unsigned int *orig_sp;
	int waiting;
	int n;
	struct process_state *next;
	struct process_state *next_block;
} process_t;

/**
 * This defines the lock structure
 */
typedef struct lock_state {
	int locked;
	process_t *blocked_queue;
	process_t *blocked_tail;
} lock_t;

/**
 * This defines the conditional variable structure
 */
typedef struct cond_var {
	int ready;
} cond_t;

#endif