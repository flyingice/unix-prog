#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

// The following code snippet demontrates communication between parent and child process
int main() {
  char write_msg[BUFFER_SIZE] = "Hello World!";
  char read_msg[BUFFER_SIZE];
  int fd[2];
  pid_t pid;

  // create an unnamed pipe, communication requires a parent-child relationship
  if (pipe(fd) == -1) {
    fprintf(stderr, "pipe creation failure");
    return EXIT_FAILURE;
  }

  // fork a child process, the child process will inherit the pipe
  pid = fork();
  if (pid < 0) {  // error occured
    fprintf(stderr, "fork failure");
    return EXIT_FAILURE;
  }

  if (pid > 0) {  // parent process
    // close the unused end of the pipe
    close(fd[READ_END]);
    // write to the pipe
    write(fd[WRITE_END], write_msg, strlen(write_msg) + 1);
    // close the write end of the pipe
    close(fd[WRITE_END]);
  }
  else {  // child process
    // close the unused end of pipe
    close(fd[WRITE_END]);
    // read from the pipe
    read(fd[READ_END], read_msg, BUFFER_SIZE);
    printf("read complete: %s\n", read_msg);
    // close the write end of the pipe
    close(fd[READ_END]);
    exit(EXIT_SUCCESS);
  }

  wait(NULL);

  return EXIT_SUCCESS;
}
