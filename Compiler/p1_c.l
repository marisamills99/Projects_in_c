 /* CS440 Proj 1 Spring 2021
    Name:Marisa Mills
    G#:01075052
    Language: C

    Note:
  */
%option caseless
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* error messages */
const char *col_num_err_msg = "Line %d: incorrect number of columns!\n";
const char *tbl_err_msg = "Line %d: more than one table!\n";
const char *header_err_msg = "Line %d: non-header cell!\n";

/* add your variable/function declarations here */
int rowcount=0,columncount=0,count=0,rownum=1,column_counter, tablecounter=0, linecounter=1,header_seen=0;
double totalCount=0.00,total=0.00, max,min;
double temp=9.9;
double *arr;
//to remove only tags and leading / trailing whitespace
char *substring(char* str){
        int i = 0,j=0;
        int length = strlen(str);
        //for null terminator
        char *str2 = malloc(length+1);
        for (i=4; i < length-5;i++){
          //j>0 when a char has been seen
          if (j>0 ||(str[i]!= ' '&&str[i]!= '\t')){ 
            str2[j] = str[i];
            j++;
          }
        }
        j=strlen(str2)-1;
        //remove 
        while( (str2[j]== ' '||str2[j]== '\t')){
          str2[j]='\0';
          j--;
        }
        return str2;
    }
//to remove tags and spaces
char *substringNum(char* str){
      //parse out the extra tags
        int i = 0, j=0;
        int length = strlen(str);
        char *str2 = malloc(length+1);
        //skip <td> and </td> start at index 
        for (i=4; i < length-5;i++)
        {
          if (str[i]!= ' '&&str[i]!= '\t'){ 
            str2[j] = str[i];
            j++;
          }

        }
        return str2;
    }
    //to insert the "|--|" on the second row
  char * insertExtraRow(char * text){
    //printf("entered row count = %d columns= %d\n",rowcount,columncount);
    char *str = malloc(100);
    if(rowcount==1){
      int x=0;
      while(x<=columncount){
        strcat(str,text);
        x++;
      }
      strcat(str,"\n");
      return str; 
    }
    return str;
  }
//create the string for the extra col 1,2,3,4...
  char * insertExtraCol(){
    char mynum[2];
    //printf(" count = %d columns= %d\n",count,columncount);
    char *str = malloc(100);
    if(count==0){
      if(rownum==1){
        strcat(str,"|");
      }
      // check if row num <row count
      sprintf(mynum,"%d",rownum);
      strcat(str,mynum);
      strcat(str,"|");
      count++;
      rownum++;
    }
      return str; 
  }
  //add extra header 
    char * insertExtraHeader(){
      char *str = malloc(100);
      if(count==0){
        strcat(str,"Header|");
        count++;
      }
      return str; 
    }
    // Remember arr[i*M + j]= arr[I][j]
    //insert the extra math rows into the table 
    // arr={[count,mean, min,max],[count,mean, min,max],[count,mean, min,max]}
    void insertMath(){
        int i=0,j=0;
        printf("|Count|");
        while(i<columncount){
        printf("%.2lf|", arr[i*4 + j]);
        i++;
        }
        i=0;
        printf("\n");
        j++;
        printf("|Mean|");
        while(i<columncount){
          if(arr[i*4 + (j-1)]){
            printf("%.2lf|", (arr[i*4 + j]/arr[i*4 + (j-1)]));
          }
          else{
            printf("N/A|");
          }
        i++;
        }
        i=0;
        printf("\n");
        j++;
        printf("|Min|");
        while(i<columncount){
          if(arr[i*4 + (j-2)]){
            printf("%.2lf|", arr[i*4 + j]);
          }
          else{
            printf("N/A|");
          }
        i++;
        }
        i=0;
        printf("\n");
        j++;
        printf("|Max|");
        while(i<columncount){
          if(arr[i*4 + (j-3)]){
            printf("%.2lf|", arr[i*4 + j]);
          }
          else{
            printf("N/A|");
          }
        i++;
        }
        i=0;
        printf("\n");
        j++;
        
   }
   //malloc the array
    void createList(){
      //always has 4 columns (mean count min and max)
      arr = malloc(columncount*4*sizeof(double));
      // check the result of malloc for NULL
      if(arr == NULL) {
        fprintf(stderr,"malloc failed to allocate memory\n");
        exit(0);
      }

    }
    //check and update max
    void checkMax(double newnum,int rownumber){
      if(!arr[rownumber*4 + 3]){
        arr[rownumber*4 + 3]=newnum;
      }
      else{
        max= arr[rownumber*4 + 3];
        if(newnum>max){
          arr[rownumber*4 + 3]=newnum;
        }
      }
    }
    //check and update min 
    void checkMin(double newnum, int rownumber){
      if(!arr[rownumber*4 + 2]){
        arr[rownumber*4 + 2]=newnum;
      }
      else{
        min= arr[rownumber*4 + 2];
        if(newnum<min){
          arr[rownumber*4 + 2]=newnum;
        }
      }
    }
   //update the math list with correct count mean min max
   //arr[i*M + j], this is equal to  arr[I][j]
    void updateList(float num){
      //printf("\n rowcount= %d columncount = %d count = %d word is %.2lf\n", rowcount,columncount, column_counter, num);
      if(arr==NULL){
        createList();
      }
      //reset row count to not count header and extra |---|
      int i=column_counter;
      //count ++ array[row][0] total+= array[row][1] 
      arr[i*4+0]+=1.0;
      arr[i*4+1]+=num;
      checkMax(num,i);
      checkMin(num,i);
      
    }
    // check if a string has pipe symbol and handle printing it
    int checkString(char * str){
      int hasPipe = strchr(str, '|') != NULL;
        if (!hasPipe){
            return 0;
        }
      char string[50];
      strcpy(string,str);
      // Extract the first token
      char * token = strtok(string, "|");
      //printing each token
      // loop through the string to extract all other tokens
      do {
        printf( "%s\\|", token ); //printing each token
        token = strtok(NULL, "|");
      }while(token != NULL && (strchr(token, '|') != NULL));
      printf( "%s|", token );
      return 1;
    }
    //check if the table tag has been seen more than once
  int checktables(){
    if(tablecounter>1){
      fprintf(stderr, "Line %d: more than one table!\n", linecounter);
      return 1;
    }
    return 0;
  }
  
%}

%option never-interactive 
	/* supress unnecessary warning */
  /* lex comments: cannot start with the leftmost location of a line */
  /* add your regular definitions here  */
/* these are self explanatory*/
 /*table can have style elements so I changed the definition*/
TABLESTART "<table"
TABLEEND "</table>"
HEADSTART "<th>"
HEADEND "</th>"
ROWSTART "<tr>"
ROWEND "</tr>"
DATASTART "<td>"
DATAEND "</td>"
/*never needed this included in float*/
WSPACE [" \t"]
/*optional sign must be followed by a num then a optional decimal and more nums  surronding white space is allowed*/
FLOAT [" \t"]*[-+]?([0-9]+\.[0-9]+|[0-9]+)[" \t"]*
/*a typical word is [A-Za-z ours has nums and symbols period comma and white space included*/
WORD [A-Za-z0-9\-%+|,.;" \t"]+

%%

{TABLESTART} {tablecounter++;if(checktables()){exit(-1);};}
{ROWSTART} {count=0; column_counter=0;
            printf("|");
            printf("%s",insertExtraRow("---|"));
            rowcount++;
            }
            

{DATASTART}{FLOAT}{DATAEND} { 
                              printf("%s",insertExtraCol());
                              updateList(atof(substringNum(yytext)));
                              printf("%s|",substringNum(yytext));
                              column_counter++;
                              if(!header_seen){
                                fprintf(stderr,"Line %d: non-header cell!\n",linecounter);
                                exit(-1);
                                }
                              if(column_counter>columncount){
                                fprintf(stderr,"Line %d: incorrect number of columns!\n",linecounter);
                                exit(-1);
                                }
                              }

{DATASTART}{WORD}{DATAEND} {printf("%s",insertExtraCol());
                              int printed=checkString(substring(yytext));
                              if (!printed){printf("%s|",substring(yytext));}
                              column_counter++;
                              if(!header_seen){
                                fprintf(stderr,"Line %d: non-header cell!\n",linecounter);
                                exit(-1);
                                }
                              if(column_counter>columncount){
                                fprintf(stderr,"Line %d: incorrect number of columns!\n",linecounter);
                                exit(-1);
                                }
                              }
{DATASTART}{DATAEND}         {printf("%s",insertExtraCol());
                              //check for empty tags
                              printf("%s|",substring(yytext));
                              column_counter++;
                              if(!header_seen){
                                fprintf(stderr,"Line %d: non-header cell!\n",linecounter);
                                exit(-1);
                                }
                              if(column_counter>columncount){
                                fprintf(stderr,"Line %d: incorrect number of columns!\n",linecounter);
                                exit(-1);
                                }
                              }

{HEADSTART}{WORD}{HEADEND}    {printf("%s",insertExtraHeader());
                              printf("%s|", substring(yytext));
                              columncount++;
                              header_seen++;
                              }
{HEADSTART}{FLOAT}{HEADEND}   { printf("%s",insertExtraHeader());
                              printf("%s|", substring(yytext));
                              columncount++;
                              header_seen++;
                              }
{ROWEND} printf("\n");
{TABLEEND} {if(!arr){createList();memset(arr, 0, sizeof(arr));}insertMath();}
. ;
[ \n] {linecounter++;};
%%


int main(int argc, char** argv) {
  
  yylex();
  free(arr);
  return 0;
  }

