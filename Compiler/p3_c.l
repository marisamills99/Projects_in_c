%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "p3.tab.h"


 int infunct;
 int functDecl;
const char *token_err_msg = "Line %d: lexical error\n";
void printerror() {
    	fprintf(stderr,token_err_msg, yylineno);
		exit(-1);
    }
%}

 /* regular definitions imported from token.h
 

	*/
%option never-interactive
%option yylineno
%%


 /* keywords */
"print" 		return(PRINT); /* keyword print */ 
"def"   		{functDecl=1;return (DEF);}
"if"		    return(IF);
"endif"	  		return(ENDIF);
"enddef"		{infunct=0; return(ENDDEF);}
"else"			return(ELSE);
"while"	  		return(WHILE);
"endwhile"  	return(ENDWHILE);
"input"	  		return(INPUT);
"true"	  		return(TRUE);
"false"	  		return(FALSE);
"return"		return(RETURN);
 /* operators */

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
 /* punctuations */
"("		    			return OPENBRACE;
")"	    				return CLOSEBRACE;
":"		    			{functDecl=0; return COLON;}
","		    			return COMMA;
 /* other tokens */
[a-zA-Z_]+([0-9]|[a-zA-Z_])*  	{yylval.name = (char*)malloc(strlen(yytext)+1);
                 				strcpy(yylval.name,yytext); return ID ;}
["](?:[^"\\]|\\.)*["]		return STRING;
0{0,1}([1-9][0-9]*)|0				{return INT;}

!([\x20-\x7E])*				;


 /* white spaces and other  .= error*/

[ \n\t\r]+                    ;
.                  { printerror(); exit(-1);}
%%


