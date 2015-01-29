#define BUFFER_SIZE 1024

struct QueueItem {
  char* text;
  struct QueueItem* nextItem;
};

struct QueueItem* frontOfQueue = NULL;

//Push an item to the global queue
void PushToQueue(char* str)
{
  struct QueueItem* newItem = malloc (sizeof (struct QueueItem));
  newItem->text = malloc(sizeof(char) * BUFFER_SIZE);
  newItem->nextItem = NULL;
  sprintf(newItem->text, "%s", str);

  if(frontOfQueue == NULL)
  {
    frontOfQueue = newItem;
  }
  else
  {
    //Find the back of the queue
    struct QueueItem* backOfQueue = frontOfQueue;
    while(backOfQueue->nextItem != NULL)
    {
     backOfQueue = backOfQueue->nextItem;
    }

    //Set the new item to be the back of the queue
    backOfQueue->nextItem = newItem;
  }
}

//Returns the string from the front of the queue or null if none exists.
//Frees the resources EXCEPT for the text pointer returned;
char* PopFromQueue()
{
  if(frontOfQueue == NULL)
  {
    return NULL;
  }
  else
  {
    char* text = frontOfQueue->text;
    struct QueueItem* next = frontOfQueue->nextItem;
    free(frontOfQueue);
    frontOfQueue = next;
    return text;
  }
}

int SizeOfQueue(){
  if(frontOfQueue == NULL)
  {
    return 0;
  }
  else
  {
    int size = 1;
    struct QueueItem* iter = frontOfQueue;
    while(iter->nextItem != NULL)
    {
     iter = iter->nextItem;
     ++size;
    }
    return size;
  }
}

//Clean up and free memory for anything that is left over in the queue
void EmptyQueue() {
  char* result = PopFromQueue();
  while(result != NULL)
  {
    free(result);
    result = PopFromQueue();
  }
}