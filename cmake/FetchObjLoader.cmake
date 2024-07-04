include(FetchContent)

FetchContent_Declare(
  objl
  GIT_REPOSITORY https://github.com/Bly7/OBJ-Loader
  GIT_TAG origin/master
  SOURCE_DIR ${CMAKE_BINARY_DIR}/deps/objl
  )

FetchContent_GetProperties(objl)
set(objl_SOURCE_DIR ${CMAKE_BINARY_DIR}/deps/objl/Source/)
message("objl_SOURCE_DIR: ${objl_SOURCE_DIR}")
