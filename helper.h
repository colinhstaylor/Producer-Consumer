/******************************************************************
 * Header file for the helper functions. This file includes the
 * required header files, as well as the function signatures and
 * the semaphore values (which are to be changed as needed).
 ******************************************************************/

/* SOURCES:
Circular Queue implementation: GeeksForGeeks.org https://www.geeksforgeeks.org/circular-queue-set-1-introduction-array-implementation/
*/

# ifndef HELPER_H
# define HELPER_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <sys/sem.h>
# include <sys/time.h>
# include <math.h>
# include <errno.h>
# include <string.h>
# include <pthread.h>
# include <ctype.h>
# include <iostream>
using namespace std;

# define SEM_KEY 0x66 // Changed this number from 0x50

# define MUTEX_INDEX 0
# define SPACE_INDEX 1
# define ITEM_INDEX 2

//union semun { //commented out for macbook
 //   int val;               /* used for SETVAL only */
   // struct semid_ds *buf;  /* used for IPC_STAT and IPC_SET */
    //ushort *array;         /* used for GETALL and SETALL */
//};

static int front_index = 0; // consumer
static int end_index = 0; // producer

// check this works

bool queueIsEmpty();

struct Queue_Item {
  int jobid;
  int duration;
  friend ostream& operator<<(ostream &out, const Queue_Item &queue_item);
};

// circular queue implementation with 'Queue_Item' elements and an element returning deQueue function
class Circular_Queue {
  int front = -1, end = -1;
  int size = 0;
  Queue_Item *queue = nullptr;
public:
  Circular_Queue() {};
  Circular_Queue(int size) : size(size) {
    queue = new Queue_Item[size];
  }
  ~Circular_Queue() { delete[] queue; }
  Queue_Item getFront() { return queue[front]; }
  Queue_Item getEnd() { return queue[end]; }
  void enQueue(Queue_Item item) {
    if ((front == 0 && end == size-1) || (end == (front-1)%(size-1))) {
      // full
      cout << "queue full. cannot add" << endl;
      return;
    }
    else if (front == -1) { // from empty
      front = end = 0;
    }
    else if (end == size - 1 && front != 0) { // loop around
      end = 0;
    } 
    else { // normal behaviour
      end++;
    }
    queue[end] = item;
  }
  Queue_Item deQueue() {
    if (front == -1) {
      cout << "cannot take item. queue is empty" << endl;
      return Queue_Item();
    }
    Queue_Item temp = queue[front];
    queue[front] = Queue_Item();
    if (front == end) { // removing last item
      front = end = -1;
      }
    else if (front == end - 1) { // front loops back around
      front = 0;
    }
    else { // normal behaviour
      front++;
    }
    return temp;
  }
  void printQueue() {
    for (int i = 0; i<size; i++) {
      cout << queue[i] << endl;
    }
  }
};

void printQueue(Queue_Item* queue, int len_of_queue);

struct Producer_Parameter {
  int sem_arr_key;
  int num_of_jobs;
  //Queue_Item* queue;
  Circular_Queue* queue;
  int len_of_queue;
};

struct Consumer_Parameter {
  int sem_arr_key;
  //Queue_Item* queue;
  Circular_Queue* queue;
  int len_of_queue;
};

// read int from cmd line arg
int check_arg (char *);
// create array
int sem_create (key_t, int);
// intialise element of the array
int sem_init (int, int, int);
// down
void sem_wait (int, short unsigned int);
// up
void sem_signal (int, short unsigned int);
// destroy array
int sem_close (int);

# endif
