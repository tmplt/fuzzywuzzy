set(LIB_INCLUDE_DIRS ${LIB_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/c-src)
include_directories(${LIB_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES RELATIVE ${PROJECT_SOURCE_DIR}/test *.c[p]*)
add_executable(main ${SOURCES})
target_link_libraries(main fuzzywuzzy)