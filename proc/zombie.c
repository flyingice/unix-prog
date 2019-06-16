#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  pid_t pid;
  // fork a child process
  pid = fork();
  if (pid < 0) {  // error occurred
    fprintf(stderr, "fork failed");
    return EXIT_FAILURE;
  }
  else if (pid == 0) {  // child process
    execlp("/bin/ls", "ls", NULL);
    /* code here won't have any effect since there is
     * no way back after a successful exec family system call
     */
  }
  else {  // parent process
    // parent enters infinite loop and never calls wait
    while (1)
      ;
    printf("parent complete\n");
  }

  return EXIT_SUCCESS;
}

/*
 * Checking with ps -l, the child process has become a zombie process.
 * The program exits while its entry in PCB remains
 * To remove the zombie process, we have to kill its parent so that the defunct process becomes an orphan.
 * Unix assigns the init process as the new parent to the orphan process. The init process periodically
 * invokes wait, therby allowing the exit status of any orphaned process to be collected and releasing the
 * orphan's PID and PCB entry.
 * UID   PID  PPID        F CPU PRI NI       SZ    RSS WCHAN     S             ADDR TTY           TIME CMD
 * 501  3396  3395     4006   0  31  0  4296240   1020 -      S                   0 ttys000    0:01.66 -bash
 * 501  5886  3396     4006   0  31  0  4267728    792 -      R                   0 ttys000    3:07.46 ./a.out
 * 501  5887  5886     6006   0   0  0        0      0 -      Z                   0 ttys000    0:00.00 (ls)
 */
