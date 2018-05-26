# ------------------------- Begin Generic CMake Variable Logging ------------------
# https://gitlab.kitware.com/cmake/community/wikis/doc/cmake/useful_variables/Logging-Useful-Variables

# the compiler used for C files
MESSAGE( STATUS "CMAKE_C_COMPILER:     " ${CMAKE_C_COMPILER} )

# the compiler used for C++ files
MESSAGE( STATUS "CMAKE_CXX_COMPILER:   " ${CMAKE_CXX_COMPILER} )

MESSAGE( STATUS "CMAKE_INSTALL_PREFIX: " ${CMAKE_INSTALL_PREFIX} )

