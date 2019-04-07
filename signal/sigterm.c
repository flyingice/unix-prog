#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * If the variable flag is declared without the volatile keyword, the program
 * may not be stopped by the kill command if it is compiled with -O3 optimization
 * level since the variable is loaded only once in the register.
 */
static volatile sig_atomic_t flag = 0;

static void handler(int signal) {
  flag = 1;
}

int main() {
  struct sigaction action;
  action.sa_handler = &handler;
  if (sigaction(SIGTERM, &action, NULL) == -1) {  // default singal sent by kill
    perror("failed to register the signal handler\n");
    return EXIT_FAILURE;
  }

  while (!flag)
    ;

  return EXIT_SUCCESS;
}