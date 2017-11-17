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

	message* server_send = NULL;
	message* server_recv = NULL;

	server_port_number = 0;

	
	while(1){
		//sleep(1);
		
		receive(server_port_number,&server_recv);
		client_port_number = server_recv->sender_port_number;
		
		server_send = server_recv;
		server_send->sender_port_number = server_port_number;
		
		send(client_port_number,server_send);
	}
}

void serverTwo(){
	int i, local_message_value;
	int server_port_number;
	int client_port_number;

	message* server_send = NULL;
	message* server_recv = NULL;

	server_port_number = 1;

	
	while(1){
		//sleep(1);
		
		receive(server_port_number,&server_recv);
		client_port_number = server_recv->sender_port_number;
		
		server_send = server_recv;
		server_send->sender_port_number = server_port_number;
		
		send(client_port_number,server_send);
	}
}

void client(){
	int i, local_message_value;
	int server_port_number;
	int client_port_number;

	message* client_send = NULL;
	message* client_recv = NULL;
	//client_recv->sender_port_number = server_port_number;					//message received by client will have server's port number
	
	while(1){		
		//sleep(1);
		client_port_number  = client_port_number = rand() % 97 + 2; 			//Getting random port number for client
		
		client_send = (message*)malloc(sizeof(message));
		client_recv = (message*)malloc(sizeof(message));

		P(message_lock);
		local_message_value = ++global_message_value;
		V(message_lock);

		for(i=0;i<10;i++){

			client_send->m_array[i] = local_message_value;
		}
		client_send->sender_port_number = client_port_number;
		server_port_number = 0;

		printf("\nClient with port number %d sent message value %d to Server on port number %d\n", client_port_number, 
																									local_message_value, 
																									server_port_number);
		send(server_port_number,client_send);
		
		receive(client_port_number, &client_recv);

		//Checking only 1 message value received correct or not
		printf("\nClient with port number %d received message value %d from Server on port number %d\n", client_port_number, 
																										client_recv->m_array[0],
																										client_recv->sender_port_number);
	}

}
void clientTwo(){
	int i, local_message_value;
	int server_port_number;
	int client_port_number;

	message* client_send = NULL;
	message* client_recv = NULL;
	//client_recv->sender_port_number = server_port_number;						//message received by client will have server's port number
	
	while(1){		
		//sleep(1);
		client_port_number  = client_port_number = rand() % 97 + 2; 			//Getting random port number for client
		
		client_send = (message*)malloc(sizeof(message));
		client_recv = (message*)malloc(sizeof(message));

		P(message_lock);
		local_message_value = ++global_message_value;
		V(message_lock);

		for(i=0;i<10;i++){

			client_send->m_array[i] = local_message_value;
		}
		client_send->sender_port_number = client_port_number;
		server_port_number = 1;

		printf("\nClient with port number %d sent message value %d to Server on port number %d\n", client_port_number, 
																									local_message_value, 
																									server_port_number);
		send(server_port_number,client_send);
		
		receive(client_port_number, &client_recv);

		//Checking only 1 message value received correct or not
		printf("\nClient with port number %d received message value %d from Server on port number %d\n", client_port_number, 
																										client_recv->m_array[0],
																										client_recv->sender_port_number);
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

    start_thread(server);
    printf("Server1 created\n");

    start_thread(client);
    printf("client1 created\n");
    
    start_thread(client);
    printf("client2 created\n");

    start_thread(serverTwo);
    printf("Server2 created\n");
    

    start_thread(clientTwo);
    printf("client3 created\n");

    start_thread(clientTwo);    
    printf("client4 created\n");
    
    run();
    printf("Started running\n");
    while (1) { 
    	sleep(1);
    };
    return 0;
}