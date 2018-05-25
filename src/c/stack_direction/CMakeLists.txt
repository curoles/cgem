# Create a library called "StackDirection".
add_library (StackDirection
    stack_direction.c
)

install(FILES stack_direction.h
    DESTINATION include/cgem
)

## Make sure the compiler can find include files for our Hello library
## when other libraries or executables link to Hello
#target_include_directories (Hello PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#
#ExternalProject_Get_Property(gls install_dir)
#include_directories(${install_dir}/include)
#
#add_dependencies(Hello gls)
##target_link_libraries(Hello ${install_dir}/lib/alib.lib)
