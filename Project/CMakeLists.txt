cmake_minimum_required(VERSION 3.10)

set(CC "/usr/bin/g++")
set(CMAKE_BUILD_TYPE "Debug")

project(Breaker C) #set project name

add_executable(${PROJECT_NAME} src/main.c src/parser.c src/breakers.c src/stringmod.c) #adds the executable

# target_compile_options(Breaker PUBLIC -lssl -lcrypto)
# add_compile_options(lssl lcrypto)
target_link_libraries(${PROJECT_NAME} ssl crypto pthread)

target_include_directories(${PROJECT_NAME} PUBLIC
                           "${PROJECT_SOURCE_DIR}/inc") # include headers

