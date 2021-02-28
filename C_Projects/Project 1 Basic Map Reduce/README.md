#### Test machine: Vole-18
#### Date: Oct-4-2020
#### Names: 
- Lin Huynh
- Aunya Mukherjee
- Hans Bakken
#### x500s: huynh365, mukhe074, bakke496
- huynh365@umn.edu
- mukhe074@umn.edu
- bakke496@umn.edu

## The purpose of your program
* MapReduce [#nMappers, #nReducers] uses two functions (map and reduce) to process large datasets.
* It allows automatic parallelization of computation across multiple machines using multiple processes. Mapreduce model is widely used in industry for Big Data Analytics and is the de-facto standard for Big Data computing!
## How to compile the program
* Use the Makefile to make the exectuables. Once this is done, we can call the program using *"./mapreduce #numberOfMappers #numberOfReducers inputFilePath" *
## What exactly your program does
* **MapReduce**
  *Argument:* MapReduce phase takes in 3 arguements, the number of mappers, the number of reducers, and the input file path. 
  MapReduce phase begins by creating MapOut and ReduceOut folders for later output. Then fom accessing the input data file, passed in from the command line, it creates chunks of data to a queue that the mappers will later access. We then fork nMappers amount of child proccesses to execute the Mappers code and wait for them all to be completed. Then, we shuffle the Mapper output files into the correct number of sections for the reducers. Again, we fork nReducers amount of child proccesses to execute the Reducer code and wait for them to all complete. When this is done, we have concluded the program, and we return. 
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

## Any assumptions outside this document
* First assumption: Any input file will end with a termination character (e.g. '\n', '\000')
* Second assumption: If we recieve an empty file as input, we will not exit the program, but instead generate empty outputs and print error statements to console.
## Contribution by each member of the team
* All members contribute to *mapper.c*, *mapreduce.c*, and *reducer.c* with pair coding *(Repl.it aided in the success of paired coding)*
* Lin and Aunya made additional test cases, composed *README.md*, and made comments through out 
* Hans is in charge of contacting TAs and instructor about questions and the validity of the assumptions that the group made.   
