#include "sem.h"
#define MAX_MSG 10
#define MSG_LENGTH 10
#define MAX_PORTS 100

typedef struct message{
	int m_array[MSG_LENGTH];
	int sender_port_number;
}message;

typedef struct port{
	message* msg;
	Semaphore_t *mutex, *full, *empty; //empty =  N: full = 0;
	int front, rear;
}port;

port port_array[MAX_PORTS];

//Initializing port
void init_port(int port_number){
	
	port_array[port_number].msg = (message*)malloc(MAX_MSG * sizeof(message));	//Each port can have 10 messages, each of size (int 10)
	//port_array[port_number].msg.sender_port_number = 999;						//default value 
	port_array[port_number].mutex = CreateSem(1);
	port_array[port_number].empty = CreateSem(MAX_MSG);
	port_array[port_number].full = CreateSem(0);
	port_array[port_number].front = 0;
	port_array[port_number].rear = 0;
}

//Send routine for port
void send(int port_number, message *port_msg){

	P(port_array[port_number].empty);
	P(port_array[port_number].mutex);
	
	port_array[port_number].msg[port_array[port_number].rear] = *port_msg;

	port_array[port_number].rear = (port_array[port_number].rear + 1) % MAX_MSG;

	V(port_array[port_number].mutex);
	V(port_array[port_number].full);

}

message* receive(int port_number){

	message *port_msg;

	P(port_array[port_number].full);
	P(port_array[port_number].mutex);
	//
	port_msg = &(port_array[port_number].msg[port_array[port_number].front]);

	port_array[port_number].front = (port_array[port_number].front + 1) % MAX_MSG; 

	V(port_array[port_number].mutex);
	V(port_array[port_number].empty);

	return port_msg;
}
