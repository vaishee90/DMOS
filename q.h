#include"tcb.h"

struct qNode{
	int payload;
	struct qNode * prev;
	struct qNode * next;
};

struct qNode * newQueue(){

	struct qNode * head;
	head = (struct qNode*)malloc(sizeof(struct qNode));
	head->prev = head;
	head->next = head;
	head->payload = 999;//dummy value
	return head;
}

struct qNode * NewItem(int value){
	struct qNode * node;
	node = (struct qNode*)malloc(sizeof(struct qNode));
	node->prev = NULL;
	node->next = NULL;
	node->payload =  value;

	return node;
}

struct qNode * AddQueue(struct qNode * head,struct qNode * item){
	struct qNode * temp;
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

struct qNode * DelQueue(struct qNode * head){
	struct qNode * del, *temp;	
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
