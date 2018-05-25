# The ExternalProject_Add() function creates a custom target to drive download,
# update/patch, configure, build, install and test steps of an external project.
# https://cmake.org/cmake/help/latest/module/ExternalProject.html

# Set default ExternalProject root directory
SET_DIRECTORY_PROPERTIES(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/external_projects)

#set(EXTERNAL_PKGS_DIR ${CMAKE_BINARY_DIR}/thirdparty)

# On Ubuntu it is easy to install and build GTest
# sudo apt-get install libgtest-dev
# cd /usr/src/gtest
# sudo mkdir build
# cd build
# sudo cmake ..
# sudo make
# sudo ln -s /usr/src/gtest/build/libgtest.a /usr/lib/libgtest.a
# sudo ln -s /usr/src/gtest/build/libgtest_main.a /usr/lib/libgtest_main.a
# Now find_package(GTest REQUIRED) finds headers and libraries.

include(ExternalProject)
ExternalProject_Add(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    # # Force separate output paths for debug and release builds to allow easy
    # # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
    # CMAKE_ARGS -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
    #            -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
    #            -Dgtest_force_shared_crt=ON
    # Disable install step
    INSTALL_COMMAND ""
    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
)
# Specify include dir
#ExternalProject_Get_Property(googletest source_dir)
#set(GTEST_INCLUDE_DIR ${source_dir}/include)

# Library
#ExternalProject_Get_Property(googletest binary_dir)
#set(GTEST_LIBRARY_PATH ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}gtest.a)
#set(GTEST_LIBRARY gtest)
#add_library(${GTEST_LIBRARY} UNKNOWN IMPORTED)
#set_property(TARGET ${GTEST_LIBRARY} PROPERTY IMPORTED_LOCATION
#                ${GTEST_LIBRARY_PATH} )
#add_dependencies(${GTEST_LIBRARY} googletest

#set(PKG_GLS_DIR ${EXTERNAL_PKGS_DIR}/gls)
#set(PKG_GLS_INCLUDE_DIR ${EXTERNAL_PKG_GLS_DIR}/include)
#
#ExternalProject_Add(gls
#    PREFIX ${PKG_GLS_DIR}
#    GIT_REPOSITORY "https://github.com/Microsoft/GSL.git"
#    #Below change flags to W/A issue: https://github.com/Microsoft/GSL/issues/553
#    CMAKE_ARGS -DCMAKE_CXX_FLAGS="-Wno-error=sign-conversion"
#    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
#    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
#)

