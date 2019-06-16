#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define SIZE 10

int main() {
  int fd;
  char* addr = NULL;
  const char* name = "test.log";

  if ((fd = open(name, O_CREAT | O_RDWR | O_TRUNC, 0666)) == -1) {
    perror("failed to open the file");
    return EXIT_FAILURE;
  }

  // It is required to adjust the file size. Otherwise,
  // EXC_BAD_ACCES will be thrown when accessing the memory via addr
  if (ftruncate(fd, SIZE) == -1) {
    perror("failed to truncate the file size");
    return EXIT_FAILURE;
  }

  if ((addr = (char*)mmap(0, SIZE, PROT_WRITE, MAP_SHARED, fd, 0)) == NULL) {
    perror("failed to map the file into the memory");
    return EXIT_FAILURE;
  }

  // The change actually goes into the file in the back
  for (int i = 0; i < SIZE; i++) {
    addr[i] = 'a' + i;
  }

  msync(addr, SIZE, MS_SYNC);
  munmap(addr, SIZE);

  // check file content
  char buf[SIZE];
  read(fd, buf, SIZE);
  fwrite(buf, sizeof(char), SIZE, stdout);
  close(fd);

  // remove the file
  unlink(name);

  return EXIT_SUCCESS;
}