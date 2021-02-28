#### Test machine: csel-kh4250-20
#### Date: Dec-14-2020
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
* The purpose of our program is to construct a multi-threaded web server using POSIX threads (pthreads) in the C language. The way in which we implement this is to have two types of threads created by the main process of the program. These are dispatcher and worker threads. Dispatcher threads repeatedly accept an incoming connection, read the client request from the connection, and place the request in a queue. Worker threads then monitor the request queue, retrieve requests and serve the request’s result back to the client. We utilise synchronization techniques such as locks to ensure that our threads are safely accessing the critical section/ shared resources.
* For this project, we used socket programming to rewrite the utility functions that were given to us previously. The names of these and their descriptions are as follows:

### void init(int port) 
- parameters:
  - port is the number of the port you want the server to be started on
- initializes the connection acception/handling system
- if init encounters any errors, it will call exit().

### int accept_connection(void)
   - returns a file descriptor for further request processing.
   - if the return value is negative, the request will be ignored.

### int get_request(int fd, char *filename);
   - parameters:
      - fd is the file descriptor obtained by accept_connection() from where you wish to get a request
      - filename is the location of a character buffer in which this function should store the requested filename. 
   - returns 0 on success, nonzero on failure. Accounts for failures because some connections might send faulty requests.

### int return_result(int fd, char *content_type, char *buf, int numbytes)
   - returns the contents of a file to the requesting client and cleans
     up the connection to the client
   - parameters:
      - fd is the file descriptor obtained by accept_connection() to where you wish to return the result of a request
      - content_type is a pointer to a string that indicates the type of content being returned.
      - buf is a pointer to a memory location where the requested file has been read into memory (the heap). return_result
        will use this memory location to return the result to the user.
      - numbytes is the number of bytes the file takes up in buf
   - returns 0 on success, nonzero on failure.

### int return_error(int fd, char *buf)
   - returns an error message in response to a bad request and cleans up the connection to the client
   - parameters:
      - fd is the file descriptor obtained by accept_connection() to where you wish to return the error
      - buf is a pointer to the location of the error text
   - returns 0 on success, nonzero on failure.

### void \*persistant_Connection(void \*arg)
  - parameters:
    - takes 1 arguments which is the socket id of the persistant connection.
  - periodically checks if 5 seconds of inactivity has occured
  - If inactive, closes socket and returns

## Extra credit implementations
* We attempted the Extra Credit by reading the buffer from the request and deciding whether it reads Connention: Close or Keep Alive. Based on this, we raise a flag to indicate persitant connection is desired and then spawn a deatched thread on persistantConnection which periodically checks the time between startTime and the current time. StartTime is updated to the current time every time a request is recieved or sent. Once the differce between the two exceeds 5 seconds, the persistantConnection function closes the socket and exits the thread. The implementation would only work with 1 thread, so for mutliple workers/ dispatchers, there would either have to be an array of flags and socket ids to keep track of for each thread, or have the dispatch function handle each instead.

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
* All members contribute to *util.c* with pair coding *(Repl.it aided in the success of paired coding)*

