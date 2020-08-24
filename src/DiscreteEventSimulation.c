#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include "../include/PriorityQueue.h"	

int num_customers;
int num_tellers;
float simulationTime;
float averageServiceTime;
float total_idle_time;
float total_service_time;
float maxWait;
float timeSpent;
int *helper_cust;
float completion_time;
float sumofSqr;
EventQueue *eventQueue;
TellerQueue **tellerQ;
TellerQueue *tellerQueue;

void plotGraph(const char gg[]) {
  FILE * pipe = popen ("gnuplot -persistent", "w"); 
  fprintf(pipe,"set style fill solid\n");
  fprintf(pipe,"plot './output/data.txt' using 2:xtic(1) title 'Float' with lines lt rgb '#406090'\n");
  fprintf(pipe,"set term png\n");
  fprintf(pipe,"set output '%s'\n",gg); 
  fprintf(pipe,"replot\n");
}

void display_stats() {
	printf("Maximum Waiting time : %f\n",maxWait );
	printf("Total Service time : %f\n",total_service_time );
	printf("Total Idle time : %f\n",total_idle_time );
	printf("Completion time : %f\n",completion_time );
	printf("Average Amount of time : %f\n",timeSpent/num_customers );
	printf("Standard Deviation of time : %f\n",sqrt((sumofSqr/num_customers) - (timeSpent/num_customers)*(timeSpent/num_customers)));
}

void GenerateEventQueue() {
	teller *t1 ;
	t1 = new_teller(t1);
	t1->line = 0;
	event *e1;
	e1 = newTellerEvent(t1, e1);
	eventQueue = newEventQueue(e1, t1->time);
	for(int i=1; i < num_tellers; i++) {
		teller *t ;
		t = new_teller(t);
		t->line = i;
		event *e;
		e = newTellerEvent(t, e);
		push_event(&eventQueue, e, t->time);
		
	}
}

void GenerateTellerQueue() {
	customer *c1 ;
	c1 = new_customer(c1);
	tellerQueue = newTellerQueue(c1, c1->arrival_time);
	for(int i=1; i < num_customers; i++) {
		customer *c ;
		c = new_customer(c);
		push_customer(&tellerQueue, c, c->arrival_time);
		
	}  	
}

void single_line_simulation(){
	GenerateEventQueue();
	GenerateTellerQueue();
	while(!isEmptyTellerQueue(&tellerQueue)){
		customer *c;
		c = top_customer(&tellerQueue);
		execute_single_line(c, c->arrival_time);
		pop_customer(&tellerQueue);
	} 
}

void multiple_line_simulation(){
	GenerateEventQueue(); 
	GenerateTellerQueue();
	helper_cust = (int*) malloc(num_tellers*sizeof(int));
	for(int i=0;i<num_tellers;i++){
		helper_cust[i] = 0;
	}
	tellerQ = (TellerQueue**) malloc(num_tellers*sizeof(TellerQueue*));
	while(!isEmptyTellerQueue(&tellerQueue)){
		customer *c;
		c = top_customer(&tellerQueue);	
		int keep_doing = 0;
		while(keep_doing == 0) {
			keep_doing = execute_multiple_line(c);
		}
		pop_customer(&tellerQueue);
	} 
}

int main(int argc, char *argv[]) {
	num_customers = atoi(argv[1]);
	num_tellers = atoi(argv[2]);
	simulationTime = atof(argv[3]);
	averageServiceTime = atof(argv[4]);
	
	printf("Simulation time : %f\n",simulationTime);
	printf("Idle time between : %d - %d\n",1,600);
	printf("num_customers : %d , num_tellers : %d\n\n", num_customers, num_tellers);

	total_service_time = 0.0;
	total_idle_time = 0.0;
	maxWait = 0.0;
	completion_time = 0.0; 
	sumofSqr = 0.0;

	printf("Simulation with single line for all tellers!\n");
	single_line_simulation();
	display_stats();
	printf("\n");

	total_service_time = 0.0;
	total_idle_time = 0.0;
	maxWait = 0.0;
	timeSpent = 0.0;
	completion_time = 0.0; 
	sumofSqr = 0.0;

	free(eventQueue);
	free(tellerQueue);

	printf("Simulation with seperate line for each teller!\n");
	multiple_line_simulation();
	display_stats();

	FILE *fp = NULL;
    fp=fopen("./output/data.txt","w");
    for(int i=1;i<200;i=i+2){
    	 fprintf(fp, "%d",i);
    	 total_service_time = total_idle_time = maxWait = timeSpent = completion_time =  sumofSqr = 0.0;
    	 num_tellers = i;
    	 single_line_simulation();
    	 fprintf(fp, " %f\n",timeSpent/num_customers);
    }
    plotGraph("./output/plot");
	return 0;
}