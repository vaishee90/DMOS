
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"threads.h"

int global = 0;

void function_1()
{
   int local = 0;

    while (1){
        //printf("XXXXPrinting from Function 1 global = %d  local = %d\n", global, local);
        //run();
        printf("Thread id = %d\n",get_thread_id());
        //sleep(1);
        global++; local ++;
        printf("Function 1 incremented .... global = %d  local = %d\n", global, local);
        yield();
        //sleep(1);
    }
}    

void function_2()
{
   int local = 0;

    while (1){
        //printf("XXXXPrinting from Function 2 global = %d  local = %d\n", global, local);
        //run();
        printf("Thread id = %d\n",get_thread_id());
        //sleep(1);
        global++; local ++;
        printf("Function 2 incremented .... global = %d  local = %d\n", global, local);
        yield();
        //sleep(1);
    }
}    

void function_3()
{
   int local = 0;

    while (1){
        //printf("XXXXPrinting from Function 3 global = %d  local = %d\n", global, local);
        //run();
        printf("Thread id = %d\n",get_thread_id());
        //sleep(1);
        global++; local ++;
        printf("Function 3 incremented .... global = %d  local = %d\n", global, local);
        yield();
        //sleep(1);
    }
}    

int main(){

	Ready_Q = newQueue();
	start_thread(function_1);
	start_thread(function_1);
	start_thread(function_1);

	run();

	while(1) sleep(1);

	return 0;
}
