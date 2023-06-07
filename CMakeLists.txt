cmake_minimum_required(VERSION 3.25)
project(mini_redis C)

set(CMAKE_C_STANDARD 11)

add_executable(mini_redis main.c data-structure/sds.h data-structure/sds.c util/log.h memory/sdsalloc.h memory/zmalloc.h memory/atomicvar.h memory/config.h memory/zmalloc.c)
