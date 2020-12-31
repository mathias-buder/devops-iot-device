# Specify the minimum version for CMake
CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0 FATAL_ERROR)

# Flag to control the generation of data structure _wrapper_s
# based on .xml declarations
SET(GENERATE_WRAPPER FALSE)

# Variable to be used inside all individual CMakeList.txt files to switch between
# SeWelaSim and uC build settings
SET(SEWELASIM 1)

# Project's name
if(CMAKE_GENERATOR MATCHES ".*Visual.*")
    project(libSeWelaSim)
else()
    project(SeWelaSim)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-unknown-pragmas")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()



set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(MAINAPP_PATH ../../010_Construction/MainApplication)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -D_POSIX_C_SOURCE=200112L -Dsimulator")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Dsimulator")

include(CMake_MainApplication.txt build/MainApplication)

SET (LOCAL_SIM_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/sewela_sim.cpp)

include_directories(.)

FOREACH(SUBMODULE ${SUBMODULES})
    add_subdirectory(${SUBMODULE})
ENDFOREACH()



function(gen_buffer TYPES_H CTYPE BUFFER_SIZE INTERFACE_DIMENSION)
    SET(OUT_C ${INTERMEDIATE_DIR}/${CTYPE}_buffer.c)
    SET(OUT_H ${INTERMEDIATE_DIR}/${CTYPE}_buffer.h)

    add_custom_command(
        DEPENDS ${IB_SCRIPT} ${MAINAPP_PATH}/${TYPES_H}
        COMMAND python.exe ${IB_SCRIPT} --type_h ${TYPES_H} --target_h ${OUT_H} --target_cpp ${OUT_C} --mainapp_path ${MAINAPP_PATH} --ctype_dimension ${INTERFACE_DIMENSION} ${BUFFER_SIZE} ${CTYPE}
        OUTPUT ${OUT_C} ${OUT_H}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating buffer for ${CTYPE}"
    )

    set_source_files_properties(${OUT_C} PROPERTIES GENERATED TRUE)
    set_source_files_properties(${OUT_H} PROPERTIES GENERATED TRUE)

    SET(BUFFER_SRCS ${BUFFER_SRCS} ${OUT_C} PARENT_SCOPE)
endfunction()

function(gen_wrapper WRAPPERSTRUCT)
    SET(OUT_C ${INTERMEDIATE_DIR}/wrapper.c)
    SET(OUT_H ${INTERMEDIATE_DIR}/wrapper.h)
    SET(DECLARATIONS  declarations/combined_header.xml
                      declarations/SENSE/SENSE.xml )

    add_custom_command(
        DEPENDS ${WG_SCRIPT} ${DECLARATIONS}
        COMMAND python.exe ${WG_SCRIPT} -wout ${OUT_C} -hout ${OUT_H} -ws ${WRAPPERSTRUCT} -x ${DECLARATIONS}
        OUTPUT ${OUT_C} ${OUT_H}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating wrapper for ${WRAPPERSTRUCT}"
    )

    set_source_files_properties(${OUT_C} PROPERTIES GENERATED TRUE)
    set_source_files_properties(${OUT_H} PROPERTIES GENERATED TRUE)

    SET(BUFFER_SRCS ${BUFFER_SRCS} ${OUT_C} PARENT_SCOPE)
endfunction()

# Generate wrapper struct _wrapper_s based on .xml declarations
if(GENERATE_WRAPPER)
    SET (INTERMEDIATE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/generated_src)
    SET (IB_SCRIPT Scripts/buffer_gen.py)
    SET (WG_SCRIPT Scripts/wrapper_gen.py)
    file(MAKE_DIRECTORY ${INTERMEDIATE_DIR})
    gen_wrapper(_wrapper_s)
endif()


add_custom_target( sewela_sim.cpp
                   DEPENDS ${BUFFER_SRCS} )

add_library(${PROJECT_NAME} SHARED ${LOCAL_SIM_SRCS} ${SIM_SRCS} ${BUFFER_SRCS})
target_link_libraries(${PROJECT_NAME} MainApplication)

message(${CMAKE_C_COMPILER})

SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} --verbose")

configure_file($ENV{COMPILER_PATH}libstdc++-6.dll ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
configure_file($ENV{COMPILER_PATH}libgcc_s_seh-1.dll ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
configure_file($ENV{COMPILER_PATH}libwinpthread-1.dll ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
