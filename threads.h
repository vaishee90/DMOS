#include"q.h"

int loc_thread_id = 0;

void start_thread(void (*function)(void))
{ // begin pseudo code
     struct TCB_t *tcb;
	int sizeOfStack = 8192
     
     // allocate a stack (via malloc) of a certain size (choose 8192)
     (void) *stackP = (void *)malloc(sizeOfStack);
     
     // allocate a TCB (via malloc)
     tcb = (struct TCB_t*)malloc(sizeof(struct TCB_t));
     
     //call init_TCB with appropriate arguments
     init_TCB(tcb, function, stackP, sizeOfStack);
     
     //Add a thread_id (use a counter)
     tcb->thread_id = loc_thread_id++;
     
     //call addQ to add this TCB into the “ReadyQ” which is a global header pointer
     AddQueue(ReadyQ, tcb)
  //end pseudo code
}

void run()
{   // real code
    Curr_Thread = DelQueue(ReadyQ)
    ucontext_t parent;     // get a place to store the main context, for faking
    getcontext(&parent);   // magic sauce
    swapcontext(&parent, &(Curr_Thread->conext));  // start the first thread
}
 

void yield() // similar to run
{  
     TCB_t *Prev_Thread;
     AddQueue(Ready_Q, Curr_Thread); 
     Prev_Thread = Curr_Thread;
     Curr_Thread = DelQueue(ReadyQ);
     
     //swap the context, from Prev_Thread to the thread pointed to Curr_Thread
     swapcontext(&(Prev-Thread->context), &(Curr_Thread->conext));
}
