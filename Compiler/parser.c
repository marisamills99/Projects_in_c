#include "lex.yy.c"
extern int linenumber;
extern char * funcName;
int lookahead = 0;
extern int strings;
extern int comments;
int functions=0;
int infunct= 0;
int parenthesis=0;

const char *syntax_err_msg = "Syntax Error: line %d\n";
const char *accept_msg = "Input Accepted\n";

const char *func_start = "Function %s: line %d\n\n";
const char *global_start = "Global: line %d\n\n";  

const char *func_count = "Number of functions defined: %d\n";
const char *comment_count = "Number of comments: %d\n";
const char *string_count = "Number of strings: %d\n";
void error() {
  printf(syntax_err_msg,linenumber);
  exit(-1);
}
// check if token = lookahead and increment if a match
int match(int token) {
  if (token == lookahead){
   lookahead = yylex();
   return 1;
  }
  //else error();
  return 0;
}
int checkList(){
   // param list (h,g,k)
   int x=1;
   while (match(COMMA)) {
      if(!match(ID)){
         x=0;
      }
   }
   return x;
}
int isParam(){
   int x=0;
   //for params in funct list
   if(match(ID)){
      x= checkList();
      return x;
   }
      //return checkList();
   
   return x;
}
int checkLeaf(){
   int x=0;
   while (match(OPENBRACE)){
      parenthesis++;
   }
         
   if (match(MINUS)){
      // do nothing just accept and increment
   }
   
   if (match(INT)){
      x= 1;
   }
   else if ( match(STRING) ){
      x= 1;
   }
   else if (match(ID)){
      //match id ()
      if (match(OPENBRACE)){
         parenthesis++;
         if ( match(CLOSEBRACE)){
            parenthesis--;
            x= 1;
         }
         // match id(f,g,h)
         else if (checkExpList() && match(CLOSEBRACE)){
            parenthesis--;
            x= 1;
         }
      } 

      x= 1;
   }
   
   while (match(CLOSEBRACE)){
      parenthesis--;
   }
      
      return x;

}
int isExp(){
   int returnval=1;
   //check for funct call 
   // check -factor
 
   int x=checkLeaf();
   // only proceed checking for math if it is not a singular expression
   if(lookahead ==MINUS || lookahead == PLUS || lookahead == MULTIPLY || lookahead == DIVIDE){
   while (x){

      if(!match(MINUS) && !match(PLUS) && !match(MULTIPLY) && !match(DIVIDE)){
         //return 0;
         break;
      }

      x=checkLeaf();
   }
   }
   
   return returnval;
   
}
int checkExpList(){
   //check that first term is a epression
   int x=isExp();
   //then check for comma followed by expresion
   while (match(COMMA)&&x) {
      if(!isExp()){
         x=0;
         return x;
      }
   }
   return x;
}
int checkLeafs(){
      //check that first term is a epression
   int x=checkLeaf();
   //then check for comma followed by expresion
   while (match(COMMA)) {
      if(!match(checkLeaf())){
         x=0;
         return x;
      }
   }
   return x;
}

int statement(){
   if(lookahead == ID){
      lookahead= yylex();
      if(match(OPENBRACE)){
         //match ()
         if(match(CLOSEBRACE)){
            return 1;
         }
         // match (exp)
         else if (checkExpList() )
         {
            if (parenthesis!=-1){
               error();
            }
            parenthesis =0;
            return 1;
         }
         //no match error
         else{
         error();
         }
         
      }
      //is it an input assignment 
      else if(match(ASSIGNMENT))
      {
         //match = input()
         if (match(INPUT)){   
            if (match(OPENBRACE)){
               parenthesis++;
               if(match(CLOSEBRACE))
               {
                  parenthesis--;
                  return 1;
               }
            }
         }
         //match = exp
         else if (isExp())
         {
            if (parenthesis!=0){
               linenumber--;
               error();
            }
            //matched
            return 1;
         }
         // no match
         else{
         error();
         }
      }
      //no match
      else{
      error();
      }
   }
   else if(match(PRINT) && match(OPENBRACE)){
      //lookahead= yylex();
      parenthesis++;
      if(isExp()){
         if (parenthesis!=0){
               linenumber--;
               error();
            }
            //matched
            return 1;
      }
      
   }
   //match return exp
   else if(match(RETURN)){
      if(isExp()){
         if (parenthesis!=0){
               linenumber--;
               error();
            }
         //matched
         return 1;
      }
   }
   else if (match(COMMENT))
   {
      return 1;
   }
   
   else{
      error();
   }
   return 0;
   
}
void statements(){
   // needs to have statements
   if (lookahead==ENDDEF){
      error();
   }
   //check statements
   while (lookahead != ENDDEF)
   {
      statement();
   }
   
   
}
int global(){
   //check for comment
   if (match(COMMENT)){
      return 1;
   }
    if(lookahead == ID){
      lookahead= yylex();
      if(match(OPENBRACE)){
         //match ()
         if(match(CLOSEBRACE)){
            linenumber--;
            //printf(global_start,linenumber);
            return 1;
         }
         // match (exp)
         else if (checkExpList() )
         {
            //offset the first open brace that was not counted
            if (parenthesis!=-1){
               error();
            }
            parenthesis =0;
            linenumber--;
            //printf(global_start,linenumber);
            return 1;
         }
         else{
         error();
         }
         
      }
      //is it an input assignment
     //is it an input assignment 
      else if(match(ASSIGNMENT))
      {
         //match = input()
         if (match(INPUT)){   
            if (match(OPENBRACE)){
               parenthesis++;
               if(match(CLOSEBRACE))
               {
                  parenthesis--;
                  linenumber--;
                  //printf(global_start,linenumber);
                  return 1;
               }
            }
         }
         //match = exp
         else if (isExp()){
            linenumber--;
            // decrement the line
            if (parenthesis!=0){
               error();
            }
            //there was a global found
           // printf(global_start,linenumber);
            return 1;
         }
         //if no match  error
         else{
         error();
         }
      }
      else{
         //if no match error
         error();
         }
         
   }
   //check if it is print
   else if(match(PRINT) && match(OPENBRACE)){
      //lookahead= yylex();
      parenthesis++;
      if(isExp()){
         linenumber--;
         if (parenthesis!=0){
               error();
            }
            //found a global
            //printf(global_start,linenumber);
            return 1;
      }
      //else an error
   else{
      error();
   }
   }
   return 0;
   
}
int funct(){
   // check for comments before functs
   while (lookahead== COMMENT){
         lookahead= yylex();
   }
   while (match(DEF)) {
    //THEN MATCH ID  ( parameters ) : statements enddef
    //lookahead = yylex();
    printf(func_start, funcName, linenumber);
    functions++;
    infunct=1;
    // check rest 
   if(match(ID)  && match(OPENBRACE)  && match(CLOSEBRACE) && match(COLON)){
      //printf("found  a match \n");
      statements();
      
   }
   else if(isParam() && match(CLOSEBRACE) && match(COLON)){
      
      statements();
   }
   //did function close
  if(match(ENDDEF)){
     infunct =0;
     //return 1;
  }
  //if no error
  else{
     error();
     return 0;
  }
}
  return 0;
}

void startprog(){
   funct();
}
void printEnd(){
   printf(func_count,functions);
   printf(comment_count, comments);
   printf(string_count, strings);
}
int main(int argc, char **argv) {
   lookahead = yylex();
   if (lookahead>0){ //yylex() returns 0 at EOF
	  
     
      //change to call global only after all functs
     funct();
     int x=global();
     if (x)
     {
        printf(global_start,linenumber);
     }
     
     while(lookahead >0){
   	x= global();
      if (!x)
      {
         error();
         return 0;
      }
      
     }
      
   }
   printEnd();
   fprintf(stderr,accept_msg);
   return 0;
}



