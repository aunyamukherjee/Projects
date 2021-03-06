#include "mapreduce.h"

int main(int argc, char *argv[]) {
	if(argc < 4) {
		printf("Less number of arguments.\n");
		printf("./mapreduce #mappers #reducers inputFile\n");
		exit(0);
	}

	// ###### DO NOT REMOVE ######
	int nMappers 	= strtol(argv[1], NULL, 10);
	int nReducers 	= strtol(argv[2], NULL, 10);
	char *inputFile = argv[3];

	// ###### DO NOT REMOVE ######
	bookeepingCode();

	// ###### DO NOT REMOVE ######
	pid_t pid = fork();
	if(pid == 0){
		//send chunks of data to the mappers in RR fashion
		sendChunkData(inputFile, nMappers);
		exit(0);
	}
	sleep(1);
	// spawn mappers processes and run 'mapper' executable using exec
  // spawnMapper(nMappers);
  // Running nMappers from 1 to the nMappers and create corresponding nMapper childs with fork()
  for (int i=1; i<=nMappers; i++){
    pid_t pid1 = fork();
    char str[5];
    sprintf(str, "%d", i);
    str[4] = '\0'; 
    char *args[] = {"mapper",str, NULL};    //arguments to be passed into exec
    if (pid1 == 0){ //if child, then exec to mapper
      execv(*args,args);
      exit(0);
    }
    // parent waits for all child processes to terminate
	  // wait for all children to complete execution
    // waitForAll();
    else if (pid1>0){       // Parent is running, parent should wait for child
      waitpid(pid1,NULL,0); //wait for the child 
    }
  }
  
	// ###### DO NOT REMOVE ######
  // shuffle sends the word.txt files generated by mapper 
  // to reducer based on a hash function
	pid = fork();
	if(pid == 0){
		shuffle(nMappers, nReducers);
		exit(0);
	}
	sleep(1);
	// spawn reducer processes and run 'reducer' executable using exec
  //spawnReducers(nReducers);
  for (int i=1; i<=nReducers; i++){
    pid_t pid2 = fork();
    char str[5];
    sprintf(str, "%d", i);
    str[4] = '\0';
    char *args[] = {"reducer", str, NULL}; //arguments to be passed into exec
    if (pid2 == 0){   // if child is running, execv to run reducer
      execv(*args,args);
      exit(0);
    }
    // parent waits for all child processes to terminate
    // To do
	  // wait for all children to complete execution
    // waitForAll();
    else if (pid2>0) {        // Parent is running, parent should wait for child
      waitpid(pid2, NULL,0);  // wait for children to be done.
    }
  }

	return 0;
}