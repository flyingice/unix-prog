/*
 * procfs that is quite popular among *nix is not implemented on macOS.
 * As a result, retrieving any process name is not trivial.
 * The following code is inspired from what BSD version of `ps` command
 * does on macOS 10.14.1:
 * https://opensource.apple.com/source/adv_cmds/adv_cmds-172/ps/
 *
 * More info about sysctl:
 * https://www.freebsd.org/cgi/man.cgi?sysctl(3)
 *
 */

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysctl.h>

#define PID(kinfo) kinfo->kp_proc.p_pid

typedef struct kinfo_proc KINFO;

static int getproclist(KINFO** k, size_t* cnt) {
  // management information base
  int mib[] = {
    CTL_KERN,
    KERN_PROC,
    KERN_PROC_ALL,
  };

  size_t size = 0;
  if (sysctl(mib, sizeof(mib) / sizeof(mib[0]), NULL, &size, NULL, 0) == -1) goto ERROR;

  *k = (KINFO*)malloc(size);
  if (*k == NULL) goto ERROR;

  if (sysctl(mib, sizeof(mib) / sizeof(mib[0]), *k, &size, NULL, 0) == -1) goto ERROR;
  *cnt = size / sizeof(KINFO);

  return 0;

ERROR:
  free(*k);
  *k = NULL;
  return -1;
}

static int getprocargmax(size_t* argmax) {
  int mib[] = {
    CTL_KERN,
    KERN_ARGMAX,
  };

  size_t size = sizeof(*argmax);
  return sysctl(mib, sizeof(mib) / sizeof(mib[0]), argmax, &size, NULL, 0);
}

static int getprocargs(KINFO* k, char* procargs, size_t size) {
  assert(k);

  int mib[] = {
    CTL_KERN,
    KERN_PROCARGS2,
    PID(k),
  };

  if (sysctl(mib, sizeof(mib) / sizeof(mib[0]), procargs, &size, NULL, 0) == -1) goto ERROR;

  // save argc
  int nargs = 0;
  memcpy(&nargs, procargs, sizeof(nargs));

  // skip argc
  char* cp = procargs + sizeof(nargs);
  // skip the exec_path
  while (cp < procargs + size) {
    if (*cp == '\0') break;
    cp++;
  }
  if (cp == procargs + size) goto ERROR;

  // skip trailing '\0' characters
  while (cp < procargs + size) {
    if (*cp != '\0') break;
    cp++;
  }
  if (cp == procargs + size) goto ERROR;

  // cp points to where argv[0] starts
  printf("pid=%d, arg[0]=%s\n", PID(k), cp);

  return 0;

ERROR:
  return -1;
}

int main(void) {
  KINFO* k = NULL;
  char* procargs = NULL;

  size_t cnt = 0;
  if (getproclist(&k, &cnt) == -1) goto ERROR;
  printf("process count: %zu\n", cnt);

  size_t argmax = 0;
  if (getprocargmax(&argmax) == -1) goto ERROR;

  procargs = (char*)malloc(argmax);
  if (procargs == NULL) goto ERROR;

  size_t failed = 0;
  for (size_t i = 0; i < cnt; i++) {
    if (getprocargs(&k[i], procargs, argmax) == -1) {
      printf("ERROR: %s\n", strerror(errno));
      failed++;
    }
  }
  printf("process count=%zu, retrieval failure=%zu\n", cnt, failed);

  free(k);
  free(procargs);

  return EXIT_SUCCESS;

ERROR:
  return EXIT_FAILURE;
}