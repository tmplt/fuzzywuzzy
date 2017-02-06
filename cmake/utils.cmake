# make_library : Builds a library target {{{

function(make_library target_name)
  set(zero_value_args SHARED STATIC)
  set(one_value_args PACKAGE HEADER_INSTALL_DIR)
  set(multi_value_args SOURCES HEADERS INCLUDE_DIRS PKG_DEPENDS CMAKE_DEPENDS TARGET_DEPENDS RAW_DEPENDS)

  cmake_parse_arguments(LIB
    "${zero_value_args}" "${one_value_args}"
    "${multi_value_args}" ${ARGN})

  # make the header paths absolute
  foreach(HEADER ${LIB_HEADERS})
    set(LIB_HEADERS_ABS ${LIB_HEADERS_ABS} ${PROJECT_SOURCE_DIR}/include/${HEADER})
  endforeach()

  # add defined INCLUDE_DIRS
  foreach(DIR ${LIB_INCLUDE_DIRS})
    string(TOUPPER ${DIR} DIR)
    include_directories(${DIR})
    include_directories(${${DIR}_INCLUDE_DIRS})
  endforeach()

  # add INCLUDE_DIRS for all external dependencies
  foreach(DEP ${LIB_TARGET_DEPENDS} ${LIB_PKG_DEPENDS} ${LIB_CMAKE_DEPENDS})
    string(TOUPPER ${DEP} DEP)
    include_directories(${${DEP}_INCLUDE_DIRS} ${${DEP}_INCLUDEDIRS})
  endforeach()

  if(LIB_SHARED)
    list(APPEND library_targets ${target_name}_shared)
  endif()
  if(LIB_STATIC)
    list(APPEND library_targets ${target_name}_static)
  endif()

  foreach(library_target_name ${library_targets})
    message(STATUS "${library_target_name}")
    add_library(${library_target_name} ${LIB_HEADERS_ABS} ${LIB_SOURCES})

    # link libraries from pkg-config imports
    foreach(DEP ${LIB_PKG_DEPENDS})
      string(TOUPPER ${DEP} DEP)
      target_link_libraries(${library_target_name} ${${DEP}_LDFLAGS})
    endforeach()

    # link libraries from cmake imports
    foreach(DEP ${LIB_CMAKE_DEPENDS})
      string(TOUPPER ${DEP} DEP)
      target_link_libraries(${library_target_name} ${${DEP}_LIB}
                                                   ${${DEP}_LIBRARY}
                                                   ${${DEP}_LIBRARIES})
    endforeach()

    # link libraries that are build as part of this project
    foreach(DEP ${LIB_TARGET_DEPENDS})
      string(TOUPPER ${DEP} DEP)
      if(LIB_BUILD_SHARED)
        target_link_libraries(${library_target_name} ${DEP}_shared)
      endif()
      if(LIB_BUILD_STATIC)
        target_link_libraries(${library_target_name} ${DEP}_static)
      endif()
    endforeach()

    if(${LIB_RAW_DEPENDS})
      if(LIB_BUILD_STATIC)
        target_link_libraries(${library_target_name} ${LIB_RAW_DEPENDS})
      endif()
    endif()

    # set the output file basename
    set_target_properties(${library_target_name} PROPERTIES OUTPUT_NAME ${target_name})

    # install headers
    install(FILES ${LIBRARY_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${LIB_HEADERS_ABS})

    # install targets
    install(TARGETS ${LIBRARY_TARGETS}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT library)
  endforeach()
endfunction()

# }}}
