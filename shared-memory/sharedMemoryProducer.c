#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
  const int SIZE   = 4096;  // the size (in bytes) of shared memory object
  const char* name = "OS";  // name of the shared memory object
  // strings written to shared memory
  const char* message_0 = "Hello";
  const char* message_1 = "World!";
  int shm_fd;  // shared memory file descriptor
  void* ptr;   // pointer to shared memory obect

  // create the shared memory object
  shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
  // configure the size of the shared memory object
  ftruncate(shm_fd, SIZE);
  // memory map the shared memory object
  ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
  // write to the shared memory object
  sprintf(ptr, "%s", message_0);
  ptr += strlen(message_0);
  sprintf(ptr, "%s", message_1);
  ptr += strlen(message_1);

  /* POSIX shared memory objects have kernel persistence: a shared memory object
   * will exist until the system shutdown, or until all processes have unmapped
   * the object and it has been deleted with shm_unlink.
   */
  return EXIT_SUCCESS;
}
