
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TEST_FUNC(name) \
  int test_ ## name(void)


TEST_FUNC(locks);
TEST_FUNC(ordering);


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
  TEST_FEATURE(ordering);
  return -1;
}