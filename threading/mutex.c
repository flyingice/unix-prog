#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static int counter;
// static initialization, otherwise, it is required to call pthread_mutex_init(&mutex);
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

#define NUM_THREAD 10

void* runner(void* param) {
  pthread_mutex_lock(&mutex);

  counter++;
  printf("counter=%d\n", counter);

  pthread_mutex_unlock(&mutex);
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

  // lock the same mutex with default type attribute twice leads to deadlock
  // pthread_mutex_lock(&mutex);
  // pthread_mutex_lock(&mutex);

  // mutex with ERRORCHECK type attribute can detect deadlock and returns error
  pthread_mutexattr_t att;
  pthread_mutexattr_init(&att);
  pthread_mutexattr_settype(&att, PTHREAD_MUTEX_ERRORCHECK);

  pthread_mutex_t mutexAnother;
  pthread_mutex_init(&mutexAnother, &att);

  pthread_mutex_lock(&mutexAnother);
  if (pthread_mutex_lock(&mutexAnother)) {
    printf("error occurred when locking the mutex");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}