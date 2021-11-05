%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "funct.h"

extern int yylineno;
extern int yylex();
int yyerror();


typedef struct symbol {
     int type;
  char * name;
  struct symbol *next;
  struct symbol *prev;
  } symbol;
symbol* headRef = NULL;
symbol* headRefglobal=NULL;
int infunct=0;
int isid=0;
symbol* lookup(symbol* headRef, char* name);
void assign(symbol** headRef,int type, char * name);
void printlist(symbol* headRef);
void printgloballist(symbol* headRef);
const char* pass_msg = "Input passed checking\n";                                   
const char* sem_err_msg = "Line %d: ID %s not defined\n";                       
const char* syn_err_msg = "Line %d: syntax error\n";   
int error();
%}
%union {
  struct symbol *symbol;
  char* name;
  int value;
}
%token TOKEN 
%token PRINT 
%token DEF 
%token IF 
%token ENDIF 
%token ENDDEF 
%token ELSE 
%token WHILE
%token ENDWHILE 
%token INPUT 
%token TRUE 
%token FALSE 
%token RETURN 
%token OPENBRACE 
%token CLOSEBRACE
%token COLON
%token COMMA
%token PLUS 
%token MINUS 
%token MULTIPLY 
%token DIVIDE 
%token LESS 
%token GREATER 
%token LESSEQUAL 
%token GREATEREQUAL 
%token EQUAL 
%token NOTEQUAL 
%token <name>ID 
%token STRING 
%token INT 
%token ASSIGNMENT 
%token COMMENT 
%type <value> factor rel_exp expression exp term

%%

program		    : function_list end_list {printgloballist(headRefglobal);}
              ;
function_list   : function_list function
                | {/* empty */}
                ;
function        : DEF ID{assign(&headRef,1,$2);assign(&headRefglobal,1,$2);} OPENBRACE arguments  {printlist(headRef); headRef=NULL;}
                ;
arguments       :       {infunct=1;}parameters  CLOSEBRACE  COLON statements ENDDEF//{printlist(headRef); headRef=NULL;printf("leaving funct\n");}
                |        CLOSEBRACE COLON{infunct=1;} statements ENDDEF//{printlist(headRef); headRef=NULL;printf("leaving funct\n");}
                ;
 
parameters      :       parameters  COMMA  ID  {assign(&headRef,2,$3);}
                |       ID                     {assign(&headRef,2,$1);}
                ;
statements      :statements statement 
                | statement
                ;
statement       : assignment_stmt 
                | print_stmt 
                | input_stmt 
                |condition_stmt 
                | while_stmt 
                | call_stmt 
                | return_stmt
                ;
assignment_stmt : ID ASSIGNMENT expression {if(!infunct){
                                              assign(&headRefglobal,$3,$1);
                                              } 
                                              else{
                                                assign(&headRef,$3,$1);
                                                }
                                                }
                ;
return_stmt     : RETURN exp
                ;
expression      :rel_exp {$$=4;}
                | exp 
                ;
rel_exp         :exp EQUAL exp 
                | exp NOTEQUAL exp 
                | exp LESS exp 
                | exp LESSEQUAL exp 
                |exp GREATER exp 
                | exp GREATEREQUAL exp 
                | OPENBRACE rel_exp CLOSEBRACE  
                ;
exp             :exp PLUS term {$$=2;}
                | exp MINUS term {$$ =2;}
                | term
                ;
term            :term MULTIPLY factor {$$ =2;}
                | term DIVIDE factor {$$=2;}
                | factor    
                ;
factor          :OPENBRACE exp  CLOSEBRACE  {$$=0;}
                | INT     { $$=2; /*assign(&headRef,2,yylval.name);printlist(headRef); */}
                | STRING { $$=3; /*assign(&headRef,3,yylval.name);printlist(headRef);*/ }
                | ID      {  symbol* x=lookup(headRef,$1); 
                              if(!x){
                              error($1);
                              } 
                              $$=x->type;
                          }
                | TRUE    {$$=4; /*assign(&headRef,4,yylval.name); printlist(headRef);*/}
                | FALSE    {$$=4; /*assign(&headRef,4,yylval.name); printlist(headRef);*/}
                |MINUS factor  {$$=$2;}           
                | call_stmt{$$=0;}
                ;
print_stmt      : PRINT OPENBRACE expression  CLOSEBRACE  
                ;
input_stmt      :ID ASSIGNMENT INPUT OPENBRACE  CLOSEBRACE  {if(!infunct){assign(&headRefglobal,2,$1);} else{assign(&headRef,2,$1);}}
                ;
call_stmt       :ID OPENBRACE  CLOSEBRACE   {  if(!lookup(headRef,$1)){
                                                  error($1);} 
                                              }
                | ID OPENBRACE exp_list  CLOSEBRACE  {  if(!lookup(headRef,$1)){
                                                        error($1);} 
                                                      }
                ;
condition_stmt  :if_head statements ENDIF 
                |if_head statements ELSE  COLON  statements ENDIF
                ;
if_head         :  IF expression  COLON 
                ;
while_stmt      : WHILE expression  COLON  statements ENDWHILE
                ;
exp_list        : exp_list  COMMA  exp 
                | exp
                ;
end_list        : statements
                ;



    
%%
symbol* lookup(symbol* headRef, char* name) {
  // search the local symbol table for the variable if it is not there then return 0
  symbol* nextnode = headRef;
  //printf("looking up %s\n",name);
  while(nextnode){
    if(strcmp(nextnode->name,name)==0){

      return nextnode;
    }
    //might get a null pointer error 
    nextnode= nextnode->next;
  }
  //check global symbol table
  nextnode=headRefglobal;
  while(nextnode){
    if(strcmp(nextnode->name,name)==0){

      return nextnode;
    }
    //might get a null pointer error 
    nextnode= nextnode->next;
  }
  //no match for global or local
  //fprintf(stderr, sem_err_msg, yylineno);
  return 0;
}
//1 = function 2= int 3= string 4= bool
void assign(symbol** headRef,int type, char * name)
{
    //make new nodes
    symbol* newnode = (symbol*) malloc(sizeof(symbol));
    symbol* prevnode;
    newnode->name = (char*) malloc((strlen(name)+1)*sizeof(char));
    if(*headRef==NULL) 
    {
        prevnode = newnode;
    }
    if(lookup(*headRef,name)){
      //already in list
      return;
    }
    //add in data
    strcpy(newnode->name,name);
    newnode->type=type;

    // The next of the new node is now head and previous is NULL 
    newnode->next = (*headRef);
    newnode->prev = NULL;
 
    // the prev of head node is new node 
    if ((*headRef) != NULL)
        (*headRef)->prev = newnode;
 
    // change head to new node
    (*headRef) = newnode;
    
}
void printgloballist(symbol* headRef){
  symbol* nextnode = headRef;
  //if list does not exist just return
  fprintf(stdout,"Symbols for scope: global\n");
  if(!nextnode){
    return;
  }
  // else go to end of list and print backward
  while(nextnode->next){
    nextnode= nextnode->next;
  }
  //print based on what data
  while(nextnode){
    switch(nextnode->type){
      case 1:
        fprintf(stdout,"%s: FUNC\n", nextnode->name);
        break;
      case 3:
        fprintf(stdout,"%s: STRING\n", nextnode->name);
        break;
      case 4:
        fprintf(stdout,"%s: BOOL\n", nextnode->name);
        break;
      default:
        fprintf(stdout,"%s: INT\n", nextnode->name);
        break;

    }
    //get next node
    nextnode=nextnode->prev;
  }
  fprintf(stdout,"\n");
}
void printlist(symbol* headRef){
  symbol* nextnode = headRef;
  //if list empty just return
  if(!nextnode){
    return;
  }
  // go to end of list and print backward
  while(nextnode->next){
    nextnode= nextnode->next;
  }
  //printf("Symbols for scope: func %s\n",nextnode->name);
  nextnode=nextnode->prev;
  //switch based on data type 
  while(nextnode){
    switch(nextnode->type){
      case 1:
        fprintf(stdout,"%s: FUNC\n", nextnode->name);
        break;
      case 3:
        fprintf(stdout,"%s: STRING\n", nextnode->name);
        break;
      case 4:
        fprintf(stdout,"%s: BOOL\n", nextnode->name);
        break;
      default:
        fprintf(stdout,"%s: INT\n", nextnode->name);
        break;

    }
    //get next node
    nextnode=nextnode->prev;
  }
    fprintf(stdout,"\n");

}
/*main and error functions*/
int main(){
    yyparse();
     fprintf(stderr, pass_msg);
    return 0;
}

int yyerror(){
   fprintf(stderr, syn_err_msg, yylineno);
   exit(-1);
}
int error(char * name){
   fprintf(stderr, sem_err_msg, yylineno,name);
   exit(-1);
}

