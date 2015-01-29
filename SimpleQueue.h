#define BUFFER_SIZE 1024

struct QueueItem {
  char* text;
  struct QueueItem* nextItem;
};

struct QueueItem* frontOfQueue = 0;

//Push an item to the global queue
void PushToQueue(char* str)
{
  struct QueueItem* newItem = malloc (sizeof (struct QueueItem));
  newItem->text = malloc(sizeof(char) * BUFFER_SIZE);
  newItem->nextItem = 0;
  sprintf(newItem->text, "%s", str);

  if(frontOfQueue == 0)
  {
    frontOfQueue = newItem;
  }
  else
  {
    //Find the back of the queue
    struct QueueItem* backOfQueue = frontOfQueue;
    while(backOfQueue->nextItem != 0)
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
  if(frontOfQueue == 0)
  {
    return 0;
  }
  else
  {
    char* text = frontOfQueue->text;
    struct QueueItem* next = frontOfQueue->nextItem;
    free(frontOfQueue);
    frontOfQueue = next;
  }
}

//Clean up and free memory for anything that is left over in the queue
void EmptyQueue() {
  char* result = PopFromQueue();
  while(result != 0)
  {
    free(result);
    result = PopFromQueue();
  }
}