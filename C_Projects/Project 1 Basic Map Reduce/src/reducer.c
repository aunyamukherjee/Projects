#include "reducer.h"

// create a key value node
finalKeyValueDS *createFinalKeyValueNode(char *word, int count){
	finalKeyValueDS *newNode = (finalKeyValueDS *)malloc (sizeof(finalKeyValueDS));
	strcpy(newNode -> key, word);
	newNode -> value = count;
	newNode -> next = NULL;
	return newNode;
}

// insert or update an key value
finalKeyValueDS *insertNewKeyValue(finalKeyValueDS *root, char *word, int count){
	finalKeyValueDS *tempNode = root;
	if(root == NULL)
		return createFinalKeyValueNode(word, count);
	while(tempNode -> next != NULL){
		if(strcmp(tempNode -> key, word) == 0){
			tempNode -> value += count;
			return root;
		}
		tempNode = tempNode -> next;
	}
	if(strcmp(tempNode -> key, word) == 0){
		tempNode -> value += count;
	} else{
		tempNode -> next = createFinalKeyValueNode(word, count);
	}
	return root;
}

// free the DS after usage. Call this once you are done with the writing of DS into file
void freeFinalDS(finalKeyValueDS *root) {
  if(root == NULL) return;

  finalKeyValueDS *tempNode = NULL;
  while (root != NULL){
  tempNode = root;
  root = root -> next;
  free(tempNode);
  }
}

// reduce function
finalKeyValueDS *reduce(char *key, finalKeyValueDS* GlobalRoot) {
  // Open the file for reading
  FILE* fd = fopen(key,"r");
  // Checking if the file is open
  if (fd == NULL){
    printf("Error: Can't open file in reduce\n");
    exit(1);
  }
  
  // reading the word from the file into "word"
  char word[MAXWORDSZ];
  fscanf(fd, "%s", word);
    
  int i;          // Place holder to read array of 1's
  int value = 0;  // Count how many 1's in the valueList
  while(!feof (fd)) {   // Running to the end of file
    fscanf(fd, "%d", &i); //scanning the list of 1s
    value++; // summing the values of word.
  }
  
  // insert word and value into our final <key, value> data structure
  GlobalRoot = insertNewKeyValue(GlobalRoot, word, value);
  
  // Close the file after reading
  fclose(fd);
  return GlobalRoot;
}

// write the contents of the final data structure
// to output/ReduceOut/Reduce_reducerID.txt
void writeFinalDS(int reducerID, finalKeyValueDS* GlobalRoot){
  // Creating a path to store the reducer file
  char word[MAXPATHSIZE] = "output/ReduceOut/Reducer_";
  char temp[MAXKEYSZ];
  sprintf(temp, "%d",reducerID);
  strcat(word,temp);
  strcat(word,".txt");        // word holds the full path
  FILE *fd = fopen(word,"w"); // Open the file to write
  
  if (fd == NULL){ //checks for success of file creation
    printf("Error: File can't be created");
    exit(1);
  }
  
  // Create a pointer to the root and iterate through the entire data structure 
  // and print it in the format of <"word" "count"> to the Reducer_reducerID.txt file
  finalKeyValueDS * tempNode = GlobalRoot;
  while (tempNode != NULL){
    fputs(tempNode -> key,fd);
    fputs(" ",fd);
    sprintf(temp, "%d", tempNode -> value); // read the value and store it into string variable temp
    fputs(temp,fd);
    fputs("\n",fd);
    tempNode = tempNode -> next; //iterating
  }
  fclose(fd);  //close the file
}

int main(int argc, char *argv[]) {

	if(argc < 2){
		printf("Less number of arguments.\n");
		printf("./reducer reducerID");
	}

  finalKeyValueDS *GlobalRoot = NULL;
	// ###### DO NOT REMOVE ######
	// initialize 

	int reducerID = strtol(argv[1], NULL, 10);

	// ###### DO NOT REMOVE ######
	// master will continuously send the word.txt files
	// alloted to the reducer
	char key[MAXKEYSZ];
	while(getInterData(key, reducerID)) {
		GlobalRoot = reduce(key, GlobalRoot);
  }

  // You may write this logic. You can somehow store the
  // <key, value> count and write to Reduce_reducerID.txt file
  // So you may delete this function and add your logic
  writeFinalDS(reducerID, GlobalRoot);
  
  // Checking if GlobalRoot is null before calling writeFinalDS
  if (GlobalRoot != NULL) {
    freeFinalDS(GlobalRoot);
  }

	return 0;
}