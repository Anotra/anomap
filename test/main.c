
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TEST_FEATURE(name)                     \
  do {                                         \
    int test_ ## name(void);                   \
    if (0 == strcmp(#name, argv[1]))           \
      return test_ ## name();                  \
  } while (0)

static int _run_test(int argc, char *argv[]);

int main(int argc, char *argv[]) {
  if (argc != 2)
    return 1;
  return _run_test(argc, argv);
}

int
_run_test(int argc, char *argv[]) {

#ifdef WITH_LOCKS
  TEST_FEATURE(locks);
#endif

  TEST_FEATURE(ordering);

  return -1;
}