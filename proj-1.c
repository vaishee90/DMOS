#include <stdio.h>
#include <stdlib.h>
#include "sem.h"

#define N 3
semaphore_t mutex, full, empty;
int arr[N];
int flag = 0;
int child_id[3] = {0, 1, 2};

void child (int *arg)
{
    while (1){ 
	P(&empty);
	P(&mutex);
    	printf("Child %d incrementing content of %d element in array...\n", *arg, *arg);
    	arr[*arg]++;
	V(&mutex);
    	V(&full);
	sleep(1);
    }
}
  

int main()
{
    init_sem(&mutex, 1);
    init_sem(&full, 0);
    init_sem(&empty, N);

    while (1){ 
    	if(flag == 0){
    		arr[0] = 0;
    		arr[1] = 0;
    		arr[2] = 0;
    		printf("Parent creating children...\n");
		start_thread(child, &child_id[0]);
    		start_thread(child, &child_id[1]);
    		start_thread(child, &child_id[2]);
    	}
	if(flag == 1){
		P(&full);
		P(&full);
		P(&full);
		P(&mutex);
	   	printf("Parent printing values for children\n");
		printf("Array now holds: %d, %d, %d\n", arr[0], arr[1], arr[2]);
		V(&mutex);
		V(&empty);
		V(&empty);
		V(&empty);
	}
	flag = 1;
	sleep(1);
    }

    while(1) sleep(1);
}
