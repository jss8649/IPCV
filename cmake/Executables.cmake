# Function for adding an executable to the build process.
#
# Arguments:
#   EXE_NAME Name for the executable
#   SOURCES Named parameter for the source files that go into the executable
function(imgs_add_executable EXE_NAME)
  set(options)
  set(oneValueArgs)
  set(multiValueArgs SOURCES)
  cmake_parse_arguments(imgs_add_executable "${options}" "${oneValueArgs}"
                                            "${multiValueArgs}" ${ARGN})

  if (NOT DEFINED EXE_NAME)
    message(FATAL_ERROR "imgs_add_executable(): EXE_NAME not defined")
  endif()

  add_executable(${EXE_NAME} ${imgs_add_executable_SOURCES})

  target_include_directories(${EXE_NAME}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
      $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
  )
endfunction()
