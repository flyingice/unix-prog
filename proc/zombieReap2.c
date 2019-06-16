#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void signalHandler(int signal) {
  pid_t pid;
  while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
    // just to play with, not good practice.
    // The write is not buffered, but on MacOS I can not see the output.
    // I didn't get why.
    write(STDOUT_FILENO, "OK", 2);
  }
}

/*
 * A more elegant way to fork a worker running in parallel with the parent,
 * who may not want to call waitpid() on the child process immediately.
 */
int main() {
  pid_t pid;
  pid = fork();
  if (pid == -1) {
    perror("failed to fork the child\n");
    return EXIT_FAILURE;
  }
  else if (pid == 0) {  // child
    printf("child ready to work\n");
    sleep(5);
  }
  else {  // parent
    /*
     * signal(SIGCHLD, signalHandler) has been deprecated due to the fact
     * that the signal handler will be reset to the default one once the
     * signal is received.
     */
    struct sigaction action;
    action.sa_handler = signalHandler;
    // setup the singal handler for SIGCHLD, which is generated when the
    // status of the child process changes.
    if (sigaction(SIGCHLD, &action, NULL) == -1) {
      perror("failed to register signal handler\n");
      return EXIT_FAILURE;
    }
    while (1)
      ;
  }

  return EXIT_SUCCESS;
}