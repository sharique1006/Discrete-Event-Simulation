#include <stdlib.h>
#include <stdio.h>
#include "Event.h"

//EventQueue
typedef struct EventQueue { 
	event *data; 
	float priority; 
	struct EventQueue* next; 
} EventQueue; 

EventQueue* newEventQueue(event *d, float p);
event *top_event(EventQueue** head);
void pop_event(EventQueue** head);
void push_event(EventQueue** head, event *d, float p);
int isEmptyEventQueue(EventQueue** head);

//TellerQueue
typedef struct TellerQueue { 
	customer *data; 
	float priority; 
	struct TellerQueue* next; 
} TellerQueue; 
 
TellerQueue* newTellerQueue(customer *c, float p);
customer *top_customer(TellerQueue** head);
void pop_customer(TellerQueue** head);
void push_customer(TellerQueue** head, customer *c, float p);
int isEmptyTellerQueue(TellerQueue** head);