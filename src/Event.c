#include <stdio.h> 
#include <stdlib.h>
#include "../include/PriorityQueue.h"

extern int num_tellers;
extern float simulationTime;
extern float averageServiceTime;
extern float total_idle_time;
extern float total_service_time;
extern float maxWait;
extern float timeSpent;
extern int *helper_cust;
extern float completion_time;
extern float sumofSqr;
extern EventQueue *eventQueue;
extern TellerQueue **tellerQ;

//Create a New Teller Event
event *newTellerEvent(teller *t1, event *e){
	e = (event *)malloc(sizeof(event));
	e->t = t1;
	return e;
}

//Create a New Customer Event
event *newCustomerEvent(customer *t1, event *e){
	e = (event *)malloc(sizeof(event));
	e->c = t1;
	return e;
}

//Create a New Teller
teller *new_teller(teller *t){
	t = (teller*)malloc(sizeof(teller));
	t->time = 0.0 ;
	t->idle_time = 1.0+ (float)rand()/(float)(RAND_MAX/10.0);
	t->line = 0;
	return t;
}

//Create a New Customer
customer *new_customer(customer *t){
	t = (customer*)malloc(sizeof(customer));
	t->time = 0.0 ;
	t->arrival_time = 1.0+ (float)rand()/(float)(RAND_MAX/simulationTime);
	return t;
}

//Calculate time when a customer is at the front
void calculate_time(event *e, customer *c, int type) {
	float serviceTime = 2*((float)rand()/(float)(RAND_MAX/averageServiceTime));
	total_service_time += serviceTime;
	e->t->time = e->t->time + serviceTime;
	if(e->t->time > completion_time){
		completion_time = e->t->time;
	}
	float serviceComp;
	if (type == 0) {
		serviceComp = e->t->time - e->c->arrival_time;
	}
	else {
		serviceComp = e->t->time - c->arrival_time;
	}
	timeSpent += serviceTime + serviceComp;
	sumofSqr += (serviceTime + serviceComp)*(serviceTime + serviceComp);
	if(serviceComp > maxWait){
		maxWait = serviceComp;
	}
}

void pushToEventQueue(event *e) {
	if(isEmptyEventQueue(&eventQueue)){
			eventQueue = newEventQueue(e,e->t->time);
	}
	else{
		push_event(&eventQueue,e,e->t->time);
	}
}

//Service in Case of Single Line 
void execute_single_line(customer *c, float arr){
	while(top_event(&eventQueue)->t->time < arr ){
		event *et = top_event(&eventQueue);
		pop_event(&eventQueue);
		et->t->time = et->t->time + et->t->idle_time;
		total_idle_time += et->t->idle_time;
		pushToEventQueue(et);
	}
	event *e;
	e = top_event(&eventQueue);
	e->c = c;
	pop_event(&eventQueue);
	calculate_time(e, c, 0);
	pushToEventQueue(e);
}

void process(int line){
	event *e = top_event(&eventQueue);
	pop_event(&eventQueue);
	customer *c = top_customer(&tellerQ[line]);
	pop_customer(&tellerQ[line]);
	helper_cust[line]--;
	calculate_time(e, c, 1);
	pushToEventQueue(e);
}

int NotAllZero() {
	int x = 0;
	for(int i = 0; i < num_tellers; i++){
		if(helper_cust[i] != 0){
			x = 1;
		}
	}
	return x;
}

int getMax() {
	int max = 0;
	for(int i = 0; i < num_tellers;i++){
		if(helper_cust[max] < helper_cust[i]){
			max = i;
		}
	}
	return max;
}

int getMin() {
	int min = 0;
	for(int i = 0; i < num_tellers; i++){
		if(helper_cust[min] > helper_cust[i]){
			min = i;
		}
	}
	return min;
}

int getMaxCount(int max) {
	int num_max = 0;
	for(int i = 0; i < num_tellers; i++){
		if(helper_cust[max] == helper_cust[i]){
			num_max++;
		}
	}
	return num_max;
}

int getMinCount(int min) {
	int num_min = 0;
	for(int i = 0; i < num_tellers; i++){
		if(helper_cust[min] == helper_cust[i]){
			num_min++;
		}
	}
	return num_min;
}

//Service in Case of Multiple Lines
int execute_multiple_line(customer *c) {
	if(c->arrival_time > top_event(&eventQueue)->t->time) {
		event *e = top_event(&eventQueue);
		if(helper_cust[e->t->line] == 0 ){
			if(NotAllZero() == 0){
				pop_event(&eventQueue);
				e->t->time += e->t->idle_time;
				total_idle_time += e->t->idle_time;
				pushToEventQueue(e);
			}
			else{
				int max = getMax();
				int num_max = getMaxCount(max);
				int random_line = rand()%num_max;
				int pos = 0;
				for(int i = 0; i < num_tellers; i++){
					if(helper_cust[max] == helper_cust[i]){
						if(pos == random_line){
							process(i);
							break;
						}
						else{
							pos++;
						}
					}
				}
			}
			return 0;
		}
		process(e->t->line);
		return 0;
	}
	else {
		int min = getMin(); 
		int num_min = getMinCount(min);
		int random_line = rand()%num_min;
		int pos = 0;
		for(int i = 0; i < num_tellers; i++) {
			if(helper_cust[min] == helper_cust[i]) {
				if(pos == random_line) {
					if(isEmptyTellerQueue(&tellerQ[i])){
						tellerQ[i] = newTellerQueue(c, c->arrival_time);
					}
					else{
						push_customer(&tellerQ[i],c,c->arrival_time);
					}
					helper_cust[i]++;
					break;
				}
				else{
					pos++;
				}
			}
		}
	}
	return 1;
}