#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int redirection1() {
  static const int SZ = 256;
  char ttyName[SZ] = { 0 };
  // save the current terminal name of stdout so that we could restore it later on
  ttyname_r(1, ttyName, SZ);

  FILE* file = freopen("log1", "w", stdout);
  if (file == NULL) {
    perror("can not open file.\n");
    return -1;
  }

  /*
   * It is supposed to send the msg to the stdout.
   * However, the output will be saved in a regular file since the stdout
   * has been associated to a file stream.
   */
  printf("1st approach to redirect stdout to a file.\n");
  fclose(file);

  freopen(ttyName, "w", stdout);
  printf("1st approach to redirect stdout to a file. Restore stdout.\n");

  return 0;
}

int redirection2() {
  FILE* file = fopen("log2", "w");
  if (file == NULL) {
    perror("can not find file.\n");
    return -1;
  }

  close(1);
  dup(fileno(file));
  // dup2(fileno(file), 1); is an equivalent
  printf("2nd approach to redirect stdout to a file\n");
  fclose(file);

  return 0;
}

int main() {
  redirection1();
  redirection2();

  return EXIT_SUCCESS;
}