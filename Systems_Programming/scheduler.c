/* Fill in your Name and GNumber in the following two comment fields
 * Name: Marisa Mills
 * GNumber: 01075052
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clock.h"
#include "structs.h"
#include "constants.h"
#include "scheduler.h"

/* Called when the program terminates.
 * You may use this to clean up memory or for any other purpose.
 */
void scheduler_exit(Schedule *schedule) {
	/*Process* del;
	Process* curr=schedule->stopped_list->head;
	
	*/
  return ;
}

/* Initialize the Schedule Struct
 * Follow the specification for this function.
 * Returns a pointer to the new Schedule or NULL on any error.
 */
Schedule *scheduler_init() {
	// allocate for schedule struct
	Schedule *mySchedule =(Schedule *) malloc(sizeof(Schedule)); 
	// allocate memory for 3 Lists
	mySchedule->ready_list= malloc(sizeof(List));
	mySchedule->ready_list->count=0;
	mySchedule->ready_list->head=NULL;
	
	mySchedule->defunct_list=malloc(sizeof(List));
	mySchedule->defunct_list->count=0;
	mySchedule->defunct_list->head=NULL;
	
	mySchedule->stopped_list=malloc(sizeof(List));
	mySchedule->stopped_list->count=0;
	mySchedule->stopped_list->head=NULL;
	
	if(mySchedule->ready_list==NULL||mySchedule->defunct_list==NULL||mySchedule->stopped_list==NULL){
		//erorr allocating 
		return NULL;
	}
	
	return mySchedule;
}
int remove_list(List *list, int pid){
	Process* curr =list->head;
	int i=0;
	Process* prev= NULL;
	if (curr->next==NULL){
		if (curr->pid==pid){
			//remove head when nothing in list
			list->head=NULL;
			list->count-=1;
			return 0;
		}
	}
	 while (curr!= NULL&& i<list->count){
		 if (curr->pid==pid){
			 if(prev==NULL){
				 // prev hasn't been set remove head when stuff in list
				 list->head = list->head->next;
				 list->count-=1;
				 break;
			 }
			 prev->next= curr->next;
			 list->count-=1;
			 break;
		 }
		 prev=curr;
		 curr=curr->next;
	 }
	 return 0;
 }
List * list_add(List *list, Process *p){
	Process *curr= list->head;
	Process *prev= NULL;
	//check if list is empty
	if(list->head==NULL){
		list->head=p;
		list->count++;
		return list;
	}
	//check if need to add at beginning of list
	if(curr->next== NULL&&(curr->pid > p->pid)){
			p->next = curr;
			list->head = p;
			list->count++;
			return list;
	}
	//while the next isnt null and the current pid is less than pid im trying to insert increment
	while (curr->next != NULL && (curr->pid < p->pid)){
		prev=curr;
		curr= curr->next;
	}
	//if we reached end of list this is the highest pid...insert last
	if(curr->next== NULL){
		curr->next = p;
		list->count++;
	}
	else{
		//if previous exists insert here
		if (prev){
			p->next = curr;
			prev->next = p;
			list->count++;
		}
		//else edit head
		else{
			p->next = curr;
			list->head=p;
			list->count++;
		}
	}
	return list;
}
/* Add the process into the appropriate linked list.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error.
 */
int scheduler_add(Schedule *s, Process *p) {
	
	if ((p->flags&0x1)==1){
		//set to state ready
		p->flags= p->flags&0xFFFFFFE0;
		p->flags=((1<<1)|p->flags);
		//insert into ready list
		s->ready_list=list_add(s->ready_list,p);
		return 0;
	}
	else if((p->flags&0x2)==2){
		if (p->time_remaining >0){
			//insert into ready list in ascending PID order
			s->ready_list=list_add(s->ready_list,p);
		}
		else{
			//insert into defunct list and set state to defunct
			p->flags= p->flags&0xFFFFFFE0;
			p->flags=(1<<3)|p->flags;
			s->defunct_list=list_add(s->defunct_list,p);
		}
	}
	else if ((p->flags&0x8)==8){
		//insert into defunct
		s->defunct_list=list_add(s->defunct_list,p);
	}
	else{
		return -1;
	}
	return 0;
}

/* Move the process with matching pid from Ready to Stopped.
 * Change its State to Stopped.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error.
 */

int scheduler_stop(Schedule *s, int pid) {
	List *ready=s->ready_list;
	Process *curr= ready->head;
	if (curr->pid==pid){
		//first pid is the one
		// change state to stopped
		curr->flags= curr->flags&0xFFFFFFE0;
		curr->flags=(1<<2)|curr->flags;
		//add to stopped
		s->stopped_list=list_add(s->stopped_list,curr);
		//remove and change head ref
		remove_list(s->ready_list,pid);
		return 0;
		
	}
	while (curr->pid<pid){
		curr=curr->next;
	}
	if (curr->pid==pid){
		//set to state stopped
		curr->flags= curr->flags&0xFFFFFFE0;
		curr->flags=(1<<2)|curr->flags;
		//move to ready
		s->stopped_list=list_add(s->stopped_list,curr);
		//remove from stopped list
		remove_list(s->ready_list,pid);
		return 0;
	}
	return -1;
}

/* Move the process with matching pid from Stopped to Ready.
 * Change its State to Ready.
 * Follow the specification for this function.
 * Returns a 0 on success or a -1 on any error.
 */
int scheduler_continue(Schedule *s, int pid) {
	List *stop=s->stopped_list;
	Process *curr= stop->head;
	if (curr->pid==pid){
		//first pid is the one
		// change state to ready
		curr->flags= curr->flags&0xFFFFFFE0;
		curr->flags=(1<<1)|curr->flags;
		//move to ready
		s->ready_list=list_add(s->ready_list,curr);
		//remove and change head ref
		remove_list(s->stopped_list,pid);
		return 0;
		
	}
	while (curr->pid<pid){
		curr=curr->next;
	}
	if (curr->pid==pid){
		//move to ready
		// change state to ready
		curr->flags= curr->flags&0xFFFFFFE0;
		curr->flags=(1<<1)|curr->flags;
		s->ready_list=list_add(s->ready_list,curr);
		//remove from stopped list
		remove_list(s->stopped_list,pid);
		return 0;
	}
	return -1;

}

/* Remove the process with matching pid from Defunct.
 * Follow the specification for this function.
 * Returns its exit code (from flags) on success or a -1 on any error.
 */
int scheduler_reap(Schedule *s, int pid) {
	int exitcode=-1;
	List *defunct=s->defunct_list;
	Process *curr= defunct->head;
	if (curr->pid==pid){
		//first pid is the one
		// change state to terminated
		curr->flags= curr->flags&0xFFFFFFE0;
		curr->flags=(1<<4)|curr->flags;
		//remove and change head ref
		remove_list(s->defunct_list,pid);
		exitcode=curr->flags>>6;
		return exitcode;
		
	}
	while (curr->pid<pid){
		curr=curr->next;
	}
	if (curr->pid==pid){
		//change state to terminated
		curr->flags= curr->flags&0xFFFFFFE0;
		curr->flags=(1<<4)|curr->flags;
		exitcode=curr->flags>>6;
		//remove from defunct list
		remove_list(s->defunct_list,pid);
		
	}
	return exitcode;
}

/* Create a new Process with the given information.
 * - Malloc and copy the command string, don't just assign it!
 * Set the STATE_CREATED flag.
 * If is_sudo, also set the PF_SUPERPRIV flag.
 * Follow the specification for this function.
 * Returns the Process on success or a NULL on any error.
 */
Process *scheduler_generate(char *command, int pid, int time_remaining, int is_sudo) {
	Process *newProcess=(Process *)malloc(sizeof(Process));
	// use malloc for command
	newProcess->command=(char *)malloc(sizeof(char)*strlen(command)+1);
	if (!newProcess->command){
   // memory allocation didnt work
		return NULL;
	}
	strcpy(newProcess->command,command);
	newProcess->pid= pid;
	newProcess->time_remaining=time_remaining;
	newProcess->time_last_run= clock_get_time();
	//set up flag to state created
	int flag=0x00000001;
	if(is_sudo==1){
		flag= 0x20|flag;
	}
	newProcess->flags=flag;

	return newProcess;
}

/* Select the next process to run from Ready List.
 * Follow the specification for this function.
 * Returns the process selected or NULL if none available or on any errors.
 */
Process *scheduler_select(Schedule *schedule) {
 	//find process with smallest time remaining in ready list
	//add remove
	Process* curr= schedule->ready_list->head;
	int count= schedule->ready_list->count;
	int least=0;
	int i=0;
	Process * returnval=curr;
	if (curr == NULL || returnval==NULL){
   // memory allocation didnt work
		return NULL;
	}
	
	if (least==0){
		//least hasn't been initilized
		least=curr->time_remaining;
		returnval=curr;
	}
	
	while(i<count && curr!= NULL){
		// check TIME_STARVATION 
		if ((clock_get_time() - curr->time_last_run) >= TIME_STARVATION){
			returnval=curr;
			remove_list(schedule->ready_list,returnval->pid);
			break;
		}
		//check if current is less the the lowest
		else if(curr->time_remaining <least){
			least=curr->time_remaining;
			returnval=curr;
		}
		//if end is reached
		if (curr->next==NULL){
			remove_list(schedule->ready_list,returnval->pid);
			return returnval;
		}
		i++;
		curr=curr->next;
	}
	//return the lowest
	return returnval;
}

/* Returns the number of items in a given List
 * Follow the specification for this function.
 * Returns the count of the List, or -1 on any errors.
 */
int scheduler_count(List *ll) {
	int count = -1;
	//if list exists return count otherwise return -1
	if(ll){
	count = ll->count;
	}
	return count;
}
void free_list(List *list){
	Process* temp = list->head;
	Process* curr;
	while (temp != NULL){
		//set current node = to next
		curr = temp->next;
		//free current node
		free(temp);
		//make the new temp node the current node
		temp = curr;
	}
}
/* Completely frees all allocated memory in the scheduler
 * Follow the specification for this function.
 */
void scheduler_free(Schedule *scheduler) {
	// free all lists then scheduler
	free_list(scheduler->ready_list);
	free_list(scheduler->stopped_list);
	free_list(scheduler->defunct_list);
	free(scheduler);
	return;
	
}

