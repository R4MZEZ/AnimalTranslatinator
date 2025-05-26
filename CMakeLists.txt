cmake_minimum_required(VERSION 3.10)

project(AnimalTranslatinator)
set(SRCDIR src)
set(INCDIR include)
file(GLOB SRCS "${SRCDIR}/*.cpp")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

add_executable(${PROJECT_NAME} ${SRCS})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCDIR})
target_compile_options(${PROJECT_NAME} PRIVATE -fpermissive)
