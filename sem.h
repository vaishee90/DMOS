#include "threads.h"

typedef struct Semaphore_t{
	struct TCB_t * SemQ;
	int counter;
} Semaphore_t;



Semaphore_t* CreateSem(int InputValue){
	Semaphore_t *new_sem;
	new_sem = (Semaphore_t*)malloc(sizeof(Semaphore_t));
	new_sem->SemQ = newQueue();
	new_sem->counter = InputValue;
	return new_sem;
}

void P(Semaphore_t *sem){
	TCB_t *ToWait_Thread;
	sem->counter--;
	if(sem->counter < 0){														//If semaphore lock not available
	
		AddQueue(sem->SemQ, Curr_Thread);										//save the current running thread to the respective semaphore queue
		ToWait_Thread = Curr_Thread;											//save the current running thread to a temporary thread varable "ToWait_Thread"
		Curr_Thread = DelQueue(Ready_Q);										//obtain a current thread from the Ready_Q, this the new thread to be run, "Curr_Thread"		
		/*if(Ready_Q->next == Ready_Q){											//If Ready_Q is empty wait till there is a thread available.
			while(Ready_Q->next == Ready_Q)
				sleep(0.5);
		}*/
		swapcontext(&(ToWait_Thread->context), &(Curr_Thread->context));		//Swap context between the thread running, "ToWait_Thread" and the thread next to 
																				//schedule "Curr_Thread".

	}
}

void V(Semaphore_t *sem){
	TCB_t *SemQ_Thread;
	sem->counter++;
	if(sem->counter <= 0){														
		SemQ_Thread = DelQueue(sem->SemQ);										//Delete a thread waiting to aquire semaphore lock from respective semQ
		AddQueue(Ready_Q, SemQ_Thread);											//Add the deleted thread to Ready_Q to get scheduled for running
	}
	yield();
}