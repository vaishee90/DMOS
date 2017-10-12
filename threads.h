#include"q.h"

int thread_id = 0;
struct TCB_t *Curr_Thread, *Ready_Q;
void *stackP;

void start_thread(void (*function)(void))
{ 
	// begin pseudo code
     // allocate a stack (via malloc) of a certain size (choose 8192)
     int sizeOfStack = 8192;
     stackP = (void*)malloc(sizeOfStack);
     
     // allocate a TCB (via malloc)
     // Curr_Thread = (struct TCB_t*)malloc(sizeof(TCB_t));
     Curr_Thread = NewItem();
     
     //call init_TCB with appropriate arguments
     init_TCB(Curr_Thread, function, stackP, sizeOfStack);
     
     //Add a thread_id (use a counter)
     Curr_Thread->thread_id = thread_id++;
     
     //call addQ to add this TCB into the “Ready_Q” which is a global header pointer
     AddQueue(Ready_Q, Curr_Thread);
  	//end pseudo code
}

void run()
{   // real code
    Curr_Thread = DelQueue(Ready_Q);
    ucontext_t parent;     // get a place to store the main context, for faking
    getcontext(&parent);   // magic sauce
    swapcontext(&parent, &(Curr_Thread->context));  // start the first thread
}
 

void yield() // similar to run
{  
     TCB_t *Prev_Thread;
     AddQueue(Ready_Q, Curr_Thread); 
     Prev_Thread = Curr_Thread;
     Curr_Thread = DelQueue(Ready_Q);
     
     //swap the context, from Prev_Thread to the thread pointed to Curr_Thread
     swapcontext(&(Prev_Thread->context), &(Curr_Thread->context));
}
 
// to print thread id in main.c for testing purpose
int get_thread_id(){
    return Curr_Thread->thread_id;
}