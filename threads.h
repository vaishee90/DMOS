#include<stdio.h>
#include<stdlib.h>
#include"q.h"

void start_thread(void (*function)(void))
{ // begin pseudo code
     allocate a stack (via malloc) of a certain size (choose 8192)
     allocate a TCB (via malloc)
     call init_TCB with appropriate arguments
     Add a thread_id (use a counter)
     call addQ to add this TCB into the “ReadyQ” which is a global header pointer
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
{  TCB_t Prev_Thread;
   AddQueue(Ready_Q, Curr_Thread); 
   Prev_Thread = Curr_Thread;
   Curr_Thread = DelQueue(ReadyQ);
   //swap the context, from Prev_Thread to the thread pointed to Curr_Thread
}