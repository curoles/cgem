SET(CGem_COMMON_C_FLAGS "-Werror -Wall")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CGEM_COMMON_C_FLAGS}")

add_subdirectory (stack_direction)

