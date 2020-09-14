#ifndef __STDOUT_REDIRECTION_H__
#define __STDOUT_REDIRECTION_H__

#include <stdio.h>
#include <unistd.h>

void stdout_redirection_init(void);
void std_out_redirection_get_buffer(char *buff, int size);
void std_out_redirection_fini(void);

#endif

#ifdef __IMPLEMENT_STDOUT_REDIRECTION__
#include <stdio.h>
#include <unistd.h>

static int  stdout_bk; //is fd for stdout backup
static int pipefd[2];

void stdout_redirection_init(void){

   printf("Initializing stdout redirection\n");
   stdout_bk = dup(fileno(stdout));

   pipe2(pipefd, 0); // O_NONBLOCK);

   // What used to be stdout will now go to the pipe.
   dup2(pipefd[1], fileno(stdout));

   printf("stdout redirection initialized. Loging to buffer\n");
   fflush(stdout);//flushall();
   close(pipefd[1]);
}

void std_out_redirection_get_buffer(char *buff, int size){
    read(pipefd[0], buff, size); 
}

void std_out_redirection_fini(void) {
   fflush(stdout);//flushall();
   write(pipefd[1], "good-bye", 9); // null-terminated string!
   close(pipefd[1]);

   dup2(stdout_bk, fileno(stdout));//restore
   printf("this is now\n");

   char buf[1000];
   read(pipefd[0], buf, 1000); 
   printf("got this from the pipe >>>%s<<<\n", buf);
}

#undef __IMPLEMENT_STDOUT_REDIRECTION__
#endif 

