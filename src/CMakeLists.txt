# CMakeLists.txt
# CMakeLists specific to unit test target

###########################

set(SOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set (CMAKE_CXX_FLAGS "-std=c++14")

# compiler includes
include_directories(
	${SOURCE_ROOT}
	${SOURCE_ROOT}/example1
    ${SOURCE_ROOT}/example2
    ${SOURCE_ROOT}/example3
)

# source files
FILE( GLOB_RECURSE  SRC_FILES
    *.cpp
    *.h
    ${SOURCE_ROOT}/example1/*.h
    ${SOURCE_ROOT}/example2/*.h
    ${SOURCE_ROOT}/example3/*.h
)

add_executable( threading_tests ${SOURCE_ROOT} ${SRC_FILES} )
