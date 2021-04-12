/************************************************************************* 
 * 12/31/2016                                                            *
 * shell.c was downloaded from MTU:                                      *
 * http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html     *
 *                                                                       *
 * NOTE: The original code has quite ugly parse() routine with dangerous *
 * library routine gets(). To address these deficiences, I (Gang-Ryung)  *
 * rewrote main() and parse() routine using fgets and strtok.            *
 *************************************************************************/

/* ----------------------------------------------------------------- */
/* PROGRAM  shell.c                                                  */
/*    This program reads in an input line, parses the input line     */
/* into tokens, and use execvp() to execute the command.             */
/* ----------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define MAXCMD 64
#define MAXARG 256
#define MAXLINE 512
#define MAXLINEPLUS 513


extern FILE *stdin;                                                        //Setting all my global variables
extern FILE *stdout;
extern FILE *stderr;

static char *cmd_sep = ";";
static char *delim = " \t\n";
static char *ent = "\r";

int exit_flag = 0;

char *com[MAXARG];
char line[MAXLINEPLUS];
char *cmds[MAXCMD];

/* ----------------------------------------------------------------- */
/* FUNCTION  parse:                                                  */
/*    This function takes an input line and parse it into tokens.    */
/* It first replaces all white spaces with zeros until it hits a     */
/* non-white space character which indicates the beginning of an     */
/* argument.  It saves the address to argv[], and then skips all     */
/* non-white spaces which constitute the argument.                   */
/* ----------------------------------------------------------------- */

void reset_com(){
     memset(com,0,MAXARG);                                  //Basic reset functions for my pointers and arrays
}

void reset_cmds(){
     memset(cmds,0,MAXCMD);
}

void reset_line(){
     memset(line,0,MAXLINEPLUS);
}


void dump_com()
{
     int i;
     for(i=0; com[i]; i++)
     {
          printf("%s \n", com[i]);
     }
}

void dump_cmds()                                       //Helper functions for debugging
{
     int i;
     for(i=0; cmds[i]; i++)
     {
          printf("%s \n", cmds[i]);
     }
}

/* ----------------------------------------------------------------- */
/* FUNCTION execute:                                                 */
/*    This function receives a commend line argument list with the   */
/* first one being a file name followed by its arguments.  Then,     */
/* this function forks a child process to execute the command using  */
/* system call execvp().                                             */
/* ----------------------------------------------------------------- */

int getSize (char * s) {
    char * t;    
    for (t = s; *t != '\0'; t++)
        ;
    return t - s -1;
}

/* ----------------------------------------------------------------- */
/*                  The main program starts here                     */
/* ----------------------------------------------------------------- */

void ActualFunction()
{
     int i =0;                                                                            //Just initial variable declaration
     int j =0;

     char *token = NULL;
     char *ptr = NULL;
     char *endptr = NULL;
     pid_t newArray[256];
     int arraySize =0;
     int status;

      if(strlen(line) > MAXLINE)                                                          //Checking if input is too big
          {
               perror("Input is greater than max line length");
               return;
          }

          if(isspace(line[0]) || line[0] == 0 || line[0] == 13)                           //Checking for invalid input
               return;

          i =0;

          token = strtok(line, cmd_sep);                                                  // Getting first comand token from the line input
          if(strcmp(token, "quit\n")==0)                                                  //Setting an exit flag if the comand is quit
               exit_flag = 1;
          
          while(token !=NULL)                                                             //Parsing through the comands based on ;
          {
               if(strcmp(token," ")!=0)                                                   // Adding commadns to comand array if its not a " "
               {
                    cmds[i] = token;
                    i++;
               }
               token = strtok(NULL, cmd_sep);                                             //Incrementing my token
          }
          cmds[i] = NULL;

          for(i =0; cmds[i]; i++)                                                         //Looping through subsequent command array
          {
               j=0;
               token = strtok(cmds[i],delim);

               if(strcmp(token, "quit")==0)                                               //If one of the options is quit, setting an exit flag
                    exit_flag = 1;

               while(token != NULL)                                                       //Looping through options
               {
                    com[j++] = token;
                    token = strtok(NULL,delim);
               }
               com[j] = NULL;                                                             //Marking end of command
               
                pid_t pid;
                int status;

               if ((pid = fork()) < 0)                                                    //Calling fork and checing for error
                    { /* fork a child process           */
                    printf("*** ERROR: forking child process failed\n");
                    exit(1);
                    }
               else if (pid == 0)
                    { /* for the child process:         */                                //Further error checking
                    if (execvp(com[0], com) < 0)
                         { /* execute the command  */
                         printf("*** ERROR: exec failed\n");
                         exit(1);
                         }
                    }
               else
                    {                             
                    if ( WIFEXITED(status) )                                              //Outputting PID number and exit status for command
                         {
                         int es = WEXITSTATUS(status);
                         printf("PID %ld -  ", (long)getpid());
                         printf("Exit status was %d\n", es);
                         }
                    }                         
                   
               newArray[arraySize] = pid;                                                 //Capturing the pid to wait on later
               arraySize++;
   
               printf("Shell -> ");
          }

          reset_com();

          for(int i =arraySize-1; i>=0; i--)                                              //Looping through youngest fork to oldest to wait on them in turn and achieve parrele processing
               {
               waitpid(newArray[i], &status,0);
               }       
          reset_line();                                                                   //Resetting arrays
          reset_cmds();




}

int main(int argc, char **argv1)
{
      
     if (argc >= 2)                                              //Checking if there is a given file input                                       
          {
          FILE *fp;
          fp = fopen(argv1[1], "r");                             // If so, opening the  file and reading from it 
          printf("Shell -> ");
          while(fgets(line, sizeof(line), fp))
               {
               ActualFunction();
               if(exit_flag==1) break;
               }
          }
     else
          {                                                      //If there is not a file, read from stdin and call function
          printf("Shell -> ");
          while(fgets(line, sizeof(line), stdin))
               {
               ActualFunction();
               if(exit_flag==1) break;
               }
          }
     
     return 0;
}
