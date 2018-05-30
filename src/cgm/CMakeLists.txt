SET(CGEM_COMMON_CXX_FLAGS "-W -Werror -Wall")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CGEM_COMMON_CXX_FLAGS}")

install(FILES stack_direction.h
    DESTINATION include/cgm
)

#add_subdirectory (stack_direction)

