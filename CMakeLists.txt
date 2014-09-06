cmake_minimum_required (VERSION 2.6)
project (cluster_process)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./bin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
        )
endif(DOXYGEN_FOUND)

add_executable(cluster_process ./src/main.cpp)
