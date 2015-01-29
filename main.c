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

/*
  char* getPrompt()
  Retrieve current working directory (see getcwd())
  if successful, returns complete prompt with absoluate path = "RSI: [cwd] >"
*/
char cwd[BUFFER_SIZE];
char* getPrompt()
{
  char* buffer = malloc(sizeof(char) * BUFFER_SIZE);
  memset(buffer, '\0', BUFFER_SIZE); //Fill memory with null terminators
  getcwd(cwd, BUFFER_SIZE);
  sprintf(buffer, "RSI: %s >", cwd);
  return buffer;
}

int main() {
  char* prompt = getPrompt();
  int bailout = 1;

  while (bailout) {
    // Get user input (Print out the prompt)
    char* reply = readline(prompt);
    /* if user quits, exit loop */
    if (!strcmp(reply, "quit")) {
      bailout = 0;
    } else { // Execute user command

      // 1. Parse the user input contained in reply
      char* token = strtok(reply, " ");
      while (token) {
        PushToQueue(token);
        token = strtok(NULL, " ");
      }

      char* cmd = PopFromQueue();
      if(cmd == NULL)
      {
        continue; //User didnt enter anything - restart
      }

      // 2. If "cd", then change directory by chdir().
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

      // 3. Else, execute command by fork() and exec()

      //Read in the arguments from the queue and store them into an array.
      int numArgs = SizeOfQueue() + 1; //Add one for the first arg to be the cmd (IE: ECHO or MKDIR)
      char* args[numArgs + 1]; //Allocated enough space for everything in the array PLUS 1 (for a null terminator)
      int arrPos;

      args[0] = cmd;
      for(arrPos = 1; arrPos < numArgs; ++arrPos)
      {
        args[arrPos] = PopFromQueue();
      }
      args[numArgs] = 0; //null terminate it

      pid_t child = fork();
      if(child >= 0)
      {
        if(child == 0) //Execute the forked code in the child only.
        {
          if(execvp(cmd, args) < 0){
            perror("Error on execvp");
          }
          // printf("PID: %d\n", getpid());
          exit(0); //Child successfully exits. This is good.
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
      else
      {
        perror("fork"); //Display an error message
        exit(0);
      }
      usleep(500 * 1000); //Sleep for 500MS
    }
  }
  printf("RSI:  Exiting normally.\n");
  free(prompt);
  return(0);
}