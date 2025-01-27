cmake_minimum_required(VERSION 3.28)
project(theProgect C)


set(CMAKE_C_STANDARD 90)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14")

add_executable(theProgect main.c
        dataBase.h
        constantVar.h
        Utility.h
        preAssembly.h
        stage1.h
        dataBase.c
        preAssembly.c
        stage1.c
        Utility.c
        stage2.h
        stage2.c)
