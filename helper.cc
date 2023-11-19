 /******************************************************************
 * The helper file that contains the following helper functions:
 * check_arg - Checks if command line input is a number and returns it
 * sem_create - Create number of sempahores required in a semaphore array
 * sem_init - Initialise particular semaphore in semaphore array
 * sem_wait - Waits on a semaphore (akin to down ()) in the semaphore array
 * sem_signal - Signals a semaphore (akin to up ()) in the semaphore array
 * sem_close - Destroy the semaphore array
 ******************************************************************/

# include "helper.h"

bool queueIsEmpty() {
  if (front_index == end_index) {return true;}
  else return false;
}

ostream& operator<<(ostream &out, const Queue_Item &queue_item) {
  out << "Job ID: " << queue_item.jobid << "; Duration: " << queue_item.duration;
  return out;
}

void printQueue(Queue_Item* queue, int len_of_queue) {
  cout << "Job ID  : ";
  for (int i = 0; i < len_of_queue; i++) {
    cout << queue[i].jobid << " ";
  }
  cout << endl;
  cout << "Duration: ";
  for (int i = 0; i < len_of_queue; i++) {
    cout << queue[i].duration << " ";
  }
  cout << endl;
  cout << "front_index = " << front_index << endl;
  cout << "end_index = " << end_index << endl;
}

int check_arg (char *buffer)
{
  int i, num = 0, temp = 0;
  if (strlen (buffer) == 0)
    return -1;
  for (i=0; i < (int) strlen (buffer); i++)
  {
    temp = 0 + buffer[i];
    if (temp > 57 || temp < 48)
      return -1;
    num += pow (10, strlen (buffer)-i-1) * (buffer[i] - 48);
  }
  return num;
}

int sem_create (key_t key, int num)
{
  int id;
  if ((id = semget (key, num,  0666 | IPC_CREAT | IPC_EXCL)) < 0)
    return -1;
  return id;
}

int sem_init (int id, int num, int value)
{
  union semun semctl_arg;
  semctl_arg.val = value;
  if (semctl (id, num, SETVAL, semctl_arg) < 0)
    return -1;
  return 0;
}
// down()
void sem_wait (int id, short unsigned int num)
{
  struct sembuf op[] = {
    {num, -1, SEM_UNDO}
  };
  semop (id, op, 1);
}
// up()
void sem_signal (int id, short unsigned int num)
{
  struct sembuf op[] = {
    {num, 1, SEM_UNDO}
  };
  semop (id, op, 1);
}

int sem_close (int id)
{
  if (semctl (id, 0, IPC_RMID, 0) < 0)
    return -1;
  return 0;
}
