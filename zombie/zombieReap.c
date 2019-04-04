#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*
 * Here shows the hack of double fork to avoid zombie child when
 * we don't want to call waitpid() from the parent since the parent
 * process may be required to run in parallel, e.g. a daemon
 */
int main() {
  pid_t pid;
  pid = fork();
  if (pid == -1) {
    fprintf(stderr, "fork failed\n");
    return EXIT_FAILURE;
  }
  else if (pid == 0) {  // child process
    printf("The child is ready to spawn grandchild\n");
    pid_t pid2 = fork();
    if (pid2 == -1) {
      fprintf(stderr, "fork failed\n");
      return EXIT_FAILURE;
    }
    else if (pid2 == 0) {  // the real work to be done by the grandchild
      /*
       * The grandchild quickly becomes an orphan since its direct parent dies.
       * As a result, it will be adopted by the init process.
       */
      printf("The grandchild is ready for work\n");
      execlp("sleep", "sleep", "10", NULL);
    }
    else {
      printf("The grandchild was born\n");
    }
  }
  else {                    // parent process
    waitpid(pid, NULL, 0);  // reap the child
    while (1)
      ;
    printf("parent completed");
  }

  return EXIT_SUCCESS;
}