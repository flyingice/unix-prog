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
  int shm_fd;               // shared memory file descriptor
  void* ptr;                // pointer to shared memory object

  // open the shared memory object
  shm_fd = shm_open(name, O_RDONLY, 0666);
  // memory map the shared memory object
  ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
  // read from the shared memory object
  printf("%s\n", (char*)ptr);
  // remove the shared memory object
  shm_unlink(name);

  return EXIT_SUCCESS;
}
