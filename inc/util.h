#pragma once

#include <utility>
#include <cstdio>
#include <cstdlib>

#define likely(x) __builtin_expect(x, 1)
#define unlikely(x) __builtin_expect(x, 0)

#define CONCAT_(x,y) x##y
#define CONCAT(x,y) CONCAT_(x,y)
#define UNIQUE_FILE_VAR CONCAT(VAR, __LINE__)

[[noreturn]] inline void
panic(const char* msg, int exit_code = 1)
{
  puts(msg);
  exit(exit_code);
}

#define OnBlockExit(C) OnBlockExitType UNIQUE_FILE_VAR(C)

template <typename CallableType>
class OnBlockExitType
{
public:
	OnBlockExitType(CallableType &&c)
	:
	action(std::forward<CallableType>(c))
	{}

	~OnBlockExitType()
	{
	this->action();
	}

	private:
		CallableType action;
};

