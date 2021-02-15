cmake_minimum_required(VERSION 3.0)
project(fuzzywuzzy LANGUAGES C CXX)


add_subdirectory(levenshtein)
add_subdirectory(src)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  enable_testing()
  add_subdirectory(test)
endif()
