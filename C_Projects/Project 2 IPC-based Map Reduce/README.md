#### Test machine: csel-kh4250-16
#### Date: Nov-4-2020
#### Names: 
- Lin Huynh
- Aunya Mukherjee
- Hans Bakken
#### x500s: huynh365, mukhe074, bakke496
- huynh365@umn.edu
- mukhe074@umn.edu
- bakke496@umn.edu

## The purpose of your program
* MapReduce [#nMappers, #nReducers] uses two functions (map and reduce) to process large datasets. It also uses functions: sendChunkData, getChunkData, shuffle and getInterData to access and send data. sendChunkData() divides the file into chunks of maximum size 1024B and stores them in a queue. getChunkData() retrieves these chunks one by one and sends them back to the mapper. shuffle() partitions the generated word.txt files across different reducers based on a hash function. getInterData() retrieves these wordFilePathNames and sends them back to the reducer.
* It allows automatic parallelization of computation across multiple machines using multiple processes. Mapreduce model is widely used in industry for Big Data Analytics and is the de-facto standard for Big Data computing!
## How to compile the program
* Use the Makefile to make the exectuables. Once this is done, we can call the program using *"./mapreduce #numberOfMappers #numberOfReducers inputFilePath" *
## What exactly your program does
* **MapReduce**
  *Argument:* MapReduce phase takes in 3 arguements, the number of mappers, the number of reducers, and the input file path. 
  MapReduce phase begins by creating MapOut and ReduceOut folders for later output. Then fom accessing the input data file, passed in from the command line, it creates chunks of data to a queue that the mappers will later access. We then fork nMappers amount of child proccesses to execute the Mappers code and wait for them all to be completed. Then, we shuffle the Mapper output files into the correct number of sections for the reducers. Again, we fork nReducers amount of child proccesses to execute the Reducer code and wait for them to all complete. When this is done, we have concluded the program. Before ending, mapreduce destroys all message queues used in the program, and then we return. 
* **Mapper** 
  *Argument:* Mapper phase takes in one input, the mapper's id. This input will be assigned by the MapReducer.
  First, Mapper calls createMapDir() to create output/MapOut/Map_mapperID folder where the generated word.txt will be store.
  Mapper calls getChunkData() to retrieve the chunks sent from MapReduce one by one and process these chunks.
  Mapper iterates through the chunks, retrieves the words, adding the word into the intermediate data structure if the word has not existed yet, or adding a "1" into the data structure's value list for everytime it encounters the word.
  After reading all of the chunks, and writing the data into the intermediate Data Structure, we create files corresponding to each word and write the word and the list of 1s into the file. When we have successfully written all words in the data structure into files, we free our data structure and return.
  *Output:* word.txt files containing the word and list of 1s. 

* **Reducer**
  *Argument:* Reducer phase takes in one input, the reducer's id. This input is assigned by MapReducer.
  From mapreduce, reducer recieves a constant stream of input files for reducer to read. We use our reducer function to synthesize the information from the multiple files into 1 unique word and the total of the counts of that word. Once we have all of the words and their values, we write them into a final linked list. From here we, can read the words and their counts into a file in ReduceOut, entitled Reducer_reducerID.txt. If this is all done successfully, we then free our data structure and return.
  *Output:* Reducer_reducerID.txt which contains the final count corresponding to each word assigned to the reducer from the master.

  **SendChunkData** 
  *Argument:* SendChunkData takes in two inputs: string inputFile, int nMappers - the file path and the number of mappers 
  We open a unique message queue, through which the GetChunkData would also get access to to read the message. SendChunkData then opens the file and parse through the file, divide the content of the file into chunks of approximately 1024 bytes. SendChunkData needs to make sure that no word is splitted during the process of cutting the file. Chunks will be send to different mappers depend on their mapper ID (through tag in message queue). After sending all of the chunks, SendChunkData sends "END" message to each mapper to signal the end of the message. It then exits the program. 

  **GetChunkData** 
  *Argument:* GetChunkData takes in one input, the mapper's id.
  We open the same message queue as SendChunkData, and receive the message from the message queue that is addressed to us. We return this message as a char array back to mapper. If we encounter an "END" message from SendChunkData, we return NULL and finish the program.  
  
  **Shuffle** 
  *Argument:* Shuffle takes in two inputs, the number of mappers and number of reducers.
  Shuffle looks at each folder from the mappers and uses a hash function to distrubute each file to the reducers. It uses a message queue to send the file path to the reducers by way of GetInterData. The message type is set to the reducer # so that the recieving reducers can tell which files are for them to work on. Once all files have been distrubuted and sent to the queue, END messages are sent to confirm that function has finshed. 
  *Output:* Void return, but sends file paths to reducers.
  
  **GetInterData** 
  *Argument:* GetInterData takes in two inputs, key: a string char array place holder for the wordFileNamePath, and the reducer's id.
  We open the same message queue as Shuffle, and receive the wordFileNamePath from the message queue that is addressed to us. We put this into our key place-holder, and return this back to reducer. If we encounter an "END" message from Shuffle, we return 0 and finish the program.
  *Output:* key, which contains the wordFileName given by Shuffle.
## Any assumptions outside this document
* First assumption: Any input file will end with a termination character (e.g. '\n', '\000')
* Second assumption: If we recieve an empty file as input, we will not exit the program, but instead generate empty outputs and print error statements to console.
* Fourth assumption: We assume that the length of the absolute path of any file passed in is less than 50
## Contribution by each member of the team
* Lin focused on * sendChunkData *
* Aunya focused on * getChunkData * and * getInterData *
* Hans focused on * shuffle *
* We worked on the preliminary drafts of all of the functions separately, and then put them together and debugged together! *
* All members contribute to *utils.c* with pair coding *(Repl.it aided in the success of paired coding)*
  
