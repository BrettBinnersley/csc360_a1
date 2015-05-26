/*
Brett Binnersley,
V00776751
Csc 360
Assignment #1

Simple linked list - used for holding background process ids.
When a process terminates, if it was a background process it
will display a background process was termianted message
*/

struct ListItem {
  pid_t pid;
  struct ListItem* nextItem;
};

struct ListItem* frontOfList = NULL;

//Push an item to the global queue
void AddToList(pid_t pid)
{
  struct ListItem* newItem = malloc (sizeof (struct ListItem));
  newItem->pid = pid;
  newItem->nextItem = NULL;

  if(frontOfList == NULL)
  {
    frontOfList = newItem;
  }
  else
  {
    //Find the back of the queue
    struct ListItem* backOfQueue = frontOfList;
    while(backOfQueue->nextItem != NULL)
    {
     backOfQueue = backOfQueue->nextItem;
    }

    //Set the new item to be the back of the queue
    backOfQueue->nextItem = newItem;
  }
}

//Returns 1 if PID is in the list, else it returns false
int IsInList(pid_t pid){
  if(frontOfList == NULL)
  {
    return 0;
  }
  else
  {
    struct ListItem* iter = frontOfList;
    while(iter != NULL)
    {
      if(iter->pid == pid)
      {
        return 1;
      }
      else
      {
        iter = iter->nextItem;
      }
    }
    return 0;
  }
}
