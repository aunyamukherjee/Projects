#include "utils.h"
#include "sys/types.h"

#define PERM 0666
#define MAXSIZE 1024 //maxsize of a chunk, same size defition
#define PATHSIZE 100 //increased max pathsize for local machine directories. Doesn't affect assumptions
#define AUNYAID 5352025
#define HANSID 5465906
#define LINID 5369539 //not used, but included for fun

// Copy this from the book THIS HAS TO BE FIXED
static int msgid;
int initqueue(int key){
  msgid =  msgget(key, PERM | IPC_CREAT);  
  if (msgid < 0){
   return -1;
  }
  return 0;
}

// This function check if a queue already exists. If yes, delete the queue and open it again
int checkQueue (int key){
  msgid =  msgget(key, PERM | IPC_EXCL | IPC_CREAT);  
  if (msgid < 0){
    msgctl(msgid,IPC_RMID,NULL);  // delete the queue before creating a new one
  } 
  return initqueue(key);  // calling the function to create a new queue
}

char *getChunkData(int mapperID) {
  key_t key;
  if ((key = ftok(".", AUNYAID))<0){
    printf("Error in getChunkData ftok()"); // using the same queue as sendChunkData
    exit(1);
  }
  
  // have to open the queue again
  int queuesuccess = initqueue(key); //open queue
  if (queuesuccess == -1) {
    printf("Error: Fail to create message key in getChunkData()\n");
    exit(1);
  }
  
  struct msgBuffer chunkData;
  chunkData.msgType = mapperID; //set the tag of the message to the mapperID

  if (msgrcv(msgid, &chunkData, MAXSIZE, mapperID, 0) < 0){
    printf("Error: Can't read message in getChunkData\n");
    msgctl(msgid, IPC_RMID, NULL); //remove message queue before error exit
    exit (1); // exits if there is a msgrcv error
  }
  
  if (strcmp(chunkData.msgText,"END") == 0){ //if end msg is recieved, return NULL
    return NULL;
  }

  char *returnstring; //create return string pointer to be returned
  returnstring = (char*)malloc(sizeof(chunkData.msgText)+1);
  memset(returnstring,'\0',sizeof(chunkData.msgText)+1);
  strcpy(returnstring,chunkData.msgText);
  return returnstring; //this holds the chunkData to send to mapper.c
}


// sends chunks of size 1024 to the mappers in RR fashion
void sendChunkData(char *inputFile, int nMappers) {
  // Open a file and check if the file can be open. If not, print an error and return with an error exit
  int fnum= open(inputFile, O_RDONLY); 
  if (fnum<0){
    printf("Error: Cant open file\n");
    exit(1);
  }

  // Declare all of the variables needed for queue
  key_t key; 
  struct msgBuffer msg;
  if ((key = ftok(".", AUNYAID))==-1){  // Create unique key for msgqueue
  // Check if ftok is working properly, else exit the function 
    printf("Ftok failure in sendChunkData");
    exit(1);
  }
  
  // Create queue and check if it is created successuly. If not print out an error and return the function
  int queuesuccess = checkQueue(key);
  if (queuesuccess == -1){
    printf("Error: Fail to create message key in sendChunkData\n");
    msgctl(msgid, IPC_RMID, NULL);  // close msgBuffer Queue
    exit(1);  
  }

  // parse through the file and take 1024 bytes, check if the word is cut in the middle
  int mapperID = 1;  // Number of mapperID
  char text[MAXSIZE];

  // Initialize the leftover word from the last chunk
  int leftOver = 0;
  char sub[MAXSIZE];
  memset(sub,'\0',strlen(sub));

  // Reading the file chuck by chunk, should not cut any word 
  while (read(fnum,text,(MAXSIZE-leftOver))>0) {

    // This block is for parsing the file and not check if a word is split in the process //
    //-----------------------------------------------------------------------//
    // Append the cut word from last chunk back into the current chunk
    if (sub[0]!='\0'){
      strcat(sub,text);                 // to put sub in the front
      sprintf(text, "%s",sub);
    }
    memset(sub,'\0',sizeof(sub));     // Clean sub 

    // Checking for string to not split character
    // check if the last character is a space or a null character. If it is a null or space character, no word is cut, leftover = 0
    int indexSpace = MAXSIZE-1; // index of the last space, initially initialize at the last character of the chunk
    if (isalpha((int)text[MAXSIZE-1])>0){// This mean that the word is split
      // Finding the index of the last space 
      for (int j = MAXSIZE-1; j>=0; j--){
        if (isspace(text[j])){
          indexSpace = j;
          break;
        }
      }
      // Copy the cut word and save it for the next chunk
      int k = 0;
      for (int j = indexSpace+1; j<MAXSIZE; j++){
        sub[k] = text[j];
        text[j] = '\0';
        k++;
      }
    }
    //-----------------------------------------------------------------------//
    // Parsing block end here

    // Send the msgBuffer text to the queue
    msg.msgType = mapperID;
    sprintf(msg.msgText, "%s",text);
    
    if (msgsnd(msgid, &msg, MAXSIZE, 0)<0){
      printf("Msgsnd error in sendChunk");
      msgctl(msgid, IPC_RMID, NULL);  // close msgBuffer Queue
      exit(1);
    }

    // set leftover for next iteration so that don't overflow when reading and appending
    leftOver = MAXSIZE-1-indexSpace;  

    // Memset to Initialize buffer at '\0'
    memset(text,'\0',sizeof(text));
    memset(msg.msgText,'\0',sizeof(msg.msgText));

    // Creating tag for the message so that every mapper has a chance to receive a chunk
    mapperID = mapperID+1;
    if (mapperID==(nMappers+1)) {
      mapperID=1;
    }
  }
  
  // Close the file
  close(fnum);

  // Send End msgBuffer to each Mapper ID
  // End message is sent to the queue with specific tag that is the mapperID. So each mapperID will receive an END message
  struct msgBuffer endmess;
  memset(endmess.msgText,'\0',MAXSIZE);
  strcpy(endmess.msgText,"END");
  for (long k = 1; k<=nMappers; k++){ // send END message to the mappers
    endmess.msgType = k;
    if (msgsnd(msgid, &endmess, MAXSIZE, 0)==-1){
      printf("Msgsnd END msgBuffer to child"); 
    }
  }
  
  return;
}


// hash function to divide the list of word.txt files across reducers
int hashFunction(char* key, int reducers){
	unsigned long hash = 0;
    int c;

    while ((c = *key++)!='\0')
        hash = c + (hash << 6) + (hash << 16) - hash;
    return (hash % reducers);
}


int getInterData(char *key, int reducerID) {
  key_t key1;  
  if ((key1 = ftok(".", HANSID))<0){
    printf("Ftok Error"); // using the same queue as shuffle
    msgctl(msgid,IPC_RMID,NULL);
    exit(1);
  }
  // have to open the queue again
  int queuesuccess = initqueue(key1);
  if (queuesuccess == -1){
    printf("Error: Fail to create message key\n");
    msgctl(msgid,IPC_RMID,NULL);
    exit(1);
  }
  struct msgBuffer wordFileName;
  wordFileName.msgType = reducerID; //settni

  if (msgrcv(msgid, &wordFileName, MAXSIZE, (long)reducerID, 0) < 0){
    printf("Error: Can't read message in getInter\n");
    msgctl(msgid,IPC_RMID,NULL);
    exit (1); // exits if there is a msgrcv error
  }
  
  if (strcmp(wordFileName.msgText,"END") == 0){
    return 0;
  }
  memset(key,'\0',sizeof(key)+1);
  strcat(key,wordFileName.msgText);
  return 1;
}

void shuffle(int nMappers, int nReducers) {
  int reducerId; 
  struct msgBuffer msg; 
  key_t key; 

  if ((key = ftok(".", HANSID))==-1){ 
    printf("Ftok Error: Fail to create key\n");
    exit(1);
  }
  // have to open the queue again
  int queuesuccess = checkQueue(key);
  if (queuesuccess == -1){
    printf("Error: Fail to create message key\n");
    msgctl(msgid, IPC_RMID, NULL);
    exit(1);
  }
  

  //traverse directory of each Mapper and filepath to reducers
  
  DIR* dir;
  char folder[PATHSIZE];
  char path[PATHSIZE];
  struct dirent* entry;
  for(int i=1;i<=nMappers;i++) {
    // Create the mapper folder with its ID 
    char parent[PATHSIZE-1];
    memset(parent,'\0',PATHSIZE-1);
    sprintf(parent,"%s", getcwd(folder,PATHSIZE)); 
    sprintf(folder, "%s/", parent);
    char temp[PATHSIZE-1];
    memset(temp,'\0',PATHSIZE-1);
    sprintf(temp,"%d",i); 
    strcat(folder,"output/MapOut/Map_");
    strcat(folder,temp);
    dir = opendir(folder);
    if(dir==NULL) {    // Check if dir is open correctly    
      printf("The path passed is invalid\n");   
      msgctl(msgid,IPC_RMID,NULL); //destroy msg queue     
      exit(1);    
    }
    
    while ((entry = readdir(dir))!=NULL){
      if (entry->d_type == DT_REG) {// This is a regular file
        // append entry->d_name into our folder
        memset(path,'\0',PATHSIZE);
        strcpy(path,folder);
        strcat(path,"/");
        strcat(path,entry->d_name);
        
        reducerId = hashFunction(entry->d_name, nReducers);
        reducerId += 1; //add 1 to hash for reducer 1,2,etc.
        //send word filepath to reducer
        msg.msgType = reducerId;
        sprintf(msg.msgText, "%s",path);
        if (msgsnd(msgid, &msg, MAXSIZE, 0)<0){
          printf("Msgsnd in shuffle\n");
          msgctl(msgid,IPC_RMID,NULL); //destroy msg queue
          exit(1);
        }
      }
    }
  }
  
  //send END message to reducers
  struct msgBuffer endmsg;
  memset(endmsg.msgText,'\0',sizeof(endmsg.msgText));
  sprintf(endmsg.msgText,"END");
  for(int i=1;i<=nReducers;i++) { //send END to all the 
    endmsg.msgType = (long) i;
    if (msgsnd(msgid, &endmsg, MAXSIZE, 0)<0){
        printf("Msgsnd error in shuffle END\n");
        msgctl(msgid,IPC_RMID,NULL); //destroy msg queue
        exit(1);
    }
  }

  return;
}

// check if the character is valid for a word
int validChar(char c){
	return ((tolower(c) >= 'a') && tolower(c <='z')) ||
					(c >= '0' && c <= '9');
}

char *getWord(char *chunk, int *i){
	char *buffer = (char *)malloc(sizeof(char) * chunkSize);
	memset(buffer, '\0', chunkSize);
	int j = 0;
	while((*i) < strlen(chunk)) {
		// read a single word at a time from chunk
		if (chunk[(*i)] == '\n' || chunk[(*i)] == ' ' || !validChar(chunk[(*i)]) || chunk[(*i)] == 0x0) {
			buffer[j] = '\0';
			if(strlen(buffer) > 0){
				(*i)++;
				return buffer;
			}
			j = 0;
			(*i)++;
			continue;
		}
		buffer[j] = chunk[(*i)];
		j++;
		(*i)++;
	}
	if(strlen(buffer) > 0)
		return buffer;
	return NULL;
}

void createOutputDir(){
	mkdir("output", ACCESSPERMS);
	mkdir("output/MapOut", ACCESSPERMS);
	mkdir("output/ReduceOut", ACCESSPERMS);
}

char *createMapDir(int mapperID){
	char *dirName = (char *) malloc(sizeof(char) * 100);
	memset(dirName, '\0', 100);
	sprintf(dirName, "output/MapOut/Map_%d", mapperID);
	mkdir(dirName, ACCESSPERMS);
	return dirName;
}

void removeOutputDir(){
	pid_t pid = fork();
	if(pid == 0){
		char *argv[] = {"rm", "-rf", "output", NULL};
		if (execvp(*argv, argv) < 0) {
			printf("ERROR: exec failed\n");
			exit(1);
		}
		exit(0);
	} else{
		wait(NULL);
	}
}

void bookeepingCode(){
	removeOutputDir();
	sleep(1);
	createOutputDir();
}