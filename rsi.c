/*
 * Brett Binnersley,
 * V00776751
 * Csc360, Assignment #1
 * Jan 30, 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include "SimpleQueue.h"
#include "SimpleLinkedList.h"


//Signal Handler for background processes - it will print out a message
static void sigchild_handler (int sig)
{
  int status;
  pid_t s_pid;

  while (1)
  {
    s_pid = waitpid(-1, &status, WNOHANG);
    if (s_pid <= 0)
      break;
    if(IsInList(s_pid))
    {
      printf("\nBackground process %d exited\n", s_pid);
    }
  }
}



char cwd[BUFFER_SIZE];
char* getPrompt()
{
  char* buffer = malloc(sizeof(char) * BUFFER_SIZE);
  memset(buffer, '\0', BUFFER_SIZE); //Fill memory with null terminators
  getcwd(cwd, BUFFER_SIZE);
  sprintf(buffer, "RSI: %s >", cwd);
  return buffer;
}

int main()
{
  char* prompt = getPrompt();
  int bailout = 1;


  struct sigaction act;
  memset (&act, '\0', sizeof(act)); //Fill will nulls
  act.sa_handler = sigchild_handler; //Function pointer
  if (sigaction(SIGCHLD, &act, 0))
  {
    perror ("sigaction");
    return 1;
  }

  while (bailout)
  {
    // Get user input (Print out the prompt)
    // char* reply = readline(prompt);
    char* reply = malloc(sizeof(char) * BUFFER_SIZE);
    memset(reply, '\0', BUFFER_SIZE); //Fill memory with null terminators
    printf("%s", prompt);
    fgets(reply, BUFFER_SIZE, stdin);
    reply[strlen(reply) - 1] = '\0';

    /* if user quits, exit loop */
    if (!strcmp(reply, "quit"))
    {
      bailout = 0;
    }
    else
    {
      /* Execute user command
      1. Parse the user input contained in reply
      */

      char* token = strtok(reply, " ");
      while (token)
      {
        PushToQueue(token);
        token = strtok(NULL, " ");
      }

      char* cmd = PopFromQueue();
      if(cmd == NULL)
      {
        continue; //User didnt enter anything - restart
      }

      /*
      2. If "cd", then change directory by chdir().
      */

      if(!strcmp(cmd, "cd"))
      {
        char* args = PopFromQueue();
        if(args != 0)
        {
          if(!strcmp(args, "~"))
          {
            chdir("/home"); //User went cd ~ (treat as home)
          }
          else
          {
            chdir(args);
          }
        }
        else
        {
          chdir("/home"); //No args -> change dir to home
        }
        free(args);
        prompt = getPrompt(); //Update with a new prompt
      }
      else
      {

        /*
        3. Else, execute command by fork() and exec()
        */

        //Read in the arguments from the queue and store them into an array.
        int numArgs = SizeOfQueue() + 1; //Add one for the first arg to be the cmd (IE: ECHO or MKDIR OR LS)
        char* args[numArgs + 1]; //Allocated enough space for everything in the array PLUS 1 (for a null terminator)
        int arrPos;

        args[0] = cmd;
        for(arrPos = 1; arrPos < numArgs; ++arrPos)
        {
          args[arrPos] = PopFromQueue();
        }
        args[numArgs] = 0; //null terminate it

        // Is there an & sign? If so -> run it as a background process, if not run it as a foreground
        // (blocking) process
        int foregroundProcess = 1;
        if(!strcmp(args[numArgs - 1], "&"))
        {
          foregroundProcess = 0;
          numArgs -= 1;
          args[numArgs] = 0;
        }


        pid_t child = fork();
        if(child >= 0)
        {
          if(child == 0) //Execute the forked code in the child only.
          {
            if(execvp(cmd, args) < 0)
            {
              printf("RSI: %s: command not found\n", cmd);
            }
            exit(0); //Child successfully exits. This is good.
          }
          else
          {
            int status;
            if(foregroundProcess)
            {
              waitpid(child, &status, 0); // WAIT FOR THE CHILD TO FINISH (BLOCKING - FOREGROUND PROCESS)
            }
            else
            {
              int retVal = waitpid(child, &status, WNOHANG); // WNOHANG - Background procees (non blocking)
              AddToList(child);
              if (retVal == -1)
              {
                perror("waitpid");
                exit(EXIT_FAILURE);
              }
            }

            // Clean up anything left over
            int i=0;
            for(i = 0; i<numArgs; ++i)
            {
              if(args[i])
              {
                free(args[i]);
                args[i] = NULL;
              }
            }
            EmptyQueue();
          }
        }
        else
        {
          perror("fork"); //Display an error message
          exit(0);
        }
      }
      usleep(200 * 1000); //Sleep for 500MS (So the input message can appear after)
    }
  }
  printf("RSI:  Exiting normally.\n");
  free(prompt);
  return(0);
}