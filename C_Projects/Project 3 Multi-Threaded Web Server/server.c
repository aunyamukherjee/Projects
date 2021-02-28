#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include "util.h"
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>

#define MAX_THREADS 100
#define MAX_QUEUE 100
#define MAX_CE 100
#define MAX_CACHE 100
#define INVALID -1
#define BUFF_SIZE 1024

/*
  THE CODE STRUCTURE GIVEN BELOW IS JUST A SUGGESTION. FEEL FREE TO MODIFY AS NEEDED
*/

// structs:
typedef struct request_queue {
   int fd;
   char request[BUFF_SIZE];
} request_t;

typedef struct cache_entry {
    int len;
    char *request;
    char *content;
} cache_entry_t;

// Queue Structure
typedef struct{
  int nrequest;
  int ntakeout;
  request_t messagequeue[MAX_QUEUE];
} Queue ;

// args structure
struct arg_struct {
    int arg1;
    int arg2;
};

// Locks
pthread_mutex_t ring_access = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cancel_access = PTHREAD_MUTEX_INITIALIZER;

// Condition variables
pthread_cond_t somecontent = PTHREAD_COND_INITIALIZER; //waits for buffer to be non empty
pthread_cond_t freeslot = PTHREAD_COND_INITIALIZER;//waits for buffer to free up 
pthread_cond_t threadCV = PTHREAD_COND_INITIALIZER;//check if a thread need to be deleted 

// Other global variables
// Dynamic checking related variables 
int threadchecking = 1; // flag if the manager for dynamic_pool_size_update is working, if it is, no worker threads should be working except 0 
int DynamicFlag = 0;
int CancelState[MAX_THREADS];
// Cache related variables
cache_entry_t cache[MAX_CACHE];
int cache_entries;

// Log file 
FILE* logfile;
Queue queue;



/* ************************ Cache Code [Extra Credit B] **************************/

// Function to check whether the given request is present in cache, returns index on success and -1 on failure.
int getCacheIndex(char *request){
  /// return the index if the request is present in the cache
  for (int i = 0; i < cache_entries; i++){
    if (cache[i].request != NULL){
      if (strcmp (request, cache[i].request) == 0){
        return i;
      }
    }
  } 
  return -1;
}

// Function to add the request and its file content into the cache
void addIntoCache(char *request, char *content , int memory_size){
  // It should add the request at an index according to the cache replacement policy
  // Make sure to allocate/free memory when adding or replacing cache entries
  static int cache_index = 0;

  if (cache_index > cache_entries-1) { //if cache index is greater than the amount of slots we have available, then reset the index to zero to implement FIF) queue
    cache_index = 0;
  }  
  free(cache[cache_index].request);
  free(cache[cache_index].content);
  char*temp = malloc(strlen(request) + 1);
  char*temp2 = malloc(strlen(content) + 1);
  strcpy(temp,request);
  strcpy(temp2,content);
  cache[cache_index].request = temp; //set cache entry to the current request
  cache[cache_index].content = temp2;
  cache[cache_index].len = memory_size;
  cache_index++;
}

// clear the memory allocated to the cache
void deleteCache(){
  // De-allocate/free the cache memory
  for(int i = 0;i < cache_entries; i++) {
    //deallocate each struct 
    free(cache[i].content);
    free(cache[i].request);
  }
}

// Function to initialize the cache
void initCache(int entries){
  // Allocating memory and initializing the cache array
  cache_entries = entries;
  for(int i=0;i<entries;i++) {
    cache[i].request = NULL;
    cache[i].content = NULL;
  }
}

/**********************************************************************************/

/* ************************************ Utilities ********************************/
// Function to get the content type from the request
// Should return the content type based on the file type in the request
char* getContentType(char * mybuf) { 

  char *temp;
  char *content_type = malloc(20*sizeof(char));
  memset(content_type,'\0',20);
  
  // if request is empty, return NULL
  if((temp = strchr(mybuf,'.')) == NULL) {
    return NULL;
  }
  if (strcmp (temp, ".html")==0){
    strcat(content_type,"text/html");
  }
  else if (strcmp (temp, ".gif")==0){
    strcat(content_type,"image/gif");
  }
  else if (strcmp (temp, ".jpg")==0){
    strcat(content_type,"image/jpeg");
  }
  else {// by default
    strcat(content_type,"text/plain");
  }
  return content_type;
}

// Function to open and read the file from the disk into the memory
int readFromDisk(char *filename, char **buf) {

  FILE * fp = fopen(filename, "rb");
 
  // if file pointer is NULL, return error
  if (fp == NULL) { 
    return -1; 
  } 
  fseek(fp, 0L, SEEK_END); 
  // calculating the size of the file 
  int res = ftell(fp); 
  rewind(fp);
  *buf = malloc((res+10)*sizeof(char));
  
  fread(*buf,sizeof(char),res,fp);

  // closing the file 
  fclose(fp);

  return res;
}

/**********************************************************************************/

// Function to receive the request from the client and add to the queue. 
void * dispatch(void* arg) {
  // takes in queue len from main
  int qlen = *((int*)(&arg));
  request_t req;
  while (1) {

    while ((req.fd = accept_connection()) < 0){
      continue; // if accept connection returns negative number, it is ignored
    }
    pthread_mutex_lock(&ring_access); //lock for global variable queue
    while ((queue.nrequest-queue.ntakeout) >=(qlen-1) && (queue.nrequest!=queue.ntakeout)){
      pthread_cond_wait(&somecontent,&ring_access); //wait for somecontent signal if queue full
    }
    // Get request from the client
    if(get_request(req.fd, req.request) != 0) { 
    }
    memmove(&req.request[0], &req.request[1], strlen(req.request));
      
    // Add the request into the queue
    queue.messagequeue[queue.nrequest % qlen] = req;
    queue.nrequest++;
    //signal freeslot and unlock
    pthread_mutex_unlock(&ring_access);
    pthread_cond_signal(&freeslot);
   }
   return NULL;
}

/**********************************************************************************/

// Function to retrieve the request from the queue, process it and then return a result to the client
void * worker(void * args) {
  // takes in queue len and worker number from main
  struct arg_struct *input = (struct arg_struct*) args;
  int qlen = input->arg2;
  int workernumber = input->arg1;
  
  int cache_temp_index, size, errorflag, cacheflag; //index saved from result of getCacheIndex, size of request, flag to track when errors occur, flag to check for cache hit
  char* temp = NULL,*buf = NULL, *error = NULL; //temp for return of getContentType, save error string to print instead of size, buffer where file is saved on memory
  int count = 0; //keep track of how many requests a worker thread has handled

  while (1) {
    errorflag = 0;
    cacheflag = 0;
    if(DynamicFlag) { // if using dynamic size of workers
      pthread_mutex_lock(&cancel_access);
      while(threadchecking==1){// wait until get a green light check from thread worker manager dynamic_pool_size_update
        pthread_cond_wait(&threadCV,&cancel_access);
      }
      if(CancelState[workernumber]) { // if the dynamic_pool_size_update marks this thread with 1, thread should unlock and return 
        CancelState[workernumber] = 0;
        pthread_mutex_unlock(&cancel_access);
        return NULL;
      }
      pthread_mutex_unlock(&cancel_access); // unlock manager thread even though not getting deleted for the manager thread to run later 
    }
    
    pthread_mutex_lock(&ring_access);
    while (queue.ntakeout==queue.nrequest){
      pthread_cond_wait(&freeslot,&ring_access); // wait for freeslot signal if queue empty
    }   
    int takeout = queue.ntakeout % qlen;  // true index of takeout 
    // Get the request from the queue
    // need to parse the file 
    char filename[BUFF_SIZE];
    memset(filename,'\0',BUFF_SIZE);
    strcpy(filename,queue.messagequeue[takeout].request);
    int fdl = queue.messagequeue[takeout].fd;

    // Check the type of the requested file 
    temp = getContentType(queue.messagequeue[takeout].request);
    
    if(temp == NULL) {  // If invalid requested file's type, return error 
      errorflag = 1;
      error = malloc(BUFF_SIZE);
      return_error(queue.messagequeue[takeout].fd,error);
    }
    else {  // find a defined type for the requested file 

      if(cache_entries > 0) { // if cache is used, find if the file already in cache 
        cache_temp_index = getCacheIndex(filename);
      }
      if(cache_entries && cache_temp_index > -1) { //HIT for cache 
        if(return_result(queue.messagequeue[takeout].fd,temp,cache[cache_temp_index].content,cache[cache_temp_index].len) != 0) { // check if return return fails
          char *error = malloc(BUFF_SIZE);
          return_error(queue.messagequeue[takeout].fd,error);
        }
        else {
          cacheflag = 1;
          size = cache[cache_temp_index].len;
        }
      }
      else { // MISS cache
      
        //get the content type of the request and store it in temp
        //recieve file size from readFromDisk
        
        if((size = readFromDisk(queue.messagequeue[takeout].request, &buf)) == -1) {
          errorflag = 1;
          char *error = malloc(BUFF_SIZE);
          return_error(queue.messagequeue[takeout].fd,error);
        }

        else {
          //if return_result returns an error, call return_error
          if(return_result(queue.messagequeue[takeout].fd,temp,buf,size) != 0) {
            char *error = malloc(BUFF_SIZE);
            return_error(queue.messagequeue[takeout].fd,error);
          }
          else{
            //return result success
            // return the result
            addIntoCache(filename,buf,size);
          }
        }
      }
    }

    queue.ntakeout++;
    count++;

    // Log the request into the file and terminal
    if(errorflag) {
      printf("[%d][%d][%d][%s][Requested File Not Found][MISS]\n",workernumber,count,fdl,filename);
      fprintf(logfile,"[%d][%d][%d][%s][Requested File Not Found][MISS]\n",workernumber,count,fdl,filename);
    }
    else if (cacheflag) {
      printf("[%d][%d][%d][%s][%d][HIT]\n",workernumber,count,fdl,filename,size);
      fprintf(logfile,"[%d][%d][%d][%s][%d][HIT]\n",workernumber,count,fdl,filename,size);
    }
    else {
      printf("[%d][%d][%d][%s][%d][MISS]\n",workernumber,count,fdl,filename,size);
      fprintf(logfile,"[%d][%d][%d][%s][%d][MISS]\n",workernumber,count,fdl,filename,size);
    }

    //free resources
    if (buf != NULL) {
      free(buf);
    }
    if (temp != NULL) {
      free(temp);
    }
    if (error != NULL) {
      free(error);
    }
    error = NULL;
    buf = NULL;
    temp = NULL;
    pthread_mutex_unlock(&ring_access);
    pthread_cond_signal(&somecontent);

  }
}

/**********************************************************************************/

/* ******************** Dynamic Pool Code  [Extra Credit A] **********************/
// Extra Credit: This function implements the policy to change the worker thread pool dynamically
// depending on the number of requests
void * dynamic_pool_size_update(void *arg) {

  // Run at regular intervals
  struct arg_struct* args = (struct arg_struct*) arg; //input stuct cast for argument to get the initial number of workers and the queue length
  int nWorker = args->arg1;
  int qlen = args->arg2;
  pthread_t dyworkerArr[100];  // worker array 

  // Creating the initial workers as the user's input 
  pthread_mutex_lock(&cancel_access);
  for (int i=0; i<nWorker; i++){
    args[i].arg1 = i;
    args[i].arg2 = qlen;
    CancelState[i] = 0;
    if (pthread_create(&dyworkerArr[i],NULL,(void *)worker, (void*) &args[i])!=0){
      printf("Error in creating dynamic threads.\n");
      pthread_kill(pthread_self(),SIGKILL);      
    }
    pthread_detach(dyworkerArr[i]); // makes it detached
  }
  pthread_mutex_unlock(&cancel_access);
  
  //Increase / decrease dynamically based on amount of unserved requests
  int activity;
  struct arg_struct input[MAX_THREADS];
  int deletedWorkers;
  while(1) {
    threadchecking = 1;
    pthread_mutex_lock(&ring_access); 
    activity = queue.nrequest-queue.ntakeout;
    pthread_mutex_unlock(&ring_access);
  
    if(activity > nWorker) {
      activity = activity-nWorker;
      if (activity > MAX_THREADS) { // make sure we dont go above MAX_THREADS amount of worker threads
        activity = MAX_THREADS - nWorker;
      }
    }
    if (activity >= 1){
      pthread_mutex_lock(&cancel_access);
      for (int i=nWorker; i<nWorker+activity; i++){ // creating additional threads
        input[i].arg1 = i;
        input[i].arg2 = qlen;
        CancelState[i] = 0;
        if (pthread_create(&dyworkerArr[i],NULL,(void *)worker, (void*) &input[i])!=0){
          printf("Error in creating dynamic threads.\n");
          pthread_kill(pthread_self(),SIGKILL);  
        }
        pthread_detach(dyworkerArr[i]); // makes it detached
      }
      pthread_mutex_unlock(&cancel_access); // lock for global variable CancelState array
      threadchecking = 0;
      pthread_cond_broadcast(&threadCV);
      
      printf("Created %d worker threads because server load is %d requests\n",activity, activity + nWorker);
      nWorker += activity;
    }
    else { //deleting worker threads
      if (nWorker>1 && activity >= 0){
        deletedWorkers = nWorker-activity-1;
        pthread_mutex_lock(&cancel_access);
        for(int i=nWorker-1; i > activity; i--) {
          CancelState[i] = 1; //set cancelstate array to one to tell that worker to return
        }
        pthread_mutex_unlock(&cancel_access);
        
        nWorker=activity+1;
        if (deletedWorkers!=0){
            printf("Removed %d worker threads because server load is %d requests\n",deletedWorkers, activity);
        }
        threadchecking = 0;
        pthread_cond_broadcast(&threadCV);
        
      }
    }
  }
}

/**********************************************************************************/
/**********************************************************************************/

// use a handler to handle graceful termination
static volatile sig_atomic_t interflag = 0;
static void catchint(int signo) {
  fprintf(stderr, "Server terminating...\n");
  interflag = 1;
}

int main(int argc, char **argv) {
  // Error check on number of arguments
  if(argc != 8){
    printf("usage: %s port path num_dispatcher num_workers dynamic_flag queue_length cache_size\n", argv[0]);
    return -1;
  }

  // initialize queue values 
  queue.ntakeout = 0;
  queue.nrequest = 0;

  // Get the input args
  int port = strtol(argv[1], NULL, 10);
  int nDispatch = strtol(argv[3], NULL, 10);
  int nWorker = strtol(argv[4], NULL, 10);
  int dynamic_flag = strtol(argv[5],NULL,10); // 0 if not doing extra credit A, else argv[5]
  int qlen = strtol(argv[6], NULL, 10);
  int cache_len = strtol(argv[7], NULL, 10); //0 if not doing extra credit B, else argv[7]
  init(port);
  // Perform error checks on the input arguments
  if (port<1025 || port >65535){
    printf("Invalid port number.\n");
    return INVALID; 
  }
  if ((nDispatch> MAX_THREADS) || (nDispatch <=0)){
    printf("Invalid number of dispatch threads.\n");
    return INVALID; 
  }
  if ((nWorker> MAX_THREADS) || (nDispatch <=0)){
    printf("Invalid number of worker threads.\n");
    return INVALID; 
  }
  DynamicFlag = dynamic_flag;
  if (qlen>MAX_QUEUE || qlen <=0)
  {
    printf("Invalid queue length size.\n");
    return INVALID;
  }
  if (cache_len>MAX_CACHE || qlen <=0)
  {
    printf("Invalid cache size.\n");
    return INVALID;
  }


  // Change SIGINT action for grace termination
  struct sigaction act;
  act.sa_handler = catchint;
  act.sa_flags = 0;
  if ((sigemptyset(&act.sa_mask) == -1) ||
  (sigaction(SIGINT, &act, NULL) == -1)) { 
    perror("Failed to set SIGINT handler"); 
    return 1;
  }

//********************************
  // Open log file
  char wordFileName[20];
  strcpy(wordFileName,"web_server_log");
  logfile = fopen(wordFileName,"w");

  // Change the current working directory to server root directory
  if (chdir(argv[2])!=0){
	  printf("Couldn't change directory to server root.: No such file or directory\n");
  }
  char path[1023];
  getcwd(path,1024);

  // Initialize cache (extra credit B)
  initCache(cache_len);

  // Start the server
  printf("Starting server on port %d: %d disp, %d work\n", port, nDispatch,nWorker);
  
  // Create dispatcher and worker threads (all threads should be detachable)
  // Create attribute for detached threads

  pthread_t dispatchArr[nDispatch];
  for (int i = 0; i<nDispatch; i++){
    if (pthread_create(&dispatchArr[i],NULL,(void *)dispatch, (void*) qlen)!=0){
      printf("Error in creating dynamic threads.\n");
      return INVALID;
    }
    pthread_detach(dispatchArr[i]);
  }
  if (dynamic_flag == 0){
    struct arg_struct args[nWorker];
    pthread_t workerArr[nWorker];
    pthread_mutex_lock(&cancel_access);
    for (int i = 0; i<nWorker; i++){ 
      args[i].arg1 = i;
      args[i].arg2 = qlen;
      CancelState[i] = 0;
      if (pthread_create(&workerArr[i],NULL,(void *)worker, (void*) &args[i])!=0){
        printf("Error in creating worker threads.\n");
        return INVALID;
      }
      pthread_detach(workerArr[i]); // makes it detached
    }
    pthread_mutex_unlock(&cancel_access);
  }
  else {// Create dynamic pool manager thread (extra credit A)
    // create a manager thread
    struct arg_struct args;
    pthread_t manager;
    args.arg1 = nWorker;
    args.arg2 = qlen;
    if (pthread_create(&manager,NULL,(void *)dynamic_pool_size_update, (void*) &args)!=0){
      printf("Error in creating dynamic threads.\n");
      return INVALID;      
    }
    pthread_detach(manager); // makes it detached
  }
  

  // Terminate server gracefully
  while(!interflag);
  
  // Print the number of pending requests in the request queue
  printf("The number of pending requests in the request queue: %d\n",queue.nrequest-queue.ntakeout);

  // close log file
  fclose(logfile);

  // Remove cache (extra credit B)
  deleteCache();
  
  printf("Main thread exiting.\n");
  return 0;
}