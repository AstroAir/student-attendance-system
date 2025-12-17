set(_msys2_root "")

set(_msys2_root_candidates "")

if(DEFINED ENV{MSYS2_ROOT} AND NOT "$ENV{MSYS2_ROOT}" STREQUAL "")
  list(APPEND _msys2_root_candidates "$ENV{MSYS2_ROOT}")
endif()

if(DEFINED ENV{MSYS2_DIR} AND NOT "$ENV{MSYS2_DIR}" STREQUAL "")
  list(APPEND _msys2_root_candidates "$ENV{MSYS2_DIR}")
endif()

if(DEFINED ENV{MSYS2} AND NOT "$ENV{MSYS2}" STREQUAL "")
  list(APPEND _msys2_root_candidates "$ENV{MSYS2}")
endif()

foreach(_candidate IN LISTS _msys2_root_candidates)
  string(STRIP "${_candidate}" _candidate)
  if(_candidate STREQUAL "")
    continue()
  endif()

  string(REPLACE "\\" "/" _candidate "${_candidate}")
  string(REGEX REPLACE "/+$" "" _candidate "${_candidate}")

  if(_candidate MATCHES "^/([a-zA-Z])/(.*)")
    string(TOUPPER "${CMAKE_MATCH_1}" _drive)
    set(_candidate "${_drive}:/${CMAKE_MATCH_2}")
  endif()

  if(EXISTS "${_candidate}/mingw64/bin/g++.exe")
    set(_msys2_root "${_candidate}")
    break()
  endif()
endforeach()

if(_msys2_root STREQUAL "")
  if(DEFINED ENV{PATH} AND NOT "$ENV{PATH}" STREQUAL "")
    set(_path_list "$ENV{PATH}")
    string(REPLACE "\\" "/" _path_list "${_path_list}")

    foreach(_entry IN LISTS _path_list)
      string(STRIP "${_entry}" _entry)
      if(_entry STREQUAL "")
        continue()
      endif()

      string(REGEX REPLACE "/+$" "" _entry "${_entry}")

      if(_entry MATCHES "^/([a-zA-Z])/(.*)")
        string(TOUPPER "${CMAKE_MATCH_1}" _drive)
        set(_entry "${_drive}:/${CMAKE_MATCH_2}")
      endif()

      if(_entry MATCHES "/mingw64/bin$")
        if(EXISTS "${_entry}/g++.exe")
          string(REGEX REPLACE "/mingw64/bin$" "" _msys2_root "${_entry}")
          break()
        endif()
      endif()
    endforeach()
  endif()
endif()

if(_msys2_root STREQUAL "")
  foreach(_candidate "C:/msys64" "C:/msys2" "D:/msys64")
    if(EXISTS "${_candidate}/mingw64/bin/g++.exe")
      set(_msys2_root "${_candidate}")
      break()
    endif()
  endforeach()
endif()

if(_msys2_root STREQUAL "")
  message(FATAL_ERROR "MSYS2 root not found. Set MSYS2_ROOT (or MSYS2_DIR) to your MSYS2 install path, e.g. C:/msys64")
endif()

set(_mingw64_bin "${_msys2_root}/mingw64/bin")
set(_mingw64_prefix "${_msys2_root}/mingw64")

message(STATUS "MSYS2 root: ${_msys2_root}")
message(STATUS "MinGW64 bin: ${_mingw64_bin}")
message(STATUS "MinGW64 prefix: ${_mingw64_prefix}")

if(NOT EXISTS "${_mingw64_bin}/g++.exe")
  message(FATAL_ERROR "MinGW64 toolchain not found at ${_mingw64_bin}. Expected g++.exe to exist.")
endif()

if(DEFINED CMAKE_FIND_ROOT_PATH)
  list(PREPEND CMAKE_FIND_ROOT_PATH "${_mingw64_prefix}")
else()
  set(CMAKE_FIND_ROOT_PATH "${_mingw64_prefix}")
endif()

if(DEFINED CMAKE_PREFIX_PATH)
  list(PREPEND CMAKE_PREFIX_PATH "${_mingw64_prefix}")
else()
  set(CMAKE_PREFIX_PATH "${_mingw64_prefix}")
endif()

set(ENV{PKG_CONFIG_PATH} "${_mingw64_prefix}/lib/pkgconfig;${_mingw64_prefix}/share/pkgconfig;$ENV{PKG_CONFIG_PATH}")

set(CMAKE_C_COMPILER "${_mingw64_bin}/gcc.exe" CACHE FILEPATH "" FORCE)
set(CMAKE_CXX_COMPILER "${_mingw64_bin}/g++.exe" CACHE FILEPATH "" FORCE)
set(CMAKE_MAKE_PROGRAM "${_mingw64_bin}/mingw32-make.exe" CACHE FILEPATH "" FORCE)
set(CMAKE_SH "CMAKE_SH-NOTFOUND" CACHE FILEPATH "" FORCE)
