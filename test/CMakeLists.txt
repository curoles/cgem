# https://cmake.org/cmake/help/v3.10/module/GoogleTest.html

MESSAGE( STATUS "GTEST_FOUND:          " ${GTEST_FOUND} )
MESSAGE( STATUS "GTEST_INCLUDE_DIRS:   " ${GTEST_INCLUDE_DIRS} )
MESSAGE( STATUS "GTEST_LIBRARIES:      " ${GTEST_LIBRARIES} )
MESSAGE( STATUS "GTEST_MAIN_LIBRARIES: " ${GTEST_MAIN_LIBRARIES} )

include(GoogleTest)

add_subdirectory (stack_direction)
add_subdirectory (stack_array)
add_subdirectory (ct_math)
add_subdirectory (numrange)
add_subdirectory (cpu_cache)
