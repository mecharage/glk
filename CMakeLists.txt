cmake_minimum_required(VERSION 2.8)
project(glk)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -Wfatal-errors -pedantic -fno-rtti -DGLM_FORCE_RADIANS")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG -O0 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -O3")

include_directories(AFTER include)

file(GLOB_RECURSE H_FILES include/*.h)
file(GLOB_RECURSE CPP_FILES src/*.cpp)
file(GLOB_RECURSE TPP_FILES src/*.tpp)
set(SOURCE_FILES ${H_FILES} ${CPP_FILES} ${TPP_FILES})

add_library(glk STATIC ${SOURCE_FILES})

install(
	TARGETS glk
	ARCHIVE DESTINATION lib
)

install(
	DIRECTORY include/glk
	DESTINATION include
)
