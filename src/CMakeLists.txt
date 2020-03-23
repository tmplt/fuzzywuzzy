
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)

add_library(fuzzywuzzy STATIC ${SOURCES})
target_link_libraries(fuzzywuzzy levenshtein)
target_include_directories(fuzzywuzzy PRIVATE ./include/)
target_include_directories(fuzzywuzzy PUBLIC ../public/)


if (CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
  set(PEDANTIC_COMPILE_FLAGS -Wall -Wextra -Wshadow -pedantic)
  set_target_properties(fuzzywuzzy PROPERTIES COMPILE_OPTIONS "${PEDANTIC_COMPILE_FLAGS}")
endif()
