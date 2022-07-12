#include <cstdlib>
#include <cstdio>

#include "test.h"

bool
operator_precedence_test();

int
main()
{
  size_t tests_failed = 0;

  RUN_TEST(operator_precedence_test);

  return tests_failed != 0;
}

