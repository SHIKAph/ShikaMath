cmake_minimum_required(VERSION 3.10)

project(ShikaMath VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(inc)

set(SOURCE_FILES
    include/Common.h
    include/Vector3.h
    src/Vector3.cpp
)

add_library(ShikaMath STATIC ${SOURCE_FILES})


if(MSVC)
    target_compile_options(ShikaMath PRIVATE /arch:AVX2)
else()
    target_compile_options(ShikaMath PRIVATE -mavx -mfma)
endif()

add_executable(TestApp tests/MathTest.cpp)

target_link_libraries(TestApp PRIVATE ShikaMath)

if(NOT MSVC)
    target_compile_options(TestApp PRIVATE -mavx -mfma)
endif()