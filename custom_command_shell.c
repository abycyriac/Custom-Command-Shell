#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line
#define MAX_COMMAND_SIZE 255    // The maximum command-line size
#define MAX_NUM_ARGUMENTS 5     // Max number of arguements is set to five

int main()
{
  char * cmd_str = (char*) malloc( MAX_COMMAND_SIZE );
  char cmd[100],commands[15][100]; //to store last 15 commands
  int i=0, j=0,k=0, pid[15]; // pid to store last 15 pids
  pid_t child_pid;
  int status,ret=0; //ret to store return svalue from exec fuction
  char currentpth[1500]; //variable to store current directiory path
  char pth1[1500],pth2[1500],pth3[1500]; //variable to store different directiory path

  while( 1 )
  {
    // Print out the cmd prompt
    printf ("custom_command_shell> ");
    fflush(stdin);

    // Read the command from the commandline.  The
    // maximum command that will be read is MAX_COMMAND_SIZE
    // This while command will wait here until the user
    // inputs something since fgets returns NULL when there
    // is no input
    while( !fgets (cmd_str, MAX_COMMAND_SIZE, stdin) );

    /* Parse input */
    char *tk[MAX_NUM_ARGUMENTS];
    int   tk_count = 0;

    // Pointer to point to the token
    // parsed by strsep
    char *arg_ptr;
    char *working_str  = strdup( cmd_str );

    // we are going to move the working_str pointer so
    // keep track of its original value so we can deallocate
    // the correct amount at the end
    char *working_root = working_str;

    // Tokenize the input stringswith whitespace used as the delimiter
    while ( ( (arg_ptr = strsep(&working_str, WHITESPACE ) ) != NULL) &&
              (tk_count<MAX_NUM_ARGUMENTS))
    {
      tk[tk_count] = strndup( arg_ptr, MAX_COMMAND_SIZE );
      if( strlen( tk[tk_count] ) == 0 )
      {
        tk[tk_count] = NULL;
      }
        tk_count++;
    }


    //shell functionality
    if (!strcmp(tk[0],"listpids"))//printing pids
    {
      for (j = 0; j < i; j++)
        printf("%d: %d\n",j,pid[j]);
    }
    else
    {
      //storing pids
      if(i==15)
      {
        for(j=0;j<14;j++)
          pid[j]=pid[j+1];

        i=14;
      }
      child_pid= fork();
      pid[i]=child_pid;
      i++;


      if(!strcmp(tk[0],"exit")||!strcmp(tk[0],"quit"))
        return 0;

      getcwd(currentpth, sizeof(currentpth)); //getting current path of file
      strcpy(currentpth,"/");
      strcpy(pth1,"/usr/local/bin/");
      strcpy(pth2,"/usr/bin/");
      strcpy(pth3,"/bin/");

      if(child_pid==0)
      {
        strcat(currentpth,tk[0]);
        strcat(pth1,tk[0]);
        strcat(pth2,tk[0]);
        strcat(pth3,tk[0]);
        ret=execlp(currentpth,tk[0],tk[1],tk[2],tk[3],tk[4],tk[5],tk[6],tk[7],tk[8],NULL );
        ret=execlp(pth1,tk[0],tk[1],tk[2],tk[3],tk[4],tk[5],tk[6],tk[7],tk[8],NULL );
        ret=execlp(pth2,tk[0],tk[1],tk[2],tk[3],tk[4],tk[5],tk[6],tk[7],tk[8],NULL );
        ret=execlp(pth3,tk[0],tk[1],tk[2],tk[3],tk[4],tk[5],tk[6],tk[7],tk[8],NULL );

        printf("%s: command not found\n",tk[0]);
      }
    }

    //storing previous commands
    if(k==15)
    {
      for(j=0;j<14;j++)
        strcpy(commands[j],commands[j+1]);

      k=14;
    }
    strcpy(commands[k],tk[0]);// storing commands
    k++;

    waitpid( child_pid, &status, 0); //waiting for child process to end
    free( working_root );

    for (j = 0; j < k; j++)
        printf("%d: %s\n",j,commands[j]);



  }
  return 0;
}
