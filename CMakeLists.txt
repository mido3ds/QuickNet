cmake_minimum_required (VERSION 2.8.11)
project (QuickNet)

#
# Set include paths
#
include_directories (src/)

#
# Add files
#
file (GLOB_RECURSE PROJECT_HEADERS src/*.hpp
                                   src/*.h)
file (GLOB_RECURSE PROJECT_SOURCES examples/*.cpp
                                   examples/*.ccxx
                                   examples/*.cc
                                   examples/*.c
                                   src/*.cpp
                                   src/*.cxx
                                   src/*.cc
                                   src/*.c)
file (GLOB         PROJECT_CONFIGS CMakeLists.txt)

#
# Organizing files
#
source_group ("headers" FILES ${PROJECT_HEADERS})
source_group ("sources" FILES ${PROJECT_SOURCES})

#
# Set executable and target link libraries
#
add_definitions (-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable (${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                                ${PROJECT_CONFIGS} ${VENDORS_SOURCES})
target_link_libraries (${PROJECT_NAME})
set_target_properties (${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

#
# create makefile
#
file (WRITE Makefile "# auto-generated makefile

.PHONY: all cmake build run

all: cmake build run

cmake:
\tcmake -G\"Visual Studio 14 2015 Win64\" -H. -B\"${CMAKE_BINARY_DIR}\" || echo \">>>> cmake initialization failed! <<<<<\"

build:
\tcmake --build \"${CMAKE_BINARY_DIR}\" || echo \">>>> build failed! <<<<<\"

run:
\t\"${CMAKE_BINARY_DIR}/${PROJECT_NAME}/Debug/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}\"

clean:
\tcmake --build \"${CMAKE_BINARY_DIR}\" --target clean || echo \">>>> clean failed! <<<<<\"

cleanAll:
\t	del Makefile && rd /s /q build && rd /s /q CMakeFiles || rm -rf build CMakeFiles Makefile
")