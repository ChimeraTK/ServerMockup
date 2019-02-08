PROJECT(server-mockup)
cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

set(${PROJECT_NAME}_MAJOR_VERSION 00)
set(${PROJECT_NAME}_MINOR_VERSION 01)
set(${PROJECT_NAME}_PATCH_VERSION 00)
include(cmake/set_version_numbers.cmake)

# Moderate version of the compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -Wextra -ansi -Wuninitialized -O3 -fomit-frame-pointer")

# add constants for HOST and USER
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSER=\\\"`whoami`\\\" -DHOST=\\\"`hostname`\\\"")

#use -DCMAKE_BUILD_TYPE=Debug in your cmake command to turn on the coverage option
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -DDEBUG")

# linker flags
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--export-dynamic -Wl,--no-as-needed -Wl,--enable-new-dtags")

include(cmake/add_dependency.cmake)
add_dependency(ChimeraTK-ApplicationCore 01.01 REQUIRED)

IF(ADAPTER STREQUAL "OPCUA")
  message("Building against the OPC UA ControlSystemAdater")
  add_dependency(ChimeraTK-ControlSystemAdapter-OPCUAAdapter REQUIRED)
ELSEIF(ADAPTER STREQUAL "DOOCS")
  message("Building against the DOOCS ControlSystemAdater")
  add_dependency(ChimeraTK-ControlSystemAdapter-DoocsAdapter REQUIRED)
ELSE()
  message(FATAL_ERROR "Please select your ControlSystemAdapter to use by passing to the cmake command line:\n"
                      "   -DADAPTER=DOOCS to use DOOCS\n"
                      "   -DADAPTER=OPCUA to use OPC-UA")
ENDIF()

FIND_PACKAGE(Boost COMPONENTS unit_test_framework thread)
ENABLE_TESTING()


FIND_PACKAGE(PkgConfig REQUIRED)
set(LIBXML++_VERSION "libxml++-2.6")
PKG_CHECK_MODULES(LibXML++ REQUIRED ${LIBXML++_VERSION})
PKG_CHECK_MODULES(glib REQUIRED glib-2.0)
include_directories(SYSTEM "${LibXML++_INCLUDEDIR}/${LIBXML++_VERSION}")
include_directories(${LibXML++_INCLUDE_DIRS})

# set include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include/)

# create lists with source files
aux_source_directory(${CMAKE_SOURCE_DIR}/src library_sources)
list(REMOVE_ITEM library_sources ${CMAKE_SOURCE_DIR}/src/applicationInstance.cc)
set(server_sources ${CMAKE_SOURCE_DIR}/src/applicationInstance.cc)

# configure version.h
configure_file(include/version.h.in ${PROJECT_BINARY_DIR}/version/version.h)
include_directories(${PROJECT_BINARY_DIR}/version)

# keep the rpath in the installed executable and library to correctly find the dependencies
if("${CMAKE_INSTALL_PREFIX}" STREQUAL "/export/doocs/server")
  SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}")
else()
  SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif()
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# main server executable
add_executable(${PROJECT_NAME} ${server_sources} ${library_sources})
target_link_libraries(${PROJECT_NAME} ${ChimeraTK-ApplicationCore_LIBRARIES}
                                      ${ChimeraTK-ControlSystemAdapter-DoocsAdapter_LIBRARIES} 
                                      ${ChimeraTK-ControlSystemAdapter-OPCUAAdapter_LIBRARIES} 
                                      ${LibXML++_LIBRARIES})

# Create the executable for automated unit testing.
# add the test executable with all test source files
add_executable(testCounter ${CMAKE_SOURCE_DIR}/test/testCounter.C ${library_sources})
target_link_libraries(testCounter ${ChimeraTK-ApplicationCore_LIBRARIES} ${Boost_LIBRARIES} ${LibXML++_LIBRARIES})
add_test(testCounter testCounter)

add_executable(test_parsing ${CMAKE_SOURCE_DIR}/test/test_parsing.C  ${library_sources})
target_link_libraries(test_parsing ${ChimeraTK-ApplicationCore_LIBRARIES} ${Boost_LIBRARIES} ${LibXML++_LIBRARIES})
add_test(test_parsing test_parsing)

# Install the executable and the RPC_LIBNO file
if("${CMAKE_INSTALL_PREFIX}" STREQUAL "/export/doocs/server")
  install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${PROJECT_NAME})
  install(FILES ${CMAKE_SOURCE_DIR}/configs/RPC_LIBNO DESTINATION ${PROJECT_NAME})
else()
  install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
endif()
