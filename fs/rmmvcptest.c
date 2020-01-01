#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const char* filename = "infile.txt";
const static int BUFFER_SIZE = 50;

/*
 * The following program keeps infile.txt open forever and prints the file content with a sequence id
 *
 * Test 1) rm infile.txt
 * The file will disappear in the directory, but the space used is not claimed.
 * The running process continue printing the intended content from the original infile.txt
 * Why? Check 'man 2 unlink'
 *
 * Test 2) echo 'xxx' > infile2.txt && mv infile2 infile.txt
 * Command ok. The original content of infile.txt will be replaced by 'xxx'.
 * However, the running process continue printing the original content of infile.txt
 * Why? Check 'man 2 rename'
 *
 * Test 3) echo 'yyy' > infile2.txt && cp infile2.txt infile.txt
 * Command ok. The original content of infile.txt will be replaced by 'yyy'.
 * Now the running process start printing 'yyy' instead of the original content from infile.txt
 * Why? When the target exists, cp will open the target with flag O_CREAT | O_TRUNC. The inode id
 * of infile.txt remains the same.
 *
 * Test 4) echo 'yyy' > infile.txt
 * The result is the same as Test 3).
 * Why? The redirection truncates the data within infile.txt
 */

int main() {
  int fd = open(filename, O_RDWR);
  if (fd == -1) {
    perror("failed to open the file");
    return EXIT_FAILURE;
  }

  char buffer[BUFFER_SIZE] = {};
  int i = 0;
  while (++i) {
    printf("%d: ", i);
    fflush(stdout);
    size_t cnt = 0;
    while ((cnt = read(fd, buffer, BUFFER_SIZE))) {
      write(STDOUT_FILENO, buffer, cnt);
    }
    lseek(fd, 0, SEEK_SET);
    sleep(2);
  }

  return EXIT_SUCCESS;
}