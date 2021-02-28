#include "mapreduce.h"
#define PERM 0666
#define AUNYAID 5352025
#define HANSID 5465906
#define LINID 5369539 //not used, but included for fun
// execute executables using execvp
void execute(char **argv, int nProcesses){
	pid_t  pid;

	int i;
	for (i = 0; i < nProcesses; i++){
		pid = fork();
		if (pid < 0) {
			printf("ERROR: forking child process failed\n");
			exit(1);
		} else if (pid == 0) {
			char *processID = (char *) malloc(sizeof(char) * 5); // memory leak
			sprintf(processID, "%d", i+1);
			argv[1] = processID;
			if (execvp(*argv, argv) < 0) {
				printf("ERROR: exec failed\n");
				exit(1);
			}
		}
  }
}

int main(int argc, char *argv[]) {
	
	if(argc < 4) {
		printf("Less number of arguments.\n");
		printf("./mapreduce #mappers #reducers inputFile\n");
		fflush(stdout);
		exit(0);
	}

	int nMappers 	= strtol(argv[1], NULL, 10);
	int nReducers 	= strtol(argv[2], NULL, 10);

	if(nMappers < nReducers){
		printf("ERROR: Number of mappers should be greater than or equal to number of reducers...\n");
		fflush(stdout);
		exit(0);
	}

	if(nMappers == 0 || nReducers == 0){
		printf("ERROR: Mapper and Reducer count should be grater than zero...\n");
		fflush(stdout);
		exit(0);
	}
	
	char *inputFile = argv[3];

	if(access(inputFile, F_OK) == -1){
		printf("ERROR: %s not accessible\n", inputFile);
		fflush(stdout);
		exit(0);
	}

	bookeepingCode();

	int status;
	pid_t pid = fork();
	if(pid == 0){// Child process
		//send chunks of data to the mappers in RR fashion
		sendChunkData(inputFile, nMappers);
		exit(0);
	}
	sleep(1);
  
	// spawn mappers
	char *mapperArgv[] = {"./mapper", NULL, NULL};
	execute(mapperArgv, nMappers);
//	wait for all children to complete execution
    while (wait(&status) > 0);
    //shuffle sends the word.txt files generated by mapper 
    //to reducer based on a hash function

	// wait for all children to complete execution
        //while (wait(&status) > 0);

    // shuffle sends the word.txt files generated by mapper 
    // to reducer based on a hash function

	pid = fork();
	if(pid == 0){
	 	shuffle(nMappers, nReducers);
		exit(0);
	}
	sleep(1);

	//spawn reducers
	char *reducerArgv[] = {"./reducer", NULL, NULL};
	execute(reducerArgv, nReducers);
	// wait for all children to complete execution
    while (wait(&status) > 0) {

    }
    


  // open and close the message queues used in the utils.c file so that no message queues are remaining.

  // message queue for sendChunkData and getChunkData
  int msgid;
  key_t key;

  if ((key = ftok(".", 5352025))==-1){
    printf("Error: Fail to create key\n");
    exit(1);
  }
  
  msgid =  msgget(key, PERM);  
  if (msgid < 0){
   printf("MsgQueue open failure in mapreduce\n");
   exit(1);
  }

  msgctl(msgid,IPC_RMID,NULL); //open msg for mapper and close it
  
  // message queue for shuffle and getInterData
  key_t key2; 
  int msgid2;

  if ((key2 = ftok(".", 5465906))==-1){
    printf("Error: Fail to create key\n");
    exit(1);
  }

  msgid2 =  msgget(key2, PERM);  
  if (msgid2 < 0){
    printf("MsgQueue open failure in mapreduce2\n");
    msgctl(msgid2,IPC_RMID,NULL);
    exit(1);
  }
  // have to open the queue again
  
  msgctl(msgid2,IPC_RMID,NULL); //open msg for reduce and close

	return 0;
}
