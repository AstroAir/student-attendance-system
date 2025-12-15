function(add_mkdocs_targets)
  set(options)
  set(oneValueArgs CONFIG_FILE SITE_DIR)
  set(multiValueArgs)
  cmake_parse_arguments(MKDOCS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(NOT MKDOCS_CONFIG_FILE)
    message(FATAL_ERROR "add_mkdocs_targets requires CONFIG_FILE")
  endif()

  if(NOT MKDOCS_SITE_DIR)
    set(MKDOCS_SITE_DIR "${CMAKE_BINARY_DIR}/mkdocs")
  endif()

  find_program(UV_EXECUTABLE uv)
  find_program(MKDOCS_EXECUTABLE mkdocs)

  set(_mkdocs_command "")
  set(_mkdocs_args "")

  if(UV_EXECUTABLE)
    set(_mkdocs_command "${UV_EXECUTABLE}")
    set(_mkdocs_args run mkdocs)
  elseif(MKDOCS_EXECUTABLE)
    set(_mkdocs_command "${MKDOCS_EXECUTABLE}")
  else()
    find_package(Python3 COMPONENTS Interpreter QUIET)
    if(Python3_Interpreter_FOUND)
      set(_mkdocs_command "${Python3_EXECUTABLE}")
      set(_mkdocs_args -m mkdocs)
    endif()
  endif()

  if(NOT _mkdocs_command)
    add_custom_target(mkdocs-build
      COMMAND ${CMAKE_COMMAND} -E echo "mkdocs not found. Install via: pip install mkdocs"
      COMMAND ${CMAKE_COMMAND} -E false
      VERBATIM
    )

    add_custom_target(mkdocs-serve
      COMMAND ${CMAKE_COMMAND} -E echo "mkdocs not found. Install via: pip install mkdocs"
      COMMAND ${CMAKE_COMMAND} -E false
      VERBATIM
    )
    return()
  endif()

  add_custom_target(mkdocs-build
    COMMAND "${_mkdocs_command}" ${_mkdocs_args} build -f "${MKDOCS_CONFIG_FILE}" -d "${MKDOCS_SITE_DIR}"
    VERBATIM
  )

  add_custom_target(mkdocs-serve
    COMMAND "${_mkdocs_command}" ${_mkdocs_args} serve -f "${MKDOCS_CONFIG_FILE}"
    VERBATIM
  )
endfunction()
