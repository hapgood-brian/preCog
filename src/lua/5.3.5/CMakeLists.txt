message("Generating project for lua")

#-------------------------------------------------------------------------------
# Adding sources and headers.
#-------------------------------------------------------------------------------

file(GLOB_RECURSE lua_SOURCES "src/*.c")

#-------------------------------------------------------------------------------
# Create library for liblua.a
#-------------------------------------------------------------------------------

add_library(lua STATIC "${lua_SOURCES}")

#-------------------------------------------------------------------------------
# Tell Lua to use floats instead of doubles.
#-------------------------------------------------------------------------------

if(WIN32)
  add_compile_definitions(lua
    LUA_FLOAT_TYPE=1)
endif()

#-------------------------------------------------------------------------------
# Force include eon.h
#-------------------------------------------------------------------------------

# Setup all the include directories per platform.
if(WIN32)
  target_include_directories(lua PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/lua",
    "${PROJECT_BINARY_DIR}")
endif()

#-------------------------------------------------------------------------------
# Setup maximum warnings on my stuff.
#-------------------------------------------------------------------------------

if(WIN32)
  target_compile_options(lua PRIVATE -W3)
endif()
