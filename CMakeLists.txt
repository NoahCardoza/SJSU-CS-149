cmake_minimum_required(VERSION 3.26)
project(SJSU_CS_149 C)

set(CMAKE_C_STANDARD 11)

add_executable(SJSU_CS_149
        src/main.c
        src/manager.c
        src/manager.h
        src/program.c
        src/cpu.c
        src/pcb.c
        src/queues.c
        src/config.h
        src/queues.h
        "src/scheduler.c"
        "src/scheduler.h")
