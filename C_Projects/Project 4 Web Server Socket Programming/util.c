#ifndef _REENTRANT
#define _REENTRANT
#endif

#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "util.h"

#define BACKLOG 20
#define EXIT_FAILURE 1
#define BUFFERSZ 1024
#define MAXGETSZ 2048
#define MAXSENDSZ 200

int socketfd;
struct sockaddr_in addr;
//time_t now;

/**********************************************
 * init
   - port is the number of the port you want the server to be
     started on
   - initializes the connection acception/handling system
   - YOU MUST CALL THIS EXACTLY ONCE (not once per thread,
     but exactly one time, in the main thread of your program)
     BEFORE USING ANY OF THE FUNCTIONS BELOW
   - if init encounters any errors, it will call exit().
************************************************/
void init(int port) {
  int opt = 1;
  if ((socketfd = socket(PF_INET, SOCK_STREAM, 0)) == 0){
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  addr.sin_family= AF_INET;
  addr.sin_addr.s_addr= inet_addr("127.0.0.1");
  addr.sin_port= htons(port);
  // Forcefully attaching socket to the chosen port  
  if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char*)&opt, sizeof(opt)) < 0) 
  { 
      perror("setsockopt"); 
      exit(EXIT_FAILURE); 
  }
   
  
  if (bind(socketfd, (struct sockaddr *)&addr,sizeof(addr))<0) 
  { 
    perror("bind failed: port already taken"); 
    exit(EXIT_FAILURE); 
  }

  if (listen(socketfd, BACKLOG) == -1) {    
    perror("Could not listen");    
    exit(EXIT_FAILURE);  
  }

  //printf("Server Start success\n");

}

/**********************************************
 * accept_connection - takes no parameters
   - returns a file descriptor for further request processing.
     DO NOT use the file descriptor on your own -- use
     get_request() instead.
   - if the return value is negative, the request should be ignored.
***********************************************/
// int lastFD;
// int persistantFlag;
// time_t startTime;


int accept_connection(void) {
  //if (!persistantFlag) {
    int new_socket;
    int addrlen = sizeof(addr);
    new_socket = accept(socketfd, (struct sockaddr *)&addr,(socklen_t*)&addrlen);
    //printf("Connection Accepted\n");
    //lastFD = new_socket;
    return new_socket;
  //}
  // else {
  //   return lastFD;
  // }
}
/*
void *persistant_Connection(void *arg) {
  int id = *(int *) arg;
  while(difftime(startTime, time(NULL)) < 5) {
    sleep(1);
  }
  persistantFlag = 0;
  if (close(id) < 0) {
    perror("Socket Close problem");
    //return;
  }
}*/
/**********************************************
 * get_request
   - parameters:
      - fd is the file descriptor obtained by accept_connection()
        from where you wish to get a request
      - filename is the location of a character buffer in which
        this function should store the requested filename. (Buffer
        should be of size 1024 bytes.)
   - returns 0 on success, nonzero on failure. You must account
     for failures because some connections might send faulty
     requests. This is a recoverable error - you must not exit
     inside the thread that called get_request. After an error, you
     must NOT use a return_request or return_error function for that
     specific 'connection'.
************************************************/
int get_request(int fd, char *filename) {
  char beforeparse[MAXGETSZ];
  int readsz;
  char firstline[MAXGETSZ];
  memset(firstline, '\0',MAXGETSZ);
  if ((readsz = read(fd, beforeparse, MAXGETSZ)) < 0) { 
    perror ("server read problem");
    return EXIT_FAILURE;
  }
  else{ 
    // extracting the first line of the request
    int spaces = 0;
    //printf("\n\nRequest: %s\n",beforeparse);
    for (int i=0; i<MAXGETSZ; i++)  { 
      if (beforeparse[i] ==' '){
        spaces++;
      }
      firstline[i] = beforeparse[i];
      if (beforeparse[i] == '\n'){
        break;
      }
    }
    // checking if there are at least two strings 
    if (spaces < 1){
      return EXIT_FAILURE;
    }
    
    int spaceflag = 0; //provided "STring1 String2 ", copies "string2"
    //filename = (char*) malloc(MAXGETSZ);
   // memset(filename,'\0',MAXGETSZ);
    char requestType[MAXGETSZ];
    memset(requestType,'\0',MAXGETSZ);
    int filenameLength = 0;
    for (int i=0;i<strlen(firstline);i++) {
      if (!spaceflag && firstline[i] != ' ') {
        requestType[i] = firstline[i];
      }
      else if (!spaceflag && firstline[i] == ' ') {
        spaceflag = 1;
      }
      else if (spaceflag && firstline[i] != ' ') {
        filename[filenameLength] = firstline[i];
        filenameLength++;
      }
      else if (spaceflag && firstline[i] == ' ') {
        break;
      }
    }
    // check if the first word is GET
    if (strcmp(requestType, "GET") !=0){
      printf("Request type is not GET!\n");
      return EXIT_FAILURE;
    } 
  }
  // int countendline = 0;
  // int tempFlag = 0;
  // for (int i=0;i<strlen(beforeparse);i++) {
  //     if (beforeparse[i]=='\n'){
  //       countendline++;
  //     }
  //     if (countendline==5){
  //       printf("The character is %c \n", beforeparse[i+13]);
  //       if (beforeparse[i+13]=='K'){
  //         tempFlag = 1;
  //       }
  //       else{
  //         tempFlag = 0;
  //       }
  //       // printf("The temp flag is %d \n", tempFlag);
  //       break;
  //     }

  //   }


  // if (tempFlag && !persistantFlag) {
  //   //pthread_create(99,&attr,persistant_Connection,(void*));
  //   persistantFlag = 1;
  //   pthread_attr_t attr;
  //   pthread_t timethread;
  //   int s;
  //   s = pthread_attr_init(&attr);
  //   if (s != 0) {
  //     fprintf(stderr, "error - attribute init failed.\n");
  //     exit(EXIT_FAILURE);
  //   }
  //   s = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  //   if (s != 0) {
  //     fprintf(stderr, "error - setting detached state failed.\n");
  //     exit(EXIT_FAILURE);
  //   }
  //   s = pthread_create(&timethread, &attr, persistant_Connection, (void *) &fd);
  //   if (s != 0) {
  //     fprintf(stderr, "error - pthread_create for persistant connection.\n");
  //   }  
  // }
  
  
  //fprintf(stderr, "First Line is %s\n", firstline); 
  //printf("File name: %s\n",filename);
  //startTime = time(NULL);
  return 0;
}



/**********************************************
 * return_result
   - returns the contents of a file to the requesting client
   - parameters:
      - fd is the file descriptor obtained by accept_connection()
        to where you wish to return the result of a request
      - content_type is a pointer to a string that indicates the
        type of content being returned. possible types include
        "text/html", "text/plain", "image/gif", "image/jpeg" cor-
        responding to .html, .txt, .gif, .jpg files.
      - buf is a pointer to a memory location where the requested
        file has been read into memory (the heap). return_result
        will use this memory location to return the result to the
        user. (remember to use -D_REENTRANT for CFLAGS.) you may
        safely deallocate the memory after the call to
        return_result (if it will not be cached).
      - numbytes is the number of bytes the file takes up in buf
   - returns 0 on success, nonzero on failure.
************************************************/
int return_result(int fd, char *content_type, char *buf, int numbytes) {
  char* msg;
  int sendSize = (MAXSENDSZ + numbytes + sizeof(content_type)) * (sizeof(char));
  msg = malloc(sendSize);
  memset(msg,'\0',sendSize);

  char* Start = "HTTP/1.1 200 OK\nContent-Type: ";
  char* Mid = "\nContent-Length: ";
  char *End;
  // if (persistantFlag){    
  //   End = "\nConnection: Keep-Alive\n\n";
  // }
  // else {
  End = "\nConnection: Close\n\n";
  //}

  snprintf(msg, sendSize, "%s%s%s%d%s%s", Start, content_type, Mid, numbytes, End, buf);

  
  if (write(fd,msg,sendSize) < 0) { 
    perror ("server write problem");
    close(fd);
    return EXIT_FAILURE;
  }
  // else if(persistantFlag) {
  //   startTime = time(NULL);
  //   return 0;
  // }
  else if (close(fd) < 0) {
    perror("Socket Close problem");
    return EXIT_FAILURE;
  }
  else {
    return 0;
  }

}

/**********************************************
 * return_error
   - returns an error message in response to a bad request
   - parameters:
      - fd is the file descriptor obtained by accept_connection()
        to where you wish to return the error
      - buf is a pointer to the location of the error text
   - returns 0 on success, nonzero on failure.
************************************************/
int return_error(int fd, char *buf) {
  char* msg;
  int sendSize = (MAXSENDSZ + sizeof(buf)) * (sizeof(char));
  msg = malloc(sendSize);
  memset(msg,'\0',sendSize);

  char* Start = "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: ";
  char* Mid; 
  // if (persistantFlag){    
  //   Mid = "\nConnection: Keep-Alive\n\n";
  // }
  // else {
    Mid = "\nConnection: Close\n\n";
  // }

  snprintf(msg, sendSize, "%s%d%s%s", Start, (int)sizeof(buf), Mid, buf);

  
  if (write(fd,msg,sendSize) < 0) { 
    perror ("server error write problem");
    close(fd);
    return EXIT_FAILURE;
  }
  // else if(persistantFlag) {
  //   startTime = time(NULL);
  //   return 0;
  // }
  else if (close(fd) < 0) {
    perror("Socket error Close problem");
    return EXIT_FAILURE;
  }
  else {
    return 0;
  }
}
