#### Test machine: csel-kh4250-20
#### Date: Nov-21-2020
#### Names: 
- Lin Huynh
- Aunya Mukherjee
- Hans Bakken
#### x500s: huynh365, mukhe074, bakke496
- huynh365@umn.edu
- mukhe074@umn.edu
- bakke496@umn.edu

## How to compile and run the program
* Use the Makefile to make the exectuables. Once this is done, we can call the program using *"% ./web_server port path num_dispatch num_workers dynamic_flag qlen cache_entries". To test request handling and download a single file from the server, use "wget http://address:port/'file_main_type'/'file_extension'/'file'", eg " wget http://127.0.0.1:9000/image/jpg/29.jpg" on a different terminal or computer*

## What exactly your program does
* The purpose of our program is to construct a multi-threaded web server using POSIX threads (pthreads) in the C language. The way in which we implement this is to have two types of threads created by the main process of the program. These are dispatcher and worker threads. Dispatcher threads repeatedly accept an incoming connection, read the client request from the connection, and place the request in a queue. Worker threads then monitor the request queue, retrieve requests and serve the request’s result back to the client. We utilise synchrnoization techniques such as locks to ensure that our threads are safely accessing the critical section/ shared resources.

## Extra credit implementations
* We attempted both Extra Credit A and Extra Credit B
### Extra Credit A - Dynamic Worker Thread Pool
* To accomplish dynamic threads, our dynamic thread function checks the difference between the number of requests and the number that has been served. If the difference is larger than the current amount of threads, threads will be created to serve these requests. If the difference is smaller than the number of threads, the extra threads will be cancelled. A simple global int array that corresponds to each thread number index will tell the thread whether it should continue or return after serving a request.
### Extra Credit B - Caching
* For the caching implementation, we chose to use a FIFO queue to store the requests in. That is to say, once the queue is full, we reset the index to 0 and start to overwrite the values from the beginning of the queue. We also make sure to malloc and free the fields of our cache_entry structs when we need to. By enabling this, we can more quickly and efficiently access requests that we have used in the past.

## Any assumptions outside this document
* We assume that an invalid port will not terminate the program
We use the assumptions stated in the project write up, as follows
* The maximum number of dispatcher threads will be 100.
* The maximum number of worker threads will be 100.
* The maximum length of the request queue will be 100 requests.
* The maximum size of the cache will be 100 entries.
* The maximum length of filename will be 1024.
* Any HTTP request for a filename containing two consecutive periods or two consecutive slashes (“..” or “//”) will automatically be detected as a bad request by our compiled code for security.

## Contribution by each member of the team
* All members contribute to *server.c* with pair coding *(Repl.it aided in the success of paired coding)*
* Lin and Hans focused on the implementation of the Dynamic Thread Pool Extra credit question
* Aunya and Hans focused on the cache implementation Extra credit question
