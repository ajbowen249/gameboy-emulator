#ifndef __TestMain_h__
#define __TestMain_h__

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "CPU.h"
#include "SimpleMemory.h"

#include "doctest.h"

#define WITH_CPU_AND_SIMPLE_MEMORY() \
    auto simpleMemory = std::make_shared<SimpleMemory>(); \
    CPU testCPU(simpleMemory); \

#endif // __TestMain_h__
