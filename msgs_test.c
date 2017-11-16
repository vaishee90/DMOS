#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include "msgs.h"


static int global_message_value = 0;
Semaphore_t *message_lock;

void server(){
	int i, local_message_value;
	int server_port_number;
	int client_port_number;

	printf("Enterd server function\n");

	message* server_send;
	message* server_recv;

	server_port_number = 0;

	
	while(1){
		sleep(1);
		printf("Inside server while loop\n");
		server_recv = receive(server_port_number);
		printf("Inside server received msg\n");

		server_send = (message*)malloc(sizeof(message));
		server_send->sender_port_number = server_port_number;
		
		P(message_lock);
		local_message_value = ++global_message_value;
		V(message_lock);

		for(i=0;i<10;i++){
			server_send->m_array[i] = local_message_value;
		}

		printf("Inside server sending msg\n");
		client_port_number = server_recv->sender_port_number; 
		send(client_port_number,server_send);
		printf("***Server with port number %d received message value %d and sent message value %d to client on port number %d***\n",server_port_number,
																																	server_recv->m_array[0],
																																	local_message_value,
																																	client_port_number);
		
		//sleep(1);
	}
}

void client(){
	int i, local_message_value;
	int server_port_number;
	int client_port_number;

	printf("Enterd client function\n");

	message* client_send;
	message* client_recv;
	//client_recv.sender_port_number = server_port_number	//message received by client will have server's port number

	client_port_number  = rand() % 97 + 2;					//Getting random port number for client

	while(1){
		printf("Inside client while loop\n");
		sleep(1);
		
		client_send = (message*)malloc(sizeof(message));
		
		P(message_lock);
		local_message_value = ++global_message_value;
		V(message_lock);

		for(i=0;i<10;i++){

			client_send->m_array[i] = local_message_value;
		}
		client_send->sender_port_number = client_port_number;
		server_port_number = 0;

		//printf("---Client with port number %d sending message value %d to Server on port number %d---\n",client_port_number, 
		//local_message_value, server_port_number);
		printf("Inside client sending msg\n");
		send(server_port_number,client_send);
		printf("Inside client receiving msg\n");
		client_recv = receive(client_port_number);	

		//Checking only 1 message value received correct or not
		printf("***Client with port number %d sent message value %d and received message value %d from Server on port number %d***\n",
																															client_port_number,
																															local_message_value,
																															client_recv->m_array[0],
																															client_recv->sender_port_number);
		//sleep(1);
	}

}

int main()
{
    int i;
    Ready_Q = newQueue();

    //Initializing all ports
    for(i=0;i<100;i++){
    	init_port(i);
    }
    message_lock = CreateSem(1);

    start_thread(client);
    printf("client1 created\n");
    //start_thread(server);
    start_thread(client);
    printf("client2 created\n");

    start_thread(server);
    printf("Server1 created\n");

    /*//start_thread(server);
    start_thread(client);
    printf("client3 created\n");
    //start_thread(server);
    start_thread(client);    
    printf("client4 created\n");*/
    
    run();
    printf("Started running\n");
    while (1) { 
    	sleep(1);
    };
    return 0;
}

/*

#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include "sem.h"

#define N 4
Semaphore_t *empty, *full, *mutex;
int buffer[N];
int in = 0, out = 0, item_num=0, prod_delay = 1, cons_delay = 1;


void prod ()
{
    while (1){ 
    printf("Producer ready to produce\n");
        P(empty);
          P(mutex);
        printf("Producer inserting item#%d, into slot #%d\n", item_num, in);
            buffer[in] = item_num++; in = (in+1) % N;
          V(mutex);
    V(full);
    sleep(prod_delay);
    }
}    

void cons()
{
    while(1){
    printf("\tConsumer ready to consume\n");
        P(full);
          P(mutex);
        printf("\tConsumer deleting item#%d, from slot #%d\n", buffer[out], out);
            out = (out+1) % N;
          V(mutex);
    V(empty);
        sleep(cons_delay);
    }    
}

*/
}

void client(){
	int i, local_message_value;
	int server_port_number;
	int client_port_number;

	printf("Enterd client function\n");

	message* client_send;
	message* client_recv;
	//client_recv.sender_port_number = server_port_number	//message received by client will have server's port number

	client_port_number  = rand() % 97 + 2;					//Getting random port number for client

	while(1){
		printf("Inside client while loop\n");
		sleep(1);
		
		client_send = (message*)malloc(sizeof(message));
		
		P(message_lock);
		local_message_value = ++global_message_value;
		V(message_lock);

		for(i=0;i<10;i++){

			client_send->m_array[i] = local_message_value;
		}
		client_send->sender_port_number = client_port_number;
		server_port_number = 0;

		//printf("---Client with port number %d sending message value %d to Server on port number %d---\n",client_port_number, 
		//local_message_value, server_port_number);
		printf("Inside client sending msg\n");
		send(server_port_number,client_send);
		printf("Inside client receiving msg\n");
		receive(client_port_number, client_recv);	

		//Checking only 1 message value received correct or not
		printf("***Client with port number %d sent message value %d and received message value %d from Server on port number %d***\n",
																															client_port_number,
																															local_message_value,
																															client_recv->m_array[0],
																															client_recv->sender_port_number);
		//sleep(1);
	}

}

int main()
{
    int i;
    Ready_Q = newQueue();

    //Initializing all ports
    for(i=0;i<100;i++){
    	init_port(i);
    }
    message_lock = CreateSem(1);

    start_thread(client);
    printf("client1 created\n");
    //start_thread(server);
    start_thread(client);
    printf("client2 created\n");

    start_thread(server);
    printf("Server1 created\n");

    /*//start_thread(server);
    start_thread(client);
    printf("client3 created\n");
    //start_thread(server);
    start_thread(client);    
    printf("client4 created\n");*/
    
    run();
    printf("Started running\n");
    while (1) { 
    	sleep(1);
    };
    return 0;
}

/*

#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include "sem.h"

#define N 4
Semaphore_t *empty, *full, *mutex;
int buffer[N];
int in = 0, out = 0, item_num=0, prod_delay = 1, cons_delay = 1;


void prod ()
{
    while (1){ 
    printf("Producer ready to produce\n");
        P(empty);
          P(mutex);
        printf("Producer inserting item#%d, into slot #%d\n", item_num, in);
            buffer[in] = item_num++; in = (in+1) % N;
          V(mutex);
    V(full);
    sleep(prod_delay);
    }
}    

void cons()
{
    while(1){
    printf("\tConsumer ready to consume\n");
        P(full);
          P(mutex);
        printf("\tConsumer deleting item#%d, from slot #%d\n", buffer[out], out);
            out = (out+1) % N;
          V(mutex);
    V(empty);
        sleep(cons_delay);
    }    
}

*/
