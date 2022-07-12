#include <cstdlib>

#include <fmt/format.h>

#define QUOTE_(x) #x
#define QUOTE(x) QUOTE_(x)

#define RUN_TEST(testname)                                                     \
  if (!testname()) {                                                           \
    fprintf(stderr, QUOTE(testname) " FAILED\n");                              \
    ++tests_failed;                                                            \
  }

#define TEST_ASSERT(x)                                                         \
  if (unlikely(!(x))) {                                                        \
    fmt::print(stderr, "Assertion failed: {}\n", QUOTE(x));                    \
    return false;                                                              \
  }
