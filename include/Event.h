#include <stdlib.h>
#include <stdio.h>

//Customer
typedef struct customer customer;
typedef struct event event;
typedef struct customer
{
	float time;
	float arrival_time;
}customer;

customer *new_customer(customer *c);

//Teller
typedef struct teller {
	float time;
	float idle_time;
	int line;
}teller;

teller *new_teller(teller *t);

//Event
typedef struct event
{
	customer *c;
	teller *t;
	
}event;

event *newTellerEvent(teller *t, event *e);
event *newCustomerEvent(customer *c, event *e);
void execute_single_line(customer *c, float arr);
int execute_multiple_line(customer *c);