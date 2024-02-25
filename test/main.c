
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int test_locks(void);

#define TEST_FEATURE(name)\
  do {\
    if (0 == strcmp(#name, argv[1])) {\
      return test_ ## name();\
    }\
  } while (0)

int main(int argc, char *argv[]) {
  if (argc != 2)
    return 1;
  TEST_FEATURE(locks);
  return -1;
}