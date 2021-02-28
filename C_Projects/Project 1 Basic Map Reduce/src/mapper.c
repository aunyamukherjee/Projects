#include "mapper.h"

// combined value list corresponding to a word <1,1,1,1....>
valueList *createNewValueListNode(char *value){
	valueList *newNode = (valueList *)malloc (sizeof(valueList));
	strcpy(newNode -> value, value);
	newNode -> next = NULL;
	return newNode;
}

// insert new count to value list
valueList *insertNewValueToList(valueList *root, char *count){
	valueList *tempNode = root;
	if(root == NULL)
		return createNewValueListNode(count);
	while(tempNode -> next != NULL)
		tempNode = tempNode -> next;
	tempNode -> next = createNewValueListNode(count);
	return root;
}

// free value list
void freeValueList(valueList *root) {
  if(root == NULL) return;
    valueList *tempNode = NULL;
  while (root != NULL){
    tempNode = root;
    root = root -> next;
    free(tempNode);
  }
}

// create <word, value list>
intermediateDS *createNewInterDSNode(char *word, char *count){
	intermediateDS *newNode = (intermediateDS *)malloc (sizeof(intermediateDS));
	strcpy(newNode -> key, word);
	newNode -> value = NULL;
	newNode -> value = insertNewValueToList(newNode -> value, count);
	newNode -> next = NULL;
	return newNode;
}

// insert or update a <word, value> to intermediate DS
intermediateDS *insertPairToInterDS(intermediateDS *root, char *word, char *count){
	intermediateDS *tempNode = root;
	if(root == NULL){
    root = createNewInterDSNode(word,count);
    return root;
  }
	while(tempNode -> next != NULL) {
		if(strcmp(tempNode -> key, word) == 0){
			tempNode -> value = insertNewValueToList(tempNode -> value, count);
			return root;
		}
		tempNode = tempNode -> next;
		
	} 
	if(strcmp(tempNode -> key, word) == 0){
		tempNode -> value = insertNewValueToList(tempNode -> value, count);
	} else {
		tempNode -> next = createNewInterDSNode(word, count);
	}
	return root;
}

// free the DS after usage. Call this once you are done with the writing of DS into file
void freeInterDS(intermediateDS *root) {
  if(root == NULL) return;
  intermediateDS *tempNode = NULL;
    while (root != NULL){
      tempNode = root;
      root = root -> next;
      freeValueList(tempNode -> value);
      free(tempNode);
    }
}

// emit the <key, value> into intermediate DS 
// Calling insert on each <key,value> and add it onto the intermediate data structure 
// Return the root of the data structure
intermediateDS * emit(char *key, char *value, intermediateDS* GlobalRoot) {
  GlobalRoot = insertPairToInterDS(GlobalRoot, key, value); //insert GlobalRoot into DS and collect root
  return GlobalRoot; 
}

// map function: take in a text portion, read in word by word and call 
// emit to put the word into the data structure
intermediateDS *map(char *chunkData, intermediateDS *GlobalRoot){
  int i = 0;
  char *buffer;
  while ((buffer = getWord(chunkData, &i)) != NULL){ //use getWord to retrieve words from the chunkData one by one.
 		GlobalRoot = emit(buffer,"1", GlobalRoot);
  }
  return GlobalRoot;
}

// write intermediate data to separate word.txt files
// Each file will have only one line : word 1 1 1 1 1 ...
void writeIntermediateDS(intermediateDS *current, char *mapOutDir) {
  // Set a tempNode pointer to the root of the Intermediate Data Structure
  intermediateDS *tempNode = current;
  while (tempNode != NULL){
    // Setting pointer word to the key of current node of the Intermediate Data Structure
    char *word = tempNode -> key;
    // Copy the word to create the path of the file
    char wordtxt[MAXKEYSZ];
    strcpy(wordtxt,word);
    // Adding the .txt to the filename
    strcat(wordtxt,".txt"); 
    // Adding the path to the direction
    char* mapOutTemp = (char*)malloc(sizeof(mapOutDir)+sizeof(wordtxt));
    strcpy(mapOutTemp,mapOutDir); //mapOutTemp will be the full file path
    strcat(mapOutTemp,"/");
    strcat(mapOutTemp,wordtxt);
    // Open the file
    FILE *fd = fopen(mapOutTemp,"w"); 
    // if unsuccessful, exit on error
    if (fd == NULL){
      printf("Error: File can't be created");
      exit(1);
    }
    // Print the word to the file
    fputs(word,fd);
    valueList *current2 = tempNode -> value; 
    while (current2 != NULL ) {
      fputs(" ", fd);
      fputs(current2 -> value, fd);
      current2 = current2 -> next; //iterate through the linked list of 1s
    }
    tempNode = tempNode -> next; //iterate through the linked list
    fclose(fd); // close up the file
  }
  
}

int main(int argc, char *argv[]) {
  // Checking if there is enough arguments passing into mapper
	if (argc < 2) {
		printf("Less number of arguments.\n");
		printf("./mapper mapperID\n");
		exit(0);
	}
	// ###### DO NOT REMOVE ######
	mapperID = strtol(argv[1], NULL, 10);


	// ###### DO NOT REMOVE ######
	// create folder specifically for this mapper in output/MapOut
	// mapOutDir has the path to the folder where the outputs of this mapper should be stored
	mapOutDir = createMapDir(mapperID);
  
  // Intialize a pointer of type intermediateDS and set to NULL
  intermediateDS *GlobalRoot;
  GlobalRoot = NULL;
	
  // ###### DO NOT REMOVE ######
	while(1) {
		// create an array of chunkSize=1024B and intialize all 
		// elements with '\0'
		char chunkData[chunkSize + 1]; // +1 for '\0'
		memset(chunkData, '\0', chunkSize + 1);

		char *retChunk = getChunkData(mapperID);
		if(retChunk == NULL) {
			break;
		}

		strcpy(chunkData, retChunk);
		free(retChunk);
		GlobalRoot = map(chunkData, GlobalRoot);
	}

  // ###### DO NOT REMOVE ######
  writeIntermediateDS(GlobalRoot, mapOutDir);
  
  // Checking if GlobalRoot is NULL
  if (GlobalRoot != NULL){
    freeInterDS(GlobalRoot);
  }
	return 0;
}

