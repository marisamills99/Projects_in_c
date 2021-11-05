// Three linked lists
#include <stdlib.h>
#include <stdio.h>

/*
typedef struct schedule_struct {
  List *ready_list;   
  List *stopped_list; 
  List *defunct_list; 
} Schedule;*/

/* List Struct Definition 
typedef struct list_struct {
  Process *head; 
  int count;     
} List;*/

/* Process Struct Definition 
typedef struct process_struct {
  char *command;      /* Process Command 
  int pid;            /* Process ID (unique) 
  int flags;          /* Process Flags 
  int time_remaining; /* Time Units Left to Execute 
  int time_last_run;  /* Last Time Executed (or time created) 
  struct process_struct *next; 
} Process;
*/
Scheduler *scheduler_init(){
	//make schedule and lists
	
	Scheduler *mySchedule = malloc(sizeof(Schedule)); // allocate memory 3 Lists
	mySchedule->ready_list= malloc(sizeof(List));
	mySchedule->defunct_list=malloc(sizeof(List));
	mySchedule->stopped_list=malloc(sizeof(List));
	
	return mySchedule;
}
int scheduler_count(List *l){
	int count = -1;
	if (*l){
	count = *l->count;
	}
	return count;
}

Process *scheduler_generate(char *command, int pid, int time_remaining, int is_sudo){
	Process *newProcess=NULL;
	// use malloc for command
	newProcess->pid= pid;
	newProcess->time_remaining=time_remaining;
	int time = clock_get_time();
	newProcess->time_last_run= time;
	//set up flag to state created
	int flag=0x00000001;
	if(is_sudo==1){
		flag= 32|flag;
	}
	newProcess->flags=flag;
	return newProcess;
}
Process * list_add(List *list, Process *p){
	Process curr;
	if(list->head==NULL){
		list->head=p;
		list->count++;
		return list->head;
	}
	curr= list->head;
	while (curr->next != NULL && (curr->pid < p->pid)){
		curr= curr->next;
	}
	if(curr->next== NULL){
		curr->next = p;
		list->count++;
	}

	else{
		p->next = curr->next;
		curr->next = p;
		list->count++;
	}
	return p;
}
int scheduler_add(Schedule *s, Process *p){
	//if in state created move to state ready 
	if (p->flags&1==1){
		p->flags= (p->flags&34);
		//insert into ready list
		list_add(s->ready_list,p);
	}
	else if(p->flags&2==2){
		if (p->time_remaining >0){
			//insert into ready list in ascending PID order
			list_add(s->ready_list,p);
		}
		else{
			//insert into defunct list 
			list_add(s->defunct_list,p);
		}
	}
	else if (p->flags&8==8){
		//insert into defunct
		list_add(s->defunct_list,p);
	}
	else{
		return -1;
	}
	return 0;
}
Process *scheduler_select(Schedule *schedule){
	//find process with smallest time remaining in ready list
	List *ready_list= schedule->ready_list;
	list mylist= ready_list->head;
	int count= ready_list->count;
	int greatest=0;
	Process returnval;
	int i=0;
	while(i<count){
		if(mylist->time_remaining>greatest){
			greatest=mylist->time_remaining;
			returnval=mylist;
		}
		i++;
		mylist=mylist->next;
	}
	return returnval;
	//ADD STARVATATION
}

int scheduler_reap(Schedule *s, int pid){
	// 4294967232
	int exitcode=-1;
	List *defunct=s->defunct_list;
	Process *curr= defunct->head;
	while (curr->pid<=pid){
		curr=curr->next;
	}
	if (curr->pid==pid){
		//remove from defunct list
		//set state to terminated
		exitcode=defunct>>5;
		free(curr);
	}
	return exitcode;
}
int scheduler_stop(Schedule *s, int pid){
	
	
}
int scheduler_continue(Schedule *s, int pid){
	
}

void scheduler_free(Schedule *s){
	
}