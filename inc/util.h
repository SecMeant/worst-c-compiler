#pragma once

#include <cstdio>
#include <cstdlib>

#define likely(x) __builtin_expect(x, 1)
#define unlikely(x) __builtin_expect(x, 0)

[[noreturn]] inline void
panic(const char* msg, int exit_code = 1)
{
  puts(msg);
  exit(exit_code);
}


