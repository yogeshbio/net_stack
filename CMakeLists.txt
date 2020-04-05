
cmake_minimum_required(VERSION 3.10.0)

project (test.exe)


add_executable(${PROJECT_NAME} graph.c
                               topologies.c
                               net.c
                               test_topology.c
                               utils.c
                               )

target_compile_options(${PROJECT_NAME}
    PUBLIC
        #-Wall
        #-Werror
        -g
    )

target_include_directories(${PROJECT_NAME} 
                           PUBLIC 
                           {CMAKE_CURRENT_LIST_DIR}
                           {CMAKE_CURRENT_LIST_DIR}/intrusive_list)
