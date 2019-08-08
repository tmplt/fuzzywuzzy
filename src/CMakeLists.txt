set(LIB_INCLUDE_DIRS ${LIB_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)
include_directories(${LIB_INCLUDE_DIRS})

add_library(levenshtein STATIC levenshtein.c)
set_property(TARGET levenshtein PROPERTY POSITION_INDEPENDENT_CODE ON)

file(GLOB_RECURSE SOURCES RELATIVE ${PROJECT_SOURCE_DIR}/src *.cpp*)
add_library(fuzzywuzzy SHARED ${SOURCES})
target_link_libraries(fuzzywuzzy levenshtein)
