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
  sprintf(buffer, "RSI: %s >", cwd);
  return buffer;
}

int main() {
  getcwd(cwd, BUFFER_SIZE); // store CWD
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

      // 2. If "cd", then change directory by chdir()
      if(strcmp(cmd, "cd"))
      {
        char* args = PopFromQueue();
        if(args != 0)
        {
          chdir(args);
        }
        else
        {
          chdir("~");
        }
        free(args);
      }


      // 3. Else, execute command by fork() and exec()

      EmptyQueue(); //Clean up anything left over in the queue
    }
  }
  printf("RSI:  Exiting normally.\n");
  free(prompt);
  return(0);
}