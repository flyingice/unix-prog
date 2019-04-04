#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int sum;                    // the data is shared by the thread
void* runner(void* param);  // thread calls this function

int main(int argc, char* argv[]) {
  pthread_t tid;        // thread identifier
  pthread_attr_t attr;  // set of thread attributes

  if (argc != 2) {
    fprintf(stderr, "usage: a.out <integer value>\n");
    return EXIT_FAILURE;
  }

  if (atoi(argv[1]) < 0) {
    fprintf(stderr, "%d must not be less than 0\n", atoi(argv[1]));
    return EXIT_FAILURE;
  }

  // get the default attributes
  pthread_attr_init(&attr);
  // create the thread
  pthread_create(&tid, &attr, runner, argv[1]);
  // wait for the thread to exit
  pthread_join(tid, NULL);

  printf("sum = %d\n", sum);

  return EXIT_SUCCESS;
}

// the thread will begin control in this function
void* runner(void* param) {
  int i, upper = atoi(param);
  sum = 0;
  for (int i = 1; i <= upper; ++i) sum += i;

  // thread termination doesn't release application resources as exit(() does
  pthread_exit(EXIT_SUCCESS);
}
