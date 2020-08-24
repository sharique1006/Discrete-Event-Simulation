#include <stdio.h> 
#include <stdlib.h> 
#include "../include/PriorityQueue.h"

//EventQueue
EventQueue* newEventQueue(event *d, float p) { 
	EventQueue* temp = (EventQueue*)malloc(sizeof(EventQueue)); 
	temp->data = d; 
	temp->priority = p; 
	temp->next = NULL; 
	return temp; 
} 

event *top_event(EventQueue** head) { 
	return (*head)->data; 
} 

void pop_event(EventQueue** head) { 
	EventQueue* temp = *head; 
	(*head) = (*head)->next; 
	free(temp); 
} 

void push_event(EventQueue** head, event *d, float p) { 
	EventQueue* start = (*head); 
	EventQueue* temp = newEventQueue(d, p); 
	if ((*head)->priority > p) { 
		temp->next = *head; 
		(*head) = temp; 
	} 
	else { 
		while (start->next != NULL && start->next->priority < p) { 
			start = start->next; 
		} 
		temp->next = start->next; 
		start->next = temp; 
	} 
} 

int isEmptyEventQueue(EventQueue** head) { 
	return (*head) == NULL; 
} 

//Teller Queue
TellerQueue* newTellerQueue(customer *d, float p) { 
	TellerQueue* temp = (TellerQueue*)malloc(sizeof(TellerQueue)); 
	temp->data = d; 
	temp->priority = p; 
	temp->next = NULL; 
	return temp; 
} 

customer *top_customer(TellerQueue** head) { 
	return (*head)->data; 
} 

void pop_customer(TellerQueue** head) { 
	TellerQueue* temp = *head; 
	(*head) = (*head)->next; 
	free(temp); 
} 

void push_customer(TellerQueue** head, customer *d, float p) { 
	TellerQueue* start = (*head); 
	TellerQueue* temp = newTellerQueue(d, p); 
	if ((*head)->priority > p) { 
		temp->next = *head; 
		(*head) = temp; 
	} 
	else { 
		while (start->next != NULL && start->next->priority < p) { 
			start = start->next; 
		} 
		temp->next = start->next; 
		start->next = temp; 
	} 
} 

int isEmptyTellerQueue(TellerQueue** head) { 
	return (*head) == NULL; 
} 