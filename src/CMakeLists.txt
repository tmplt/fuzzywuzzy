file(GLOB_RECURSE SOURCES RELATIVE ${PROJECT_SOURCE_DIR}/src *.c[p]*)

set(LIB_INCLUDE_DIRS ${LIB_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)

#foreach(DIR ${LIB_INCLUDE_DIRS})
#    string(TOUPPER ${DIR} DIR)
#    include_directories(${DIR})
#    include_directories(${${DIR}_INCLUDE_DIRS})
#endforeach()

include_directories(${LIB_INCLUDE_DIRS})

add_library(fuzzywuzzy ${SOURCES})

#make_library(fuzzywuzzy ${PROJECT_NAME}
#    SOURCES
#      ${SOURCES}
#    INCLUDE_DIRS
#      ${LIB_INCLUDE_DIRS}
#    RAW_DEPENDS
#      ${LIB_LIBRARIES})
