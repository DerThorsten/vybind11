#-------------------------------------------------------------------------------------------------------------------
# Add The module
#-------------------------------------------------------------------------------------------------------------------



set(PY_MOD_NAME vybind11 )
set(PY_MOD_LIB_NAME _${PY_MOD_NAME})
#include_directories(${Vigra_INCLUDE_DIRS})


# add the python library
pybind11_add_module(${PY_MOD_LIB_NAME}  main.cpp)

# `link` against pybind11 interface module
target_link_libraries(${PY_MOD_LIB_NAME} PUBLIC  
    ${INTERFACE_LIB_NAME}
    pybind11::module)

# include directories for non-modern cmake
message(STATUS "Vigra_INCLUDE_DIRS" ${Vigra_INCLUDE_DIRS})
# set(PY_INCLUDE_DIRS 
#     "${PYTHON_NUMPY_INCLUDE_DIR};${xtensor-python_INCLUDE_DIRS};${Vigra_INCLUDE_DIRS}"
# )
message(STATUS "PY_INCLUDE_DIRS" ${PY_INCLUDE_DIRS})
# target_include_directories(${PY_MOD_LIB_NAME} PUBLIC  ${PY_INCLUDE_DIRS})


get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
message(STATUS "dirs" ${dirs})

install(TARGETS ${PY_MOD_LIB_NAME} 
    DESTINATION ${PYTHON_MODULE_INSTALL_DIR}/${PY_MOD_NAME}/)