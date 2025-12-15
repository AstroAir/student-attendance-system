function(add_doxygen_target)
  find_package(Doxygen QUIET)
  if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen not found")
  endif()

  set(DOXYFILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile")
  if(NOT EXISTS "${DOXYFILE_PATH}")
    message(FATAL_ERROR "Doxyfile not found at: ${DOXYFILE_PATH}")
  endif()

  add_custom_target(doxygen
    COMMAND "${DOXYGEN_EXECUTABLE}" "${DOXYFILE_PATH}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/docs"
    VERBATIM
  )
endfunction()
