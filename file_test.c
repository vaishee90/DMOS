//Submitted by:
//Anandavaishnavi Ardhanari Shanmugam (1211256043)
//Aditi Sonik (1211009433)

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include "msgs.h"

int max_client_count; 

static int global_client_file_index = 1;
Semaphore_t *id_lock;
char** filenames;

int server_port_number = 0;								

//message is packed with sender_port_id, opcode and file_name/file_data
//port is at index 0 of message array;
//opcode is at index 1 of message array;

/*
	opcode -Table
	1 = client intrested in connection establishment
	2 = client_count < 3
	3 = client_count exceeded 3

	4 = successful connection established with server
	11 = client completed file name transfer

	5 = filename length is under 15 chars
	6 = filename length is exceeding limits

	7 = client ready to send file data
	8 = client completed file data transfer

	9 = file data size under 1MB
	10 = file data size exceeds 1MB

*/

//function to check message-data approved by server or not
int checkOpcodeAction(int opcode, int client_port_number){
	if(opcode == 5 || opcode == 9){											
       	return 2;
    }
    else if(opcode == 6){													
    	printf("Filename exceeded 15 chars for client %d\n",client_port_number);
    	while(1)
    		yield();
    }
    else if(opcode == 10){
    	printf("Filedata size exceeded 1MB for client %d\n",client_port_number);
    	while(1)
    		yield();
    }
    else{
    	printf("filename : Invalid opcode received... exiting...\n");	
		while(1)
			yield();
    }
}

//function to obtain length of the client file 
int file_length(FILE * fd){
	int size;

	fseek(fd, 0, SEEK_END);												//reposition the file pointer to the end of the file
	size = ftell(fd);													//obtain the size of file by obtaining the location of file pointer

	return size;
}

void server(){
	int i;
	int server_port_number = 0;
	int client_port_number;
	
	int client_count = 0;												//keep track of total clients currently served by the server
	char file_names[max_client_count][256];								//array to store all client filenames

	message* server_send;
	message* server_recv;

	char filename[27] = "";												//local file name object for keeping track of filename of each client
	int file_name_length;												//to keep track of file_name_length 

	FILE * fileFd;														//file descriptor to write to files
	unsigned long file_size;											//to read size of file
	
	//server listening to requests
	while(1){
		strcpy(filename,"");											//resetting local filename variable
		
		receive(server_port_number, &server_recv);
		server_send = (message*)malloc(sizeof(message));

		client_port_number = server_recv->m_array[0];					//msg index 0 has the sender port number
		
		//client request for connection establishment i.e opcode = 1
		if(server_recv->m_array[1] == 1){		
			if(client_count == 3){				
				server_send->m_array[1] = 3;							//server busy
			}
			else{
				client_count++;					
				server_send->m_array[1] = 2;							//connection successful
			}
			server_send->m_array[0] = server_port_number;
			send(client_port_number,server_send);
		}				
		else if(server_recv->m_array[1] == 4 || server_recv->m_array[1] == 11){
			//client connection established now performing filename transfer i.e. opcode = 4 || 11		
			
			memset(filename,0,sizeof(filename));
	      	strcpy(filename,file_names[client_port_number]);

	      	file_name_length = strlen(filename);

	      	
	      	for(i=2;i<10;i++){
	      		
		        if(server_recv->m_array[i] != -1){
			    	if(file_name_length >= 15){
			            server_send->m_array[1] = 6;					//file name exceeded 15 chars
			        	client_count--;
			        	break;    
			        }
			        else{
			        	filename[file_name_length++] = server_recv->m_array[i];
			        }
		    	}
	    	}
	    	
	    	if(server_send->m_array[1] != 6){	    		
	    		server_send->m_array[1] = 5;							//file name accepted by server

	    		printf("server received file = %s from client\n",filename);

	    		if(server_recv->m_array[1] == 11){						
	    			strcat(filename,".server");							//creating a server copy of file
	    			
	    		}

	    	}
	    	strcpy(file_names[client_port_number],filename);
	    	server_send->m_array[0] = server_port_number;				//msg index 0 contains sender port number
			send(client_port_number,server_send);
		}
		else if(server_recv->m_array[1] == 7 || server_recv->m_array[1] == 8){				
			//client transfering data for its file i.e. opcode = 7 || 8					
			
			strcat(filename,file_names[client_port_number]);
	    	fileFd = fopen(filename, "a");

	    	for(i=2;i<10;i++){
	    		
		        if(server_recv->m_array[i] != -1){
		        	fputc(server_recv->m_array[i], fileFd);
	    		}

	    		file_size = file_length(fileFd);
	    		
	    		if(file_size > 1048576){				//Check if file size copied from client exceeds 1MB limit
	    			server_send->m_array[1] = 10;		//file size exceeds 1MB
	    			fclose(fileFd);
	    			client_count--;
	    			remove(filename);
	    			break;
	    		}
	    	}
	    	if(server_send->m_array[1] != 10){
	    		server_send->m_array[1] = 9;			//file size accepted by server
	    		fclose(fileFd);
	    		if(server_recv->m_array[1] == 8){		//file transfer and writting completed
	    			client_count--;						// client can terminate and server can serve another client
	    		}
	    	}	

	    	server_send->m_array[0] = server_port_number;
			send(client_port_number,server_send);    	
		}		
		else{									
			//client request unidentified by server
			printf("unidentified request sent by client\n");
			
		}
		
	}//end of while
}


void client(){

	int local_file_index, i;
	int client_port_number;
	char c;

	int m_array_start_index = 2;

	char *client_filename;
	FILE *client_file;

	message* client_send;
	message* client_recv;

	//locking client file ID (also client ID)
	P(id_lock);		
		local_file_index = global_client_file_index;
		client_port_number = global_client_file_index;
		printf("Client %d attempting to connect to server\n", global_client_file_index);		
		global_client_file_index++;
	V(id_lock);

	client_filename = filenames[local_file_index];

	client_send = (message*)malloc(sizeof(message));
	client_send->m_array[0] = client_port_number;
	client_send->m_array[1] = 1;												//client request for connection establishment i.e opcode = 1

	while(1){
		send(server_port_number, client_send);
		receive(client_port_number, &client_recv);

		//Checking if client can send message
		if(client_recv->m_array[1] == 2){										//connection established with the server
			break;
		}
		else if(client_recv->m_array[1] == 3){									//server busy client rejected
			//sleep(1);
			printf("connection : Buffer full. Client %d rejected \n",client_port_number);
			while(1)
				yield();
		}
		else{																	//server received unidentified client request
			printf("connection : Invalid opcode received... exiting...\n");
			while(1)
				yield();
		}
	}

	client_send->m_array[1] = 4;									//successful connection established with the server, client ready to send filename
		
	for(i = 0; i < strlen(client_filename); i++){
    	client_send->m_array[m_array_start_index++] = client_filename[i];
    	if(m_array_start_index == 10){											//send message to server if the message array limit has been reached
    		printf("Client %d sending filename to server\n",local_file_index);

   			client_send->m_array[0] = client_port_number;
	   	    send(server_port_number, client_send);
       		receive(client_port_number, &client_recv);

       		m_array_start_index = checkOpcodeAction(client_recv->m_array[1], client_port_number);	//reset message array index to send remaining file name if still present
    	}		
    }

    if(m_array_start_index != 10){
   		for(i = m_array_start_index; i < 10; i++){
   			client_send->m_array[i] = -1;										//filling up empty spaces of message with -1
   		}
   	}

    client_send->m_array[1] = 11;												//filename transfer successful, client ready to send data to server

   	client_send->m_array[0] = client_port_number;
   	send(server_port_number, client_send);
   	receive(client_port_number, &client_recv);

   	m_array_start_index = checkOpcodeAction(client_recv->m_array[1], client_port_number); //checking filename length > 15 chars
         

    client_file = fopen(client_filename, "r");

    client_send->m_array[1] = 7;								//client ready to transfer data

    m_array_start_index = 2;

    while(1){
    										
    	if((c = fgetc(client_file)) != EOF){						//reading each char from client file
    		if(m_array_start_index != 10){
    			client_send->m_array[m_array_start_index++] = c;	//appending file data to the message array
     		}
      		else{
      			client_send->m_array[0] = client_port_number;
    			send(server_port_number, client_send);
    			receive(client_port_number, &client_recv);

    			m_array_start_index = checkOpcodeAction(client_recv->m_array[1], client_port_number);	//reset message array index to send remaining file data if still present
    		}
    	}
    	else
    		break;
    }

    if(m_array_start_index != 10){
    	for(i = m_array_start_index; i < 10; i++){
    		client_send->m_array[i] = -1;					//filling remaining message array with -1 when all data has been loaded
    	}
    }

    client_send->m_array[1] = 8;							//file data transfer complete

    client_send->m_array[0] = client_port_number;
    send(server_port_number, client_send);
    receive(client_port_number, &client_recv);

	m_array_start_index = checkOpcodeAction(client_recv->m_array[1], client_port_number); //checking for file data size > 1MB

    fclose(client_file);

    //free(filenames[client_port_number]);

    printf("Client %d has completed file transfer\n",client_port_number);
    while(1)
       	yield();
}

int main(int argc, char* argv[])
{
    int i;
    Ready_Q = newQueue();

    //Initializing all ports
    for(i=0;i<100;i++){
    	init_port(i);
    }

    //lock to prevent other clients from obtaining wrong ids
    id_lock = CreateSem(1);

    //starting server thread
    start_thread(server);
    printf("Server created\n\n");

    //getting number of clients
    max_client_count = atoi(argv[1]);

    //allocating space for the filenames of the clients
    filenames = malloc(max_client_count * sizeof(*filenames));

    //storing filenames, opening files for reading and starting each client thread
    for(i = 0; i < max_client_count; i++){
    	filenames[i+1] = malloc(strlen(argv[2+i]) + 1);
    	strcpy(filenames[i+1], argv[2 + i]);
    	start_thread(client);
    	printf("Client %d started\n", i+1);    	
    }
    printf("\n");

    run();
    printf("Started running\n\n");

    while (1) { 
    	//printf("main running\n");
    	sleep(1);
    }

    return 0;
}

