#include<stdio.h>

#include"q.h"

int main(){
	struct qNode * object, * head, *temp;
	int i;
	object = (struct qNode*)malloc(sizeof(struct qNode));

	head = newQueue();
	for(i=0;i<6;i++){
		object = NewItem(i+10);
		head = AddQueue(head,object);
	}
	temp = head;
	while(1){
		if(temp->next != head){
			printf("payload value at = %d\n",temp->payload);
			temp = temp->next;
		}
		else
			break;
	}

	object = DelQueue(head);
	printf("Deleted payload = %d\n",object->payload);
	printf("New Queue\n");
	temp = head;
	while(1){
		if(temp->next != head){
			printf("payload value at = %d\n",temp->payload);
			temp = temp->next;
		}
		else
			break;
	}

	return 0;
} 