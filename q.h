#include"tcb.h"
/*
struct qNode{
	int payload;
	struct qNode * prev;
	struct qNode * next;
};
*/
struct TCB_t * newQueue(){

	struct TCB_t * head;
	head = (struct TCB_t*)malloc(sizeof(struct TCB_t));
	head->prev = head;
	head->next = head;
	//head->payload = 999;//dummy value
	return head;
}

struct TCB_t * NewItem(){
	struct TCB_t * node;
	node = (struct TCB_t*)malloc(sizeof(struct TCB_t));
	node->prev = NULL;
	node->next = NULL;
	//node->payload =  value;

	return node;
}

struct TCB_t * AddQueue(struct TCB_t * head,struct TCB_t * item){
	struct TCB_t * temp;
	temp = head;
	if(head->next == head){
		head->next = item;
		item->prev = head;
	}
	else{
		while(1){
			if(temp->next != head)
				temp = temp->next;
			else
				break;
		}
		temp->next = item;
		item->prev = temp;
	}
	//reverse direction from head to last node
	item->next = head;
	head->prev = item;
	return head;
}

struct TCB_t * DelQueue(struct TCB_t * head){
	struct TCB_t * del, *temp;	
	if(head->next != head){
		temp = head;
		del = head->next;
		temp->next = temp->next->next;
		temp->next->prev = temp;
		del->next = NULL;
		del->prev = NULL;
		head = temp;
	}
	else {
		printf("Empty queue\n");
		del = NULL;
	}
	return del;
}
