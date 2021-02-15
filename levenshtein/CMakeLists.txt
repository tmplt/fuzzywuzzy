add_library(levenshtein STATIC levenshtein.c)
target_include_directories(levenshtein PUBLIC ./)
set_target_properties(levenshtein PROPERTIES POSITION_INDEPENDENT_CODE ON)

if (CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
  set(PEDANTIC_COMPILE_FLAGS -Wall -Wextra -Wshadow -pedantic)
  set_target_properties(levenshtein PROPERTIES COMPILE_OPTIONS "${PEDANTIC_COMPILE_FLAGS}")
endif()
