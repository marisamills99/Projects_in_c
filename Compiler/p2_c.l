%{
#include <stdio.h>
#include <stdlib.h>
#include<string.h> 
#include "tokens.h"

/* error messages */
const char *token_err_msg = "Unknown Token: line %d\n";
extern int linenumber;
extern char *funcName; 
extern int strings;
extern int comments;
/* add your variable/function declarations here */
void printerror() {
    	fprintf(stderr,token_err_msg, linenumber);
		exit(1);
    }

  
   
   
%}

%option never-interactive 
	/*supress unnecessary warning*/

 /* lex comments: cannot start with the leftmost location of a line */
 /* add your regular definitions here*/


%%

"print" 		return(PRINT); /* keyword print */ 
"def"   		return (DEF);
"if"		    return(IF);
"endif"	  		return(ENDIF);
"enddef"		return(ENDDEF);
"else"			return(ELSE);
"while"	  		return(WHILE);
"endwhile"  	return(ENDWHILE);
"input"	  		return(INPUT);
"true"	  		return(TRUE);
"false"	  		return(FALSE);
"return"		return(RETURN);

"("		    			return OPENBRACE;
")"	    				return CLOSEBRACE;
":"		    			return COLON;
","		    			return COMMA;

"="						return ASSIGNMENT;
"+"		    			return PLUS;
"-"		    			return MINUS;
"*"		    			return MULTIPLY;
"/"						return DIVIDE;

"<"	    							return LESS;
">"		    						return GREATER;
"<="								return LESSEQUAL;
">="								return GREATEREQUAL;
"=="								return EQUAL;
"<>"								return NOTEQUAL;

[a-zA-Z_]+([0-9]|[a-zA-Z_])*  	{funcName= strdup(yytext);
									return ID ;}
["](?:[^"\\]|\\.)*["]		{strings++; return STRING;}
0{0,1}([1-9][0-9]*)|0				return INT;

!([\x20-\x7E])*				{ comments++; return COMMENT;}

[\n] 						linenumber++;
[ \t\r]+                    ;
.                  {printerror(); exit(-1);}
%%



