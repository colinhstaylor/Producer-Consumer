/******************************************************************
 * The Main program with the two functions. A simple
 * example of creating and using a thread is provided.
 ******************************************************************/

#include "helper.h"

void *producer (void *id);
void *consumer (void *id);

int main (int argc, char **argv)
{

  

  // TODO // PROGRAM SKETCH //



  // read in command line arguments:

    int LENGTH_OF_QUEUE = check_arg(argv[1]);
    if (LENGTH_OF_QUEUE < 0) {cerr << "fail1" << endl; exit(1);}
    int NUMBER_OF_JOBS = check_arg(argv[2]);
    if (NUMBER_OF_JOBS < 0) {cerr << "fail2" << endl; exit(1);}
    int NUMBER_OF_PRODUCERS = check_arg(argv[3]);
    if (NUMBER_OF_PRODUCERS < 0) {cerr << "fail3" << endl; exit(1);}
    int NUMBER_OF_CONSUMERS = check_arg(argv[4]);
    if (NUMBER_OF_CONSUMERS < 0) {cerr << "fail4" << endl; exit(1);}
    
  // set up and initialise data structures and variables

    //  CREATE CIRCULAR QUEUE

    //Queue_Item queue[LENGTH_OF_QUEUE];
    Circular_Queue *queue = new Circular_Queue(LENGTH_OF_QUEUE);

    // VARIABLES


  // set up and initialise semaphores

    // SEM_CREATE CREATES THE ARRAY (3 needed)
    // INACCESSIBLE TO PRODUCER/CONSUMER FUNCTION
    int sem_arr_key = sem_create(SEM_KEY, 3);
    if (sem_arr_key < 0) { cerr << "fail5" << endl; exit(1); }

    // SEM_INIT TO INITIALISE EACH ELEMENT 
      // MUTEX, 1
      // SPACE, check_arg(argv[0])
      // ITEM, 0

    if (sem_init(sem_arr_key, MUTEX_INDEX, 1) < 0) {cerr << "fail6" << endl; exit(1);}
    if (sem_init(sem_arr_key, SPACE_INDEX, LENGTH_OF_QUEUE) < 0) {cerr << "fail7" << endl; exit(1);}
    if (sem_init(sem_arr_key, ITEM_INDEX, 0) < 0) {cerr << "fail8" << endl; exit(1);}

  // create producers and consumers

    pthread_t producerid;
    Producer_Parameter producer_parameter;
    producer_parameter.sem_arr_key = sem_arr_key;
    producer_parameter.num_of_jobs = NUMBER_OF_JOBS;
    producer_parameter.queue = queue;
    producer_parameter.len_of_queue = LENGTH_OF_QUEUE;

    pthread_create (&producerid, NULL, producer, (void *) &producer_parameter);

    pthread_t consumerid;
    Consumer_Parameter consumer_parameter;
    consumer_parameter.sem_arr_key = sem_arr_key;
    consumer_parameter.queue = queue;
    consumer_parameter.len_of_queue = LENGTH_OF_QUEUE;

    pthread_create (&consumerid, NULL, consumer, (void *) &consumer_parameter);

    
    // PTHREAD_CREATE?

        // each producer and consumer runs in its own thread

  // quit ensuring processes are cleaned up

    // PTHREAD_HOLD?
    pthread_join (producerid, NULL);
    cout << "Doing some work after the producer join" << endl;
    pthread_join (consumerid, NULL);
    cout << "Doing some work after the consumer join" << endl;
    delete queue;
    if (sem_close(sem_arr_key) < 0) {cerr << "failed to close semaphore array" << endl; exit(1);}
    pthread_exit(NULL); // NULL ? // ends main


  // END PROGRAM SKETCH //



  // EXAMPLE GIVEN: creating and using a thread
  /*
  {
    pthread_t producerid;
    int parameter = 5;

    pthread_create (&producerid, NULL, producer, (void *) &parameter);

    pthread_join (producerid, NULL);

    cout << "Doing some work after the join" << endl;
  }
  */
  // END EXAMPLE //


  return 0;
}

void *producer (void *parameter) // Producer_Parameter
{
  cout << "Entering producer function" << endl;
  // TODO

  // initialise parameters as required

  Producer_Parameter *param = (Producer_Parameter *) parameter;
  int sem_id = param->sem_arr_key;
  int num_of_jobs = param->num_of_jobs;
  //Queue_Item* queue = param->queue;
  Circular_Queue* queue = param->queue;
  //int len_of_queue = param->len_of_queue;

  // add jobs to the queue

    // in synchronous fashion described in lectures:
    int count = 0; // in place of condition
    while (true) {
      // produce item;
        // contains job ID and a duration (which is the job)
        Queue_Item item;
        item.jobid = count; //
        item.duration = count; //
      // deposit item;
        //cout << "waiting for space" << endl;
        sem_wait(sem_id, SPACE_INDEX);
        //cout << "waiting for mutex" << endl;
        sem_wait(sem_id, MUTEX_INDEX);

        // producer adds to the end
        //queue[end_index] = item;
        count++;
        //end_index = (end_index+1)%len_of_queue;

        queue->enQueue(item);
        // print status
        //cerr << "Producer D: Job ID " << item.jobid << " duration " << item.duration << " at position " << end_index << endl; // how to ref to this producer number
        
        sem_signal(sem_id, MUTEX_INDEX);
        sem_signal(sem_id, ITEM_INDEX);
      cout << "Producer D: Job ID " << item.jobid << " duration " << item.duration << endl;
      // end when no more jobs;
      if (count > num_of_jobs) { cout << "No more jobs" << endl; break; }
    }

  
  // quit when no more jobs

  //int *param = (int *) parameter;

  //cout << "Parameter = " << *param << endl;

  //sleep (5);

  //cout << "\nThat was a good sleep - thank you \n" << endl;

  pthread_exit(0);
}

void *consumer (void *id) // Consumer+_Parameter
{
  // TODO 
  cout << "entering consumer function" << endl;
  // initialise params
  Consumer_Parameter *param = (Consumer_Parameter *) id;
  int sem_id = param->sem_arr_key;
  //Queue_Item* queue = param->queue;
  Circular_Queue* queue = param->queue;
  //int len_of_queue = param->len_of_queue;
  Queue_Item item;

  // take jobs as described
  int count = 0; // for testing // replace with no more jobs condition

  while (true) {
    //if (count > 1) break;
    //cout << "waiting for item" << endl;
    sem_wait(sem_id, ITEM_INDEX);
    sem_wait(sem_id, MUTEX_INDEX);
    //cout << "fetching item" << endl;
    // fetch item;
      //item = queue[front_index];
    item = queue->deQueue();
      //cout << "collecting job " << item.jobid << " from queue position " << front_index << endl;
      //if (front_index != 0) {front_index = (front_index+1)%len_of_queue;}
      //if (!queueIsEmpty()) {end_index = (end_index+len_of_queue-1)%len_of_queue;}
      // consumer consumes from the front
    sem_signal(sem_id, MUTEX_INDEX);
    sem_signal(sem_id, SPACE_INDEX);
    // consume item;
      cout << "Consumer X: Job ID " << item.jobid << " executing sleep duration " << item.duration << endl;
      sleep(item.duration);
      cout << "Consumer D: Job ID " << item.jobid << " completed" << endl;
      //printQueue(queue, len_of_queue);
    // end when no more jobs;
      count++;
    if (count >= 4) break;
  }

  // print status

  // quit if no jobs after 20 seconds

  pthread_exit (0);

}
