/* This is the only file you should update and submit. */

/* Fill in your Name and GNumber in the following two comment fields
 * Name: Marisa Mills
 * GNumber:01075052
 */

#include "logging.h"
#include "shell.h"
#include <stdlib.h>

/* List Struct Definition */
typedef struct joblist_struct {
  int jobId;    
  pid_t pid;
  int running; //execution state running or stopped
  char cmd[MAXLINE];	//inital command line argument
  //int jobs;	// count of current jobs
  struct joblist_struct* next;  //pointer to next item
  int is_fg;
} JobNode;


/* Constants */
static const char *shell_path[] = {"./", "/usr/bin/", NULL};
static const char *built_ins[] = 
			{"quit", "help", "kill", "jobs", "fg", "bg", NULL};
//job array
JobNode *jobsList=NULL;
int count=0;
int status;
// function definitions
void addNode(char* command, pid_t pid, int is_bg, int running);
void deleteNode(pid_t pid);
/* Feel free to define additional functions and update existing lines */
void redirect(char * argv[],char *path, char* path2, Cmd_aux* aux, char* cmd){
	int fd_in = -1,fd_out=-1;
	pid_t child = 0;
	// Open the  file for  reading writing or append
	if (aux->in_file){
		fd_in = open(aux->in_file, O_RDONLY ,0600);
		if (fd_in<0){
		log_file_open_error(aux->in_file);
		exit(0);
		}
	}
	if(aux->out_file){
		if(aux->is_append==0){
			fd_out = open(aux->out_file, O_WRONLY | O_CREAT |O_TRUNC ,0600);
		}
		else{
			fd_out = open(aux->out_file, O_WRONLY | O_CREAT| O_APPEND, 0600);
		}
		if (fd_out<0){
			log_file_open_error(aux->out_file);
			exit(0);
		}
	}
	// fork to create a new process that will be redirected out.
	child = fork();
	setpgid(0,0);
	// If it is the child...
	addNode(cmd, child,aux->is_bg,1);
	if(child == 0) {
		// check for out file
		if(aux->out_file){
			dup2(fd_out, STDOUT_FILENO);
		}
		if(aux->in_file){
			dup2(fd_in, STDIN_FILENO);
		}
		// Execute the process with path “/usr/bin/” and the command “./”
		if(execv(path, argv)< 0){
			if(execv(path2, argv)< 0){
				//log_command_error(path);
				//exit(-1);
			}			
		}
	}
	// log and wait for process
	if(aux->is_bg==0){
		log_start_fg(child, cmd);
		child = waitpid(child, &status, 0);
		log_job_fg_term(child, cmd);
		deleteNode(child);
	}
	else{ 
		/* don’t wait for bg job */
		//printf("%d %s", pid, command);
		log_start_bg(child, cmd);
	}
	
}

void printList(){
	printf("The list is \n\n");
	// temp is head of list
	JobNode *temp = jobsList;
	while (temp != NULL){
		printf("command is %s job count is %d pid is %d\n", temp->cmd,temp->jobId, temp->pid);
		temp = temp->next;
	}
	printf("\n");
}
void deleteNode(pid_t pid){
	JobNode* prev=NULL, *pointer= jobsList;
	while(pointer){
		if(pointer->pid==pid){	
			if (prev == NULL){
				jobsList = jobsList->next; // Changed head 
				return;				
			}
			//otherwise get rid of middle node
			else{
				prev->next=pointer->next;
			}
		}
		prev=pointer;
		pointer=pointer->next;
	}
}
void updateJobs(int signal,pid_t pid){
	JobNode* pointer= jobsList;
	//traverse until we find the correct pid 
	while(pointer){
		if (pointer->pid ==pid){
			//change fg to positive job id
			if(pointer->jobId ==0){
				pointer->jobId=++count;
			}
			break;
		}
		pointer= pointer->next;
	}
	switch(signal){
		case 9:
			deleteNode(pid);
			break;
		case 18:
			pointer->running =1;
			break;
		case 19:
			pointer->running =0;
			break;
	}
	
}
void addNode(char* command, pid_t pid, int is_bg, int running){
    /* allocate node */
    JobNode *new_node= ( JobNode*) malloc(sizeof(JobNode));
    /* put in whether fg or bg  */
	if(is_bg){
		new_node->jobId= ++count;
		new_node->is_fg= 0;
	}
	else{
		new_node->jobId=0;
		new_node->is_fg= 1;
	}
	// put data in node
	strcpy(new_node->cmd,command);
    new_node->pid=pid;
	new_node->running= running;
    new_node->next=NULL; 
	JobNode* pointer= jobsList; 
	
    /* If the List is empty, then make the new node as jobsList */
    if (jobsList==NULL) { 
		jobsList = new_node; 
		return; 
    } 
	
    //traverse until last node then add
    while (pointer->next != NULL) {
        pointer = pointer->next; 
	}
	pointer->next= new_node;
	
}
int checkToken(char *tok){
	// if token is a file or redirection let parse know
	
	// check the token
	int result = strcmp(tok, "<");
	if(result==0){
		//we must have a <....the following file is our input
		return 2;
	}
	result = strcmp(tok, ">");
	if(result==0){
		//we must have a >...the previous is redirected to next file
		return 3;
	}
	result = strcmp(tok, ">>");
	if(result==0){
		//we must have a >...Output from stream is appended to file
		return 4;
	}
	result = strcmp(tok, "&");
	if(result==0){
		//we must have a >...the previous is redirected to next file
		return 5;
	}
	return 0;
}
Cmd_aux* updateInfile(Cmd_aux *aux, char* tok){
	aux->in_file= malloc(MAXLINE);
	strcpy(aux->in_file,tok);
	return aux;
}
Cmd_aux* updateOutfile(Cmd_aux *aux, char* tok,int res){
	// update output files
    aux->out_file= malloc(MAXLINE);
	// if res is 3 no append if 4 yes append
	strcpy(aux->out_file,tok);
	if (res==4){
		aux->is_append= 1;
	}
	if (res==3){
		aux->is_append= 0;
	}
	return aux;
}
Cmd_aux* updateBackground(Cmd_aux *aux){
	aux->is_bg= 1;
	return aux;
}
Cmd_aux* initilizeAux(Cmd_aux *aux){
	// create an empty aux
	aux->in_file= NULL;
    aux->out_file= NULL;
    aux->is_append=-1;
    aux->is_bg= 0;
	return aux;
}
void parse(char *cmdline, char *argv[], Cmd_aux *aux){
	int i=0;
	char* tok;
	//char token[MAXLINE];
	// initilize this so cmdline is not changed
	char cmd_line[MAXLINE]; 
    // get first token 
	strcpy(cmd_line,cmdline);
    tok = strtok(cmd_line, " ");  // Checks for delimeter 
	aux=initilizeAux(aux);
    while (tok) { 
		int result= checkToken(tok);
		//check token returns 0 for anything that is not a file or redirect
		if (result==0){
			argv[i]=malloc(strlen(tok));
			strcpy(argv[i],tok);
			//argv[i]= token;
			i++;
		}
		else if(result==2){
			//we have an infile 
			tok = strtok(0, " "); 
			aux=updateInfile(aux, tok);
		}
		else if(result==3){
			//we have an output no append 
			tok = strtok(0, " "); 
			aux=updateOutfile(aux, tok,result);
		}
		
		else if(result==4){
			//we have an output yes append  
			tok = strtok(0, " "); 
			aux=updateOutfile(aux, tok,result);
		}
		else if(result==5){
			//we have a bg job 
			aux=updateBackground(aux);
		}
        // go through other tokens 
        tok = strtok(NULL, " "); 
		
    }
	argv[i]= '\0';
	i=0;
}
int checkBuiltin(char * command){
	// check if argv[0] is a built in command 
	int i=0;
	while(built_ins[i]){
		int res= strcmp(command,built_ins[i]);
		if(res==0){
			//the command is a built in 
			// return 0 for quit 1 for help 2 for kill etc
			return i;
		}
		i++;
	}
	return -1;
}

void killit(int sig){
	log_ctrl_c();
	JobNode* pointer=jobsList;
	while(pointer){
		if(pointer->jobId==0){
			pid_t pid =pointer->pid;
			//send signal to process
			kill(pid,SIGINT);
			log_job_fg_term_sig(pid, pointer->cmd);
			// delete process from head of the list
			deleteNode(pid);
			return;
		}
		pointer=pointer->next;
	}
	pointer=jobsList;
	while(pointer){
		if(pointer->is_fg==1){
			pid_t pid =pointer->pid;
			//send signal to process
			kill(pid,SIGINT);
			log_job_fg_term_sig(pid, pointer->cmd);
			// delete process from head of the list
			deleteNode(pid);
			break;
		}
		pointer=pointer->next;
	}

}
void stopit(int sig){
	log_ctrl_z();
	JobNode* pointer=jobsList;
	while(pointer){
		//search list for fg 
		if(pointer->jobId==0){
			pid_t pid =pointer->pid;
			//send signal to process
			kill(pid,SIGTSTP);
			//handle case if job ID is 0 
			if(pointer->jobId==0|| pointer->is_fg==1){
				updateJobs(19, pid);
			}
			log_job_fg_stopped(pid, pointer->cmd);
			return;
		}
		pointer=pointer->next;
	}
	pointer=jobsList;
	//search for a bg switched to fg 
	while(pointer){
		//doesnt't work if multiple 
		if(pointer->is_fg==1){
			pid_t pid =pointer->pid;
			//send signal to process
			kill(pid,SIGTSTP);
			//handle case if job ID is 0 
			if(pointer->jobId==0|| pointer->is_fg==1){
				updateJobs(19, pid);
			}
			log_job_fg_stopped(pid, pointer->cmd);
			break;
		}
		pointer=pointer->next;
	}
}
void reaper(int sig){
	int status =0;
	pid_t pid;
	JobNode* pointer= jobsList;
	//check the satus after wait and return the correct signal
	while((pid= waitpid(-1,&status, WNOHANG))>0){
	if(WIFSTOPPED(status)){
		while(pointer){
			// need to log based on fg or background
		 if(pointer->pid==pid){	
			if(pointer->jobId>0){
				log_job_bg_stopped(pid, pointer->cmd);
			}
			else{
				log_job_fg_stopped(pid, pointer->cmd);
			}
			return;
		 }
		 pointer=pointer->next;
		}
	  }
	else if(WIFSIGNALED(status)){
		while(pointer){
			// need to log based on fg or background
		 if(pointer->pid==pid){	
			if(pointer->jobId>0){
				log_job_bg_term_sig(pid, pointer->cmd);
			}
			else{
				log_job_fg_term_sig(pid, pointer->cmd);
			}
			deleteNode(pid);
			return;
		 }
		 pointer=pointer->next;
		}
	 }
	
	else if(WIFEXITED(status)){
		while(pointer){
			// need to log based on fg or background
		 if(pointer->pid==pid){	
			if(pointer->jobId>0){
				log_job_bg_term(pid, pointer->cmd);
				deleteNode(pid);
				return;
			}
		 }
		 
		 pointer=pointer->next;
		}
		//log_job_fg_term(jobsList->pid, jobsList->cmd);
		//deleteNode(pointer->pid);
		return;
	  }
	}
}

void nonBuiltin(char *command, char* argv[], Cmd_aux* aux){
	pid_t pid;
	int status;
	char *path = malloc(MAXLINE);
	char *path2= malloc(MAXLINE);
	//do not alter shell_path pointers and create both paths
	strcat(path,shell_path[0]);
	strcat(path,argv[0]);
	strcat(path2,shell_path[1]);
	strcat(path2,argv[0]);
	
	int bg=aux->is_bg;

	// is there a file redirection
	if(aux->in_file||aux->out_file){
		//if there needs to be redirection
		redirect(argv, path2, path, aux,command);
		return;
	}
	// check the path 
	else {
	  if((pid=fork()) == 0){ /* child runs user job */
		setpgid(0,0);
		if(execv(path, argv)< 0){
			if(execv(path2, argv)< 0){
				log_command_error(command);
				exit(0);
			}			
		}
		}
		addNode(command, pid,bg,1);
	}
	//check if fg or bg
	if (!bg) { 
		/* parent must wait for fg job to terminate */
		log_start_fg(pid, command);
		pid = waitpid(pid, &status, 0);
		if (pid>0){
			log_job_fg_term(pid, command);
			//get rid of the fg job 
			deleteNode(pid);
		}
		
	}				
	else{ 
		/* don’t wait for bg job */
		//printf("%d %s", pid, command);
		log_start_bg(pid, command);
	}
	
	
}

int countjobs(){
	//walk through list and count the number of jobs
	int num_jobs= 0;
	JobNode* pointer=jobsList;
	while(pointer){
		num_jobs++;
		pointer= pointer->next;
	}
	return num_jobs;
	
}
void jobs(){
	int num_jobs= 0;
	char state[10];
	JobNode* pointer=jobsList;
	num_jobs=countjobs();
	//log the job numbers 
	log_job_number(num_jobs);
	// log job details for each job in the list
	while(pointer){
		// is the job Running?
		if (pointer->running==1){
			strcpy(state,"Running");
		}
		else{
			strcpy(state,"Stopped");
		}
		log_job_details(pointer->jobId, pointer->pid,state,pointer->cmd);
		pointer=pointer->next;
	}
}

// built ins are {"quit", "help", "kill", "jobs", "fg", "bg", NULL};
void killProcess(int signal, pid_t pid){
	//find matching job
	JobNode* pointer= jobsList;
	while(pointer){
		if (pointer->pid ==pid){
			log_kill(signal, pid);
			// send correct signal to process
			kill(pointer->pid,signal);
			//update jobs list 
			updateJobs(signal,pid);
			break;
		}
		pointer= pointer->next;
	}
}
void switchfg(char* command, int jobId){
	int status;
	pid_t pid;
	JobNode* pointer= jobsList;
	// oterate through list
	while(pointer){
		// find matching job
		if (jobId==pointer->jobId){
			pid=pointer->pid;
			log_job_fg(pid, pointer->cmd);
			pointer->is_fg= 1;
			//if not running continue
			if(pointer->running==0){
				pointer->running =1;
				//send the signal to continue
				kill(pid,SIGCONT);
				log_job_fg_cont(pid, pointer->cmd);
				wait(&status);
			}
			/* parent must wait for fg job to terminate */
			pid = waitpid(pointer->pid, &status, 0);
			if (pid >0){
				log_job_fg_term(pid, pointer->cmd);
				//get rid of the fg job 
				deleteNode(pid);
			}
			return;
		}
		pointer=pointer->next;
	}
	log_jobid_error(jobId);
}
void switchbg(char* command, int jobId){
	//pid_t pid;
	JobNode* pointer= jobsList;
	while(pointer){
		//if we match job ids and find the stopped job
		if ((jobId==pointer->jobId)&& pointer->running==0){
			log_job_bg(pointer->pid, command);
			pointer->running=1;
			//send the signal to continue
			kill(pointer->pid, SIGCONT);
			//update jobsList
			updateJobs(SIGCONT,pointer->pid);
			//use kill to send a signal to a process 
			return;
		}
		pointer=pointer->next;
	}
	log_jobid_error(jobId);
}
int commands(char *command, char* argv[],Cmd_aux* aux){
	int res = checkBuiltin(argv[0]);
	//initilize or add to job array
	if(res>= 0){
		//we have a built-in
		if (res == 0){
			// the command was quit
			log_quit();
			exit(0);
		}
		else if (res == 1){
			// the command was help
			log_help();
		}
		else if (res == 2){
			// the command was kill
			int signal, pid;
			sscanf(argv[1], "%d", &signal);
			sscanf(argv[2], "%d", &pid);
			killProcess(signal,pid);
		}
		else if (res == 3){
			// the command was jobs
			jobs();
		}
		if (res == 4){
			// the command was fg
			int jobid;
			sscanf(argv[1],"%d", &jobid);
			switchfg(argv[0],jobid);
		}
		if (res == 5){
			// the command was bg
			int jobID;
			sscanf(argv[1],"%d",&jobID);
			switchbg(argv[0],jobID);
		}
	}
	else{
			//command is not built in
			nonBuiltin(command,argv,aux);
		}
	return 0;
}
void signalhandlers(){
	// signal handlers for SIGINT,SIGTSTP,SIGCHLD
	struct sigaction ctrlc;
	memset(&ctrlc, 0, sizeof(struct sigaction));
	ctrlc.sa_handler = killit;
	sigaction(SIGINT, &ctrlc, NULL);
	
	struct sigaction ctrlz;
	memset(&ctrlz, 0, sizeof(struct sigaction));
	ctrlz.sa_handler = stopit;
	sigaction(SIGTSTP, &ctrlz, NULL);
	
	struct sigaction chld;
	memset(&chld, 0, sizeof(struct sigaction));
	chld.sa_handler = reaper;
	sigaction(SIGCHLD, &chld, NULL);
}

/* main */
/* The entry of your shell program */
int main() 
{
    char cmdline[MAXLINE]; /* Command line */
	//static JobNode* jobsList= NULL;
    /* Intial Prompt and Welcome */
    log_prompt();
    log_help();
	
	//job array
	//JobNode* jobsList=NULL;
	// set up signal handlers
	signalhandlers();
	
    /* Shell looping here to accept user command and execute */
    while (1) {

        char *argv[MAXARGS]; /* Argument list */
        Cmd_aux aux; /* Auxilliary cmd info: check shell.h */
	/* Print prompt */
  	log_prompt();
	/* Read a line */
	// note: fgets will keep the ending '\n'
	if (fgets(cmdline, MAXLINE, stdin)==NULL)
	{
	   	if (errno == EINTR)
			continue;
	    	exit(-1); 
	}

	if (feof(stdin)) {
	    	exit(0);
	}
	
	/* Parse command line */
    cmdline[strlen(cmdline)-1] = '\0';  /* remove trailing '\n' */
	parse(cmdline, argv, &aux);		
	/* Evaluate command */
	commands(cmdline,argv, &aux);
    } 
}
/* end main */