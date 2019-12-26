message("Generating project for lua")

#-------------------------------------------------------------------------------
# Adding sources and headers.
#-------------------------------------------------------------------------------

file(GLOB_RECURSE lua_HEADERS "include/*.h")
file(GLOB_RECURSE lua_SOURCES "src/*.c")

#-------------------------------------------------------------------------------
# Create library for liblua.a
#-------------------------------------------------------------------------------

add_library(lua STATIC "${lua_HEADERS}" "${lua_SOURCES}")

#-------------------------------------------------------------------------------
# Setup maximum warnings on my stuff.
#-------------------------------------------------------------------------------

if(WIN32)
  target_compile_options(lua PRIVATE -W0)
endif()

