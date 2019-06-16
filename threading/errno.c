#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREAD 10

/* The following code shows that the errno is thread-local */
void* runner(void* param) {
  printf("The errno address of thread %p is %p\n", pthread_self(), &errno);

  pthread_exit(EXIT_SUCCESS);
}

int main() {
  pthread_t threads[NUM_THREAD];

  for (int i = 0; i < NUM_THREAD; i++) {
    pthread_create(&threads[i], NULL, runner, NULL);
  }

  for (int i = 0; i < NUM_THREAD; i++) {
    pthread_join(threads[i], NULL);
  }

  return EXIT_SUCCESS;
}