
cmake_minimum_required(VERSION 3.1)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
project (vybind11)


set(PROJECT_INCLUDE_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CPP_ROOT_FOLDER_NAME   vybind11)
include_directories(${PROJECT_INCLUDE_DIR})

include(CheckCXXCompilerFlag)
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Intel")
    CHECK_CXX_COMPILER_FLAG("-std=c++11" HAS_CPP11_FLAG)
    if (HAS_CPP11_FLAG)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    else()
        message(FATAL_ERROR "Unsupported compiler -- C++11 support required!")
    endif()
endif() 


# Versionning
# ===========
set(CPP_CONFIG_HEADER_NAME vybind11_config.hpp)
set(PROJECT_CONFIG_FILE ${PROJECT_INCLUDE_DIR}/${CPP_ROOT_FOLDER_NAME}/${CPP_CONFIG_HEADER_NAME})

file(STRINGS ${PROJECT_CONFIG_FILE} project_version_defines
     REGEX "#define VYBIND11_VERSION_(MAJOR|MINOR|PATCH)")
message(STATUS ${PROJECT_CONFIG_FILE})

foreach(ver ${project_version_defines})
    if(ver MATCHES "#define VYBIND11_VERSION_(MAJOR|MINOR|PATCH) +([^ ]+)$")
        set(${PROJECT_NAME}_VERSION_${CMAKE_MATCH_1} "${CMAKE_MATCH_2}" CACHE INTERNAL "")
    endif()
endforeach()

set(${PROJECT_NAME}_VERSION
    ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH})
message(STATUS "Building project v${${PROJECT_NAME}_VERSION}")

# Dependencies
# ============
find_package(Vigra REQUIRED)

# # Build
# # =====


SET(INTERFACE_LIB_NAME vybind11)


file(GLOB_RECURSE ${PROJECT_NAME}_HEADERS  ${PROJECT_INCLUDE_DIR}/*.hpp)

SET(INTERFACE_LIB_INCLUDES  "${PROJECT_INCLUDE_DIR};${Vigra_INCLUDE_DIRS}")

add_library(${INTERFACE_LIB_NAME} INTERFACE)
 

target_include_directories(${INTERFACE_LIB_NAME}
    INTERFACE   $<BUILD_INTERFACE:${INTERFACE_LIB_INCLUDES}>
                $<INSTALL_INTERFACE:include>)


# # Tests
# # ============
# OPTION(BUILD_TESTS "${PROJECT_NAME} test suite" ON)
# # OPTION(DOWNLOAD_DOCTEST "build doctest from downloaded sources" ON)

# # if(DOWNLOAD_DOCTEST)
# #     set(BUILD_TESTS ON)
# # endif()

# if(BUILD_TESTS)
#     add_subdirectory(test)
# endif()



# Benchmark
# ============
OPTION(BUILD_BENCHMARK "${PROJECT_NAME} test suite" ON)
OPTION(DOWNLOAD_GOOGLE_BENCHMARK "build google benchmark from downloaded sources" ON)

if(DOWNLOAD_GOOGLE_BENCHMARK)
    set(BUILD_BENCHMARK ON)
endif()

# if(BUILD_BENCHMARK)
#     add_subdirectory(benchmark)
# endif()

# Python
# ============
OPTION(BUILD_PYTHON "${PROJECT_NAME} python binding" ON)
if(BUILD_PYTHON)
    add_subdirectory(python)
endif()


# Examples
# ============
OPTION(BUILD_EXAMPLES "${PROJECT_NAME} test suite" ON)


# if(BUILD_EXAMPLES)
#     add_subdirectory(examples)
# endif()



# Installation
# ============

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}-targets)

# Makes the project importable from the build directory
export(EXPORT ${PROJECT_NAME}-targets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

# install(FILES ${${PROJECT_NAME}_HEADERS}
#         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CPP_ROOT_FOLDER_NAME})
install(DIRECTORY  ${PROJECT_INCLUDE_DIR}/${CPP_ROOT_FOLDER_NAME}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set(${PROJECT_NAME}_CMAKECONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}" CACHE
    STRING "install path for ${PROJECT_NAME}Config.cmake")

configure_package_config_file(${PROJECT_NAME}Config.cmake.in
                              "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                              INSTALL_DESTINATION ${${PROJECT_NAME}_CMAKECONFIG_INSTALL_DIR})


# ${PROJECT_NAME} is header-only and does not depend on the architecture.
# Remove CMAKE_SIZEOF_VOID_P from ${PROJECT_NAME}ConfigVersion.cmake so that an ${PROJECT_NAME}Config.cmake
# generated for a 64 bit target can be used for 32 bit targets and vice versa.
set(_${PROJECT_NAME}_CMAKE_SIZEOF_VOID_P ${CMAKE_SIZEOF_VOID_P})
unset(CMAKE_SIZEOF_VOID_P)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
                                 VERSION ${${PROJECT_NAME}_VERSION}
                                 COMPATIBILITY AnyNewerVersion)
set(CMAKE_SIZEOF_VOID_P ${_${PROJECT_NAME}_CMAKE_SIZEOF_VOID_P})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${${PROJECT_NAME}_CMAKECONFIG_INSTALL_DIR})
install(EXPORT ${PROJECT_NAME}-targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION ${${PROJECT_NAME}_CMAKECONFIG_INSTALL_DIR})

configure_file(${PROJECT_NAME}.pc.in
               "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
                @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig/")



