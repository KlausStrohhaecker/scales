cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME find-scales)

project(${PROJECT_NAME})

set(MYFLAGS "-Wall -Wextra -Werror -O3")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${MYFLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MYFLAGS}")

add_executable(${PROJECT_NAME}
## sources
    src/main.cpp

## includes

)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    src
)
