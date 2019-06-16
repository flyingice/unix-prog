#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

// The following code snippet demontrates the communication between two sibling processes
int main() {
  char write_msg[BUFFER_SIZE] = "Hello World!";
  char read_msg[BUFFER_SIZE];
  int fd[2];
  pid_t pid[2];

  // create an unnamed pipe, communication requires a parent-child relationship
  if (pipe(fd) == -1) {
    fprintf(stderr, "pipe creation failure");
    return EXIT_FAILURE;
  }

  // fork two child processes, the child process will inherit the pipe
  for (int i = 0; i < 2; ++i) {
    pid[i] = fork();
    if (pid[i] < 0) {  // error occured
      fprintf(stderr, "fork failure");
      return EXIT_FAILURE;
    }

    if (pid[i] == 0) {  // child process
      switch (i) {
        case 0:  // 1st child
          close(fd[READ_END]);
          write(fd[WRITE_END], write_msg, strlen(write_msg) + 1);
          printf("write complete\n");
          close(fd[WRITE_END]);
          break;
        case 1:  // 2nd child
          close(fd[WRITE_END]);
          read(fd[READ_END], read_msg, BUFFER_SIZE);
          printf("read complete: %s\n", read_msg);
          close(fd[READ_END]);
          break;
        default:
          break;
      }
      exit(EXIT_SUCCESS);
    }
  }

  // we can't close the pipe until both processes have been forked
  close(fd[READ_END]);
  close(fd[WRITE_END]);

  // wait for all the child processes to finish
  for (int i = 0; i < 2; ++i) {
    wait(NULL);
  }
  printf("parent complete\n");

  return EXIT_SUCCESS;
}
