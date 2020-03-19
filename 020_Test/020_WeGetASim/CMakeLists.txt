# Specify the minimum version for CMake

CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0 FATAL_ERROR)

# Project's name
if(CMAKE_GENERATOR MATCHES ".*Visual.*")
project(libWeGetASim)
else()
project(WeGetASim)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(MAINAPP_PATH ../../060_Software_Construction/MainApplication)
set(ACSIM_PATH ../ACSim)
set(ACSIM_DECLARATION_PATH ${ACSIM_PATH}/Config/Common/declarations)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -D_POSIX_C_SOURCE=200112L -Dsimulator")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Dsimulator")

include(CMake_MainApplication.txt build/MainApplication)

SET (SUBMODULES
	CommsProcessing
	Helpers
	#HelpFunctions
	#src
)

set(CANPROCESSING_PATH ${ACSIM_PATH}/Config/Common/CanProcessing)

SET (LOCAL_SIM_SRCS
    ${ACSIM_PATH}/Config/Common/EcuControl/DomainExecutionControl/DomainExecutionClasses/All_Domains/DomainExecution_All.cpp
    ${ACSIM_PATH}/Config/Common/EcuControl/HelpFunctions/VIDEO/video_help_functions.cpp
    ${CANPROCESSING_PATH}/A078_Protocol/process_message_measurement_protocol.cpp
    ${CANPROCESSING_PATH}/FUSION_Protocol/process_message_fusion_protocol.cpp
    ${CANPROCESSING_PATH}/IVECO_Vehicle_Protocol/process_message_iveco_vehicle_protocol.cpp
    ${CANPROCESSING_PATH}/process_message.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ecu_control_wegeta.cpp
)

include_directories(.)

FOREACH(SUBMODULE ${SUBMODULES})
    add_subdirectory(${SUBMODULE})
ENDFOREACH()

SET (INTERMEDIATE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/generated_src)
SET (IB_SCRIPT Scripts/buffer_gen.py)
SET (WG_SCRIPT Scripts/wrapper_gen.py)
#SET (PYGETA_SCRIPT ${PYGETA_PATH}/dwarf_parser.py)

file(MAKE_DIRECTORY ${INTERMEDIATE_DIR})

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
	SET(DECLARATIONS   declarations/combined_header.xml
                       #${ACSIM_DECLARATION_PATH}/ACSIM/wrapper_addons.xml
                       ${ACSIM_DECLARATION_PATH}/OF/OF.xml
	                   ${ACSIM_DECLARATION_PATH}/OSL_ACC/OSL_ACC.xml
	                   ${ACSIM_DECLARATION_PATH}/OSL_AEB/OSL_AEB.xml
	                   ${ACSIM_DECLARATION_PATH}/LA/LA.xml
	                   ${ACSIM_DECLARATION_PATH}/TRACK/View_Properties/Vehi.xml
	                   ${ACSIM_DECLARATION_PATH}/FD/FD.xml
	                   ${ACSIM_DECLARATION_PATH}/VIDEO/VIDEO/video_types.xml
	                   ${ACSIM_DECLARATION_PATH}/VIDEO/VIDEO/VIDEO.xml)

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

gen_wrapper(_wrapper_s)

add_custom_target(
    ecu_control_wegeta.cpp
	DEPENDS ${BUFFER_SRCS})

add_library(${PROJECT_NAME} SHARED ${LOCAL_SIM_SRCS} ${SIM_SRCS} ${BUFFER_SRCS})
target_link_libraries(${PROJECT_NAME} MainApplication)

# hack because of bad include path in MainApplication
#target_include_directories(${PROJECT_NAME}
#    PRIVATE
#        ${ACSIM_PATH}
#        ${ACSIM_PATH}/Config/Common
#        ${ACSIM_PATH}/Config/Common/CanProcessing
#       )

message(${CMAKE_C_COMPILER})
    
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} --verbose")

configure_file($ENV{COMPILER_PATH}libstdc++-6.dll ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
configure_file($ENV{COMPILER_PATH}libgcc_s_seh-1.dll ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
configure_file($ENV{COMPILER_PATH}libwinpthread-1.dll ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)

#if(NOT MSVC)
#ADD_CUSTOM_COMMAND(
#	TARGET ${PROJECT_NAME}
#	POST_BUILD
#	COMMAND ${CMAKE_COMMAND} -E copy_if_different $ENV{COMPILER_PATH}libstdc++-6.dll ${CMAKE_CURRENT_BINARY_DIR}/libstdc++-6.dll
#	WORKING_DIRECTORY ${PYGETA_PATH}
#	COMMENT "Copy DLL to output dir ${CMAKE_CURRENT_BINARY_DIR}"
#)
#endif(NOT MSVC)









































