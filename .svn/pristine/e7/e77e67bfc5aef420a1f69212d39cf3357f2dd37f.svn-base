# - Try to find GTKEXTGL 1.0
#http://www.cmake.org/Wiki/CMake:How_To_Find_Libraries
#http://zi.fi/cmake/Modules/
include(LibFindMacros)

# Use pkg-config to get hints about paths
#pkg-config --msvc-syntax -cflags gtkglext-1.0
libfind_pkg_check_modules(GTKEXTGL_PKGCONF gtkglext-1.0)

# Main include dir
find_path(GTKEXTGL_INCLUDE_DIR
  NAMES gtk/gtkgl.h gdk/gdkgl.h
  PATHS ${GTKEXTGL_PKGCONF_INCLUDE_DIRS}
)
find_path(GTKEXTGL_LIB_INCLUDE_DIR
  NAMES gdkglext-config.h
  PATHS ${GTKEXTGL_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
find_library(GTKEXTGL_LIBRARY
  NAMES COPYING
  PATHS ${GTKEXTGL_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(GTKEXTGL_PROCESS_INCLUDES GTKEXTGL_INCLUDE_DIR GTKEXTGL_LIB_INCLUDE_DIR)
set(GTKEXTGL_PROCESS_LIBS GTKEXTGL_LIBRARY)
libfind_process(GTKEXTGL)

MESSAGE(${GTKEXTGL_PKGCONF_LIBRARY_DIRS})
