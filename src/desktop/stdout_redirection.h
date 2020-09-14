#ifndef __STDOUT_REDIRECTION_H__
#define __STDOUT_REDIRECTION_H__

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "../dstring/dynamic_string.h"

void stdout_redirection_init(void);
void std_out_redirection_update_buffer(dstring_t* buffer);
void std_out_redirection_fini(void);

#endif

#ifdef __IMPLEMENT_STDOUT_REDIRECTION__
#include <stdio.h>
#include <unistd.h>

static int  stdout_bk; //is fd for stdout backup
static int  stderr_bk; //is fd for stdout backup
static int pipefd[2];
static int pipeerrfd[2];

void stdout_redirection_init(void){

   printf("Initializing stdout redirection\n");
   stdout_bk = dup(fileno(stdout));
   stderr_bk = dup(fileno(stderr));

   pipe2(pipefd, O_NONBLOCK);

   // What used to be stdout will now go to the pipe.
   dup2(pipefd[1], fileno(stdout));
   dup2(pipeerrfd[1], fileno(stderr));

   printf("stdout redirection initialized. Loging to buffer\n");
   fflush(stdout);
   fflush(stderr);
   close(pipefd[1]);
   close(pipeerrfd[1]);
}

void std_out_redirection_update_buffer(dstring_t* buffer){
   char temp_buf[1001] = {0};
   read(pipefd[0], temp_buf, 1000);
   dstring_append_char_ptr(buffer, temp_buf);
   read(pipeerrfd[0], temp_buf, 1000);
   dstring_append_char_ptr(buffer, temp_buf);
}

void std_out_redirection_fini(void) {
   close(pipefd[1]);
   close(pipeerrfd[1]);

   dup2(stdout_bk, fileno(stdout));//restore
   dup2(stderr_bk, fileno(stderr));//restore
}

#undef __IMPLEMENT_STDOUT_REDIRECTION__
#endif 

